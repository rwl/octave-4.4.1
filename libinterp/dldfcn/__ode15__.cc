/*

Copyright (C) 2016-2018 Francesco Faccio <francesco.faccio@mail.polimi.it>

This file is part of Octave.

Octave is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Octave is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, see
<https://www.gnu.org/licenses/>.

*/

#if defined (HAVE_CONFIG_H)
#  include "config.h"
#endif

#include "dColVector.h"
#include "dMatrix.h"
#include "dSparse.h"

#include "Cell.h"
#include "defun-dld.h"
#include "error.h"
#include "errwarn.h"
#include "oct-map.h"
#include "ov.h"
#include "ovl.h"
#include "parse.h"

#if defined (HAVE_SUNDIALS)

#  if defined (HAVE_IDA_IDA_H)
#    include <ida/ida.h>
#  endif

#  if defined (HAVE_IDA_IDA_DENSE_H)
#    include <ida/ida_dense.h>
#  endif

#  if defined (HAVE_IDA_IDA_KLU_H)
#    include <ida/ida_klu.h>
#    include <sundials/sundials_sparse.h>
#  endif

#  if defined (HAVE_NVECTOR_NVECTOR_SERIAL_H)
#    include <nvector/nvector_serial.h>
#  endif

static inline realtype *
nv_data_s (N_Vector& v)
{
#if defined (HAVE_PRAGMA_GCC_DIAGNOSTIC)
   // Disable warning from GCC about old-style casts in Sundials
   // macro expansions.  Do this in a function so that this
   // diagnostic may still be enabled for the rest of the file.
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

  return NV_DATA_S (v);

#if defined (HAVE_PRAGMA_GCC_DIAGNOSTIC)
   // Restore prevailing warning state for remainder of the file.
#  pragma GCC diagnostic pop
#endif
}

namespace octave
{
  class IDA
  {
  public:

    typedef
    ColumnVector (*DAERHSFuncIDA) (const ColumnVector& x,
                                   const ColumnVector& xdot,
                                   realtype t, octave_function *idaf);

    typedef
    Matrix (*DAEJacFuncDense) (const ColumnVector& x,
                               const ColumnVector& xdot, realtype t,
                               realtype cj, octave_function *idaj);

    typedef
    SparseMatrix (*DAEJacFuncSparse) (const ColumnVector& x,
                                      const ColumnVector& xdot,
                                      realtype t, realtype cj,
                                      octave_function *idaj);

    typedef
    Matrix (*DAEJacCellDense) (Matrix *dfdy, Matrix *dfdyp,
                               realtype cj);

    typedef
    SparseMatrix (*DAEJacCellSparse) (SparseMatrix *dfdy,
                                      SparseMatrix *dfdyp, realtype cj);

    //Default
    IDA (void)
      : t0 (0.0), y0 (), yp0 (), havejac (false), havejacfun (false),
        havejacsparse (false), mem (nullptr), num (), ida_fun (nullptr),
        ida_jac (nullptr), dfdy (nullptr), dfdyp (nullptr), spdfdy (nullptr),
        spdfdyp (nullptr), fun (nullptr), jacfun (nullptr), jacspfun (nullptr),
        jacdcell (nullptr), jacspcell (nullptr)
    { }


    IDA (realtype t, ColumnVector y, ColumnVector yp,
         octave_function *ida_fcn, DAERHSFuncIDA daefun)
      : t0 (t), y0 (y), yp0 (yp), havejac (false), havejacfun (false),
        havejacsparse (false), mem (nullptr), num (), ida_fun (ida_fcn),
        ida_jac (nullptr), dfdy (nullptr), dfdyp (nullptr), spdfdy (nullptr),
        spdfdyp (nullptr), fun (daefun), jacfun (nullptr), jacspfun (nullptr),
        jacdcell (nullptr), jacspcell (nullptr)
    { }


    ~IDA (void) { IDAFree (&mem); }

    IDA&
    set_jacobian (octave_function *jac, DAEJacFuncDense j)
    {
      jacfun = j;
      ida_jac = jac;
      havejac = true;
      havejacfun = true;
      havejacsparse = false;
      return *this;
    }

    IDA&
    set_jacobian (octave_function *jac, DAEJacFuncSparse j)
    {
      jacspfun = j;
      ida_jac = jac;
      havejac = true;
      havejacfun = true;
      havejacsparse = true;
      return *this;
    }

    IDA&
    set_jacobian (Matrix *dy, Matrix *dyp, DAEJacCellDense j)
    {
      jacdcell = j;
      dfdy = dy;
      dfdyp = dyp;
      havejac = true;
      havejacfun = false;
      havejacsparse = false;
      return *this;
    }

    IDA&
    set_jacobian (SparseMatrix *dy, SparseMatrix *dyp,
                  DAEJacCellSparse j)
    {
      jacspcell = j;
      spdfdy = dy;
      spdfdyp = dyp;
      havejac = true;
      havejacfun = false;
      havejacsparse = true;
      return *this;
    }

    void set_userdata (void);

    void initialize (void);

    static ColumnVector NVecToCol (N_Vector& v, long int n);

    static N_Vector ColToNVec (const ColumnVector& data, long int n);

    void
    set_up (void);

    void
    set_tolerance (ColumnVector& abstol, realtype reltol);

    void
    set_tolerance (realtype abstol, realtype reltol);

    static int
    resfun (realtype t, N_Vector yy, N_Vector yyp,
            N_Vector rr, void *user_data);

    void
    resfun_impl (realtype t, N_Vector& yy,
                 N_Vector& yyp, N_Vector& rr);

    static int
    jacdense (long int Neq, realtype t,  realtype cj, N_Vector yy,
              N_Vector yyp, N_Vector, DlsMat JJ, void *user_data,
              N_Vector, N_Vector, N_Vector)
    {
      IDA *self = static_cast <IDA *> (user_data);
      self->jacdense_impl (Neq, t, cj, yy, yyp, JJ);
      return 0;
    }

    void
    jacdense_impl (long int Neq, realtype t, realtype cj,
                   N_Vector& yy, N_Vector& yyp, DlsMat& JJ);

#  if defined (HAVE_SUNDIALS_IDAKLU)
    static int
    jacsparse (realtype t, realtype cj, N_Vector yy, N_Vector yyp,
               N_Vector, SlsMat Jac, void *user_data, N_Vector,
               N_Vector, N_Vector)
    {
      IDA *self = static_cast <IDA *> (user_data);
      self->jacsparse_impl (t, cj, yy, yyp, Jac);
      return 0;
    }

    void
    jacsparse_impl (realtype t, realtype cj, N_Vector& yy,
                    N_Vector& yyp, SlsMat& Jac);
#endif

    void set_maxstep (realtype maxstep);

    void set_initialstep (realtype initialstep);

    bool
    interpolate (int& cont, Matrix& output, ColumnVector& tout,
                 int refine, realtype tend, bool haveoutputfcn,
                 bool haveoutputsel, octave_function *output_fcn,
                 ColumnVector& outputsel, bool haveeventfunction,
                 octave_function *event_fcn, ColumnVector& te,
                 Matrix& ye, ColumnVector& ie, ColumnVector& oldval,
                 ColumnVector& oldisterminal, ColumnVector& olddir,
                 int& temp, ColumnVector& yold);

    bool
    outputfun (octave_function *output_fcn, bool haveoutputsel,
               const ColumnVector& output, realtype tout, realtype tend,
               ColumnVector& outputsel, const std::string& flag);


    bool
    event (octave_function *event_fcn,
           ColumnVector& te, Matrix& ye, ColumnVector& ie,
           realtype tsol, const ColumnVector& y, const std::string& flag,
           const ColumnVector& yp, ColumnVector& oldval,
           ColumnVector& oldisterminal, ColumnVector& olddir,
           int cont, int& temp, realtype told, ColumnVector& yold);

    void set_maxorder (int maxorder);

    octave_value_list
    integrate (const int numt, const ColumnVector& tt,
               const ColumnVector& y0, const ColumnVector& yp0,
               const int refine, bool haverefine, bool haveoutputfcn,
               octave_function *output_fcn, bool haveoutputsel,
               ColumnVector& outputsel, bool haveeventfunction,
               octave_function *event_fcn);

    void print_stat (void);

  private:

    realtype t0;
    ColumnVector y0;
    ColumnVector yp0;
    bool havejac;
    bool havejacfun;
    bool havejacsparse;
    void *mem;
    int num;
    octave_function *ida_fun;
    octave_function *ida_jac;
    Matrix *dfdy;
    Matrix *dfdyp;
    SparseMatrix *spdfdy;
    SparseMatrix *spdfdyp;
    DAERHSFuncIDA fun;
    DAEJacFuncDense jacfun;
    DAEJacFuncSparse jacspfun;
    DAEJacCellDense jacdcell;
    DAEJacCellSparse jacspcell;
  };

  int
  IDA::resfun (realtype t, N_Vector yy, N_Vector yyp, N_Vector rr,
               void *user_data)
  {
    IDA *self = static_cast <IDA *> (user_data);
    self->resfun_impl (t, yy, yyp, rr);
    return 0;
  }

  void
  IDA::resfun_impl (realtype t, N_Vector& yy,
                    N_Vector& yyp, N_Vector& rr)
  {
    BEGIN_INTERRUPT_WITH_EXCEPTIONS;

    ColumnVector y = IDA::NVecToCol (yy, num);

    ColumnVector yp = IDA::NVecToCol (yyp, num);

    ColumnVector res = (*fun) (y, yp, t, ida_fun);

    realtype *puntrr = nv_data_s (rr);

    for (octave_idx_type i = 0; i < num; i++)
      puntrr[i] = res(i);

    END_INTERRUPT_WITH_EXCEPTIONS;
  }

  void
  IDA::set_up (void)
  {
    if (havejacsparse)
      {
#  if defined (HAVE_SUNDIALS_IDAKLU)
        if (IDAKLU (mem, num, num*num, CSC_MAT) != 0)
          error ("IDAKLU solver not initialized");

        IDASlsSetSparseJacFn (mem, IDA::jacsparse);
#  else
        error ("IDAKLU is not available in this version of Octave");
#  endif
      }
    else
      {
        if (IDADense (mem, num) != 0)
          error ("IDADense solver not initialized");

        if (havejac && IDADlsSetDenseJacFn (mem, IDA::jacdense) != 0)
          error ("Dense Jacobian not set");
      }
  }

  void
  IDA::jacdense_impl (long int Neq, realtype t, realtype cj,
                      N_Vector& yy, N_Vector& yyp, DlsMat& JJ)

  {
    BEGIN_INTERRUPT_WITH_EXCEPTIONS;

    ColumnVector y = NVecToCol (yy, Neq);

    ColumnVector yp = NVecToCol (yyp, Neq);

    Matrix jac;

    if (havejacfun)
      jac = (*jacfun) (y, yp, t, cj, ida_jac);
    else
      jac = (*jacdcell) (dfdy, dfdyp, cj);

    std::copy (jac.fortran_vec (),
               jac.fortran_vec () + jac.numel (),
               JJ->data);

    END_INTERRUPT_WITH_EXCEPTIONS;
  }

#  if defined (HAVE_SUNDIALS_IDAKLU)
  void
  IDA::jacsparse_impl (realtype t, realtype cj, N_Vector& yy, N_Vector& yyp,
                       SlsMat& Jac)

  {
    BEGIN_INTERRUPT_WITH_EXCEPTIONS;

    ColumnVector y = NVecToCol (yy, num);

    ColumnVector yp = NVecToCol (yyp, num);

    SparseMatrix jac;

    if (havejacfun)
      jac = (*jacspfun) (y, yp, t, cj, ida_jac);
    else
      jac = (*jacspcell) (spdfdy, spdfdyp, cj);

    SparseSetMatToZero (Jac);
    int *colptrs = *(Jac->colptrs);
    int *rowvals = *(Jac->rowvals);

    for (int i = 0; i < num + 1; i++)
      colptrs[i] = jac.cidx(i);

    for (int i = 0; i < jac.nnz (); i++)
      {
        rowvals[i] = jac.ridx(i);
        Jac->data[i] = jac.data(i);
      }

    END_INTERRUPT_WITH_EXCEPTIONS;
  }
#endif

  ColumnVector
  IDA::NVecToCol (N_Vector& v, long int n)
  {
    ColumnVector data (n);
    realtype *punt = nv_data_s (v);

    for (octave_idx_type i = 0; i < n; i++)
      data(i) = punt[i];

    return data;
  }

  N_Vector
  IDA::ColToNVec (const ColumnVector& data, long int n)
  {
    N_Vector v = N_VNew_Serial (n);

    realtype *punt = nv_data_s (v);

    for (octave_idx_type i = 0; i < n; i++)
      punt[i] = data(i);

    return v;
  }

  void
  IDA::set_userdata (void)
  {
    void *userdata = this;

    if (IDASetUserData (mem, userdata) != 0)
      error ("User data not set");
  }

  void
  IDA::initialize (void)
  {
    num = y0.numel ();
    mem = IDACreate ();

    N_Vector yy = ColToNVec (y0, num);

    N_Vector yyp = ColToNVec (yp0, num);

    IDA::set_userdata ();

    if (IDAInit (mem, IDA::resfun, t0, yy, yyp) != 0)
      error ("IDA not initialized");
  }

  void
  IDA::set_tolerance (ColumnVector& abstol, realtype reltol)
  {
    N_Vector abs_tol = ColToNVec (abstol, num);

    if (IDASVtolerances (mem, reltol, abs_tol) != 0)
      error ("IDA: Tolerance not set");

    N_VDestroy_Serial (abs_tol);
  }

  void
  IDA::set_tolerance (realtype abstol, realtype reltol)
  {
    if (IDASStolerances (mem, reltol, abstol) != 0)
      error ("IDA: Tolerance not set");
  }

  octave_value_list
  IDA::integrate (const int numt, const ColumnVector& tspan,
                  const ColumnVector& y, const ColumnVector& yp,
                  const int refine, bool haverefine, bool haveoutputfcn,
                  octave_function *output_fcn, bool haveoutputsel,
                  ColumnVector& outputsel, bool haveeventfunction,
                  octave_function *event_fcn)
  {
    // Set up output
    ColumnVector tout, yout (num), ypout (num), ysel (outputsel.numel ());
    ColumnVector ie, te, oldval, oldisterminal, olddir;
    Matrix output, ye;
    int cont = 0, temp = 0;
    bool status = 0;
    std::string string = "";
    ColumnVector yold = y;

    realtype tsol = tspan(0);
    realtype tend = tspan(numt-1);

    N_Vector yyp = ColToNVec (yp, num);

    N_Vector yy = ColToNVec (y, num);

    // Initialize OutputFcn
    if (haveoutputfcn)
      status = IDA::outputfun (output_fcn, haveoutputsel, y,
                               tsol, tend, outputsel, "init");

    // Initialize Events
    if (haveeventfunction)
      status = IDA::event (event_fcn, te, ye, ie, tsol, y,
                           "init", yp, oldval, oldisterminal,
                           olddir, cont, temp, tsol, yold);

    if (numt > 2)
      {
        // First output value
        tout.resize (numt);
        tout(0) = tsol;
        output.resize (numt, num);

        for (octave_idx_type i = 0; i < num; i++)
          output.elem (0, i) = y.elem (i);

        //Main loop
        for (octave_idx_type j = 1; j < numt && status == 0; j++)
          {
            // IDANORMAL already interpolates tspan(j)

            if (IDASolve (mem, tspan (j), &tsol, yy, yyp, IDA_NORMAL) != 0)
              error ("IDASolve failed");

            yout = NVecToCol (yy, num);
            ypout = NVecToCol (yyp, num);
            tout(j) = tsol;

            for (octave_idx_type i = 0; i < num; i++)
              output.elem (j, i) = yout.elem (i);

            if (haveoutputfcn)
              status = IDA::outputfun (output_fcn, haveoutputsel, yout, tsol,
                                       tend, outputsel, string);

            if (haveeventfunction)
              status = IDA::event (event_fcn, te, ye, ie, tout(j), yout,
                                   string, ypout, oldval, oldisterminal,
                                   olddir, j, temp, tout(j-1), yold);

            // If integration is stopped, return only the reached steps
            if (status == 1)
              {
                output.resize (j + 1, num);
                tout.resize (j + 1);
              }

          }
      }
    else // numel (tspan) == 2
      {
        // First output value
        tout.resize (1);
        tout(0) = tsol;
        output.resize (1, num);

        for (octave_idx_type i = 0; i < num; i++)
          output.elem (0, i) = y.elem (i);

        bool posdirection = (tend > tsol);

        //main loop
        while (((posdirection == 1 && tsol < tend)
                || (posdirection == 0 && tsol > tend))
               && status == 0)
          {
            if (IDASolve (mem, tend, &tsol, yy, yyp, IDA_ONE_STEP) != 0)
              error ("IDASolve failed");

            if (haverefine)
              status = IDA::interpolate (cont, output, tout, refine, tend,
                                         haveoutputfcn, haveoutputsel,
                                         output_fcn, outputsel,
                                         haveeventfunction, event_fcn, te,
                                         ye, ie, oldval, oldisterminal,
                                         olddir, temp, yold);

            ypout = NVecToCol (yyp, num);
            cont += 1;
            output.resize (cont + 1, num); // This may be not efficient
            tout.resize (cont + 1);
            tout(cont) = tsol;
            yout = NVecToCol (yy, num);

            for (octave_idx_type i = 0; i < num; i++)
              output.elem (cont, i) = yout.elem (i);

            if (haveoutputfcn && ! haverefine && tout(cont) < tend)
              status = IDA::outputfun (output_fcn, haveoutputsel, yout, tsol,
                                       tend, outputsel, string);

            if (haveeventfunction && ! haverefine && tout(cont) < tend)
              status = IDA::event (event_fcn, te, ye, ie, tout(cont), yout,
                                   string, ypout, oldval, oldisterminal,
                                   olddir, cont, temp, tout(cont-1), yold);
          }

        if (status == 0)
          {
            // Interpolate in tend
            N_Vector dky = N_VNew_Serial (num);

            if (IDAGetDky (mem, tend, 0, dky) != 0)
              error ("IDA failed to interpolate y");

            tout(cont) = tend;
            yout = NVecToCol (dky, num);

            for (octave_idx_type i = 0; i < num; i++)
              output.elem (cont, i) = yout.elem (i);

            // Plot final value
            if (haveoutputfcn)
              {
                status = IDA::outputfun (output_fcn, haveoutputsel, yout,
                                         tend, tend, outputsel, string);

                // Events during last step
                if (haveeventfunction)
                  status = IDA::event (event_fcn, te, ye, ie, tend, yout,
                                       string, ypout, oldval, oldisterminal,
                                       olddir, cont, temp, tout(cont-1),
                                       yold);
              }

            N_VDestroy_Serial (dky);
          }

        // Cleanup plotter
        status = IDA::outputfun (output_fcn, haveoutputsel, yout, tend, tend,
                                 outputsel, "done");

      }

    return ovl (tout, output, te, ye, ie);
  }

  bool
  IDA::event (octave_function *event_fcn,
              ColumnVector& te, Matrix& ye, ColumnVector& ie,
              realtype tsol, const ColumnVector& y, const std::string& flag,
              const ColumnVector& yp, ColumnVector& oldval,
              ColumnVector& oldisterminal, ColumnVector& olddir, int cont,
              int& temp, realtype told, ColumnVector& yold)
  {
    bool status = 0;

    octave_value_list args = ovl (tsol, y, yp);

    // cont is the number of steps reached by the solver
    // temp is the number of events registered

    if (flag == "init")
      {
        octave_value_list output = feval (event_fcn, args, 3);
        oldval = output(0).vector_value ();
        oldisterminal = output(1).vector_value ();
        olddir = output(2).vector_value ();
      }
    else if (flag == "")
      {
        ColumnVector index (0);
        octave_value_list output = feval (event_fcn, args, 3);
        ColumnVector val = output(0).vector_value ();
        ColumnVector isterminal = output(1).vector_value ();
        ColumnVector dir = output(2).vector_value ();

        // Get the index of the changed values
        for (octave_idx_type i = 0; i < val.numel (); i++)
          {
            if ((val(i) > 0 && oldval(i) < 0 && dir(i) != -1) // increasing
                || (val(i) < 0 && oldval(i) > 0 && dir(i) != 1)) // decreasing
              {
                index.resize (index.numel () + 1);
                index (index.numel () - 1) = i;
              }
          }

        if (cont == 1 && index.numel () > 0)  // Events in first step
          {
            temp = 1; // register only the first event
            te.resize (1);
            ye.resize (1, num);
            ie.resize (1);

            // Linear interpolation
            ie(0) = index(0);
            te(0) = tsol - val (index(0)) * (tsol - told)
                    / (val (index(0)) - oldval (index(0)));

            ColumnVector ytemp
              = y - ((tsol - te(0)) * (y - yold) / (tsol - told));

            for (octave_idx_type i = 0; i < num; i++)
              ye.elem (0, i) = ytemp.elem (i);

          }
        else if (index.numel () > 0)
          // Not first step: register all events and test if stop integration or not
          {
            te.resize (temp + index.numel ());
            ye.resize (temp + index.numel (), num);
            ie.resize (temp + index.numel ());

            for (octave_idx_type i = 0; i < index.numel (); i++)
              {

                if (isterminal (index(i)) == 1)
                  status = 1; // Stop integration

                // Linear interpolation
                ie(temp+i) = index(i);
                te(temp+i) = tsol - val(index(i)) * (tsol - told)
                             / (val(index(i)) - oldval(index(i)));

                ColumnVector ytemp
                  = y - (tsol - te (temp + i)) * (y - yold) / (tsol - told);

                for (octave_idx_type j = 0; j < num; j++)
                  ye.elem (temp + i, j) = ytemp.elem (j);

              }

            temp += index.numel ();
          }

        // Update variables
        yold = y;
        told = tsol;
        olddir = dir;
        oldval = val;
        oldisterminal = isterminal;
      }

    return status;
  }

  bool
  IDA::interpolate (int& cont, Matrix& output, ColumnVector& tout,
                    int refine, realtype tend, bool haveoutputfcn,
                    bool haveoutputsel, octave_function *output_fcn,
                    ColumnVector& outputsel, bool haveeventfunction,
                    octave_function *event_fcn, ColumnVector& te,
                    Matrix& ye, ColumnVector& ie, ColumnVector& oldval,
                    ColumnVector& oldisterminal, ColumnVector& olddir,
                    int& temp, ColumnVector& yold)
  {
    realtype h = 0, tcur = 0;
    bool status = 0;

    N_Vector dky = N_VNew_Serial (num);

    N_Vector dkyp = N_VNew_Serial (num);

    ColumnVector yout (num);
    ColumnVector ypout (num);
    std::string string = "";

    if (IDAGetLastStep (mem, &h) != 0)
      error ("IDA failed to return last step");

    if (IDAGetCurrentTime (mem, &tcur) != 0)
      error ("IDA failed to return the current time");

    realtype tin = tcur - h;

    realtype step = h / refine;

    for (octave_idx_type i = 1;
         i < refine && tin + step * i < tend && status == 0;
         i++)
      {
        if (IDAGetDky (mem, tin + step*i, 0, dky) != 0)
          error ("IDA failed to interpolate y");

        if (IDAGetDky (mem, tin + step*i, 1, dkyp) != 0)
          error ("IDA failed to interpolate yp");

        cont += 1;
        output.resize (cont + 1, num);
        tout.resize (cont + 1);

        tout(cont) = tin + step * i;
        yout = NVecToCol (dky, num);
        ypout = NVecToCol (dkyp, num);

        for (octave_idx_type j = 0; j < num; j++)
          output.elem (cont, j) = yout.elem (j);

        if (haveoutputfcn)
          status = IDA::outputfun (output_fcn, haveoutputsel, yout,
                                   tout(cont), tend, outputsel, "");

        if (haveeventfunction)
          status = IDA::event (event_fcn, te, ye, ie, tout(cont),
                               yout, string, ypout, oldval,
                               oldisterminal, olddir, cont, temp,
                               tout(cont-1), yold);
      }

    N_VDestroy_Serial (dky);

    return status;
  }

  bool
  IDA::outputfun (octave_function *output_fcn, bool haveoutputsel,
                  const ColumnVector& yout, realtype tsol,
                  realtype tend, ColumnVector& outputsel,
                  const std::string& flag)
  {
    bool status = 0;

    octave_value_list output;
    output(2) = flag;

    ColumnVector ysel (outputsel.numel ());
    if (haveoutputsel)
      {
        for (octave_idx_type i = 0; i < outputsel.numel (); i++)
          ysel(i) = yout(outputsel(i));

        output(1) = ysel;
      }
    else
      output(1) = yout;

    if (flag == "init")
      {
        ColumnVector toutput(2);
        toutput(0) = tsol;
        toutput(1) = tend;
        output(0) = toutput;

        feval (output_fcn, output, 0);
      }
    else if (flag == "")
      {
        output(0) = tsol;
        octave_value_list val = feval (output_fcn, output, 1);
        status = val(0).bool_value ();
      }
    else
      {
        // Cleanup plotter
        output(0) = tend;
        feval (output_fcn, output, 0);
      }

    return status;
  }

  void
  IDA::set_maxstep (realtype maxstep)
  {
    if (IDASetMaxStep (mem, maxstep) != 0)
      error ("IDA: Max Step not set");
  }

  void
  IDA::set_initialstep (realtype initialstep)
  {
    if (IDASetInitStep (mem, initialstep) != 0)
      error ("IDA: Initial Step not set");
  }

  void
  IDA::set_maxorder (int maxorder)
  {
    if (IDASetMaxOrd (mem, maxorder) != 0)
      error ("IDA: Max Order not set");
  }

  void
  IDA::print_stat (void)
  {
    long int nsteps = 0, netfails = 0, nrevals = 0;

    if (IDAGetNumSteps (mem, &nsteps) != 0)
      error ("IDA failed to return the number of internal steps");

    if (IDAGetNumErrTestFails (mem, &netfails) != 0)
      error ("IDA failed to return the number of internal errors");

    if (IDAGetNumResEvals (mem, &nrevals) != 0)
      error ("IDA failed to return the number of residual evaluations");

    std::cout << nsteps << " successful steps\n";
    std::cout << netfails << " failed attempts\n";
    std::cout << nrevals << " function evaluations\n";
    // std::cout << " partial derivatives\n";
    // std::cout << " LU decompositions\n";
    // std::cout << " solutions of linear systems\n";
  }

  ColumnVector
  ida_user_function (const ColumnVector& x, const ColumnVector& xdot,
                     double t, octave_function *ida_fc)
  {
    octave_value_list tmp;

    try
      {
        tmp = feval (ida_fc, ovl (t, x, xdot), 1);
      }
    catch (execution_exception& e)
      {
        err_user_supplied_eval (e, "__ode15__");
      }

    return tmp(0).vector_value ();
  }

  Matrix
  ida_dense_jac (const ColumnVector& x, const ColumnVector& xdot,
                 double t, double cj, octave_function *ida_jc)
  {
    octave_value_list tmp;

    try
      {
        tmp = feval (ida_jc, ovl (t, x, xdot), 2);
      }
    catch (execution_exception& e)
      {
        err_user_supplied_eval (e, "__ode15__");
      }

    return tmp(0).matrix_value () + cj * tmp(1).matrix_value ();
  }

  SparseMatrix
  ida_sparse_jac (const ColumnVector& x, const ColumnVector& xdot,
                  double t, double cj, octave_function *ida_jc)
  {
    octave_value_list tmp;

    try
      {
        tmp = feval (ida_jc, ovl (t, x, xdot), 2);
      }
    catch (execution_exception& e)
      {
        err_user_supplied_eval (e, "__ode15__");
      }

    return tmp(0).sparse_matrix_value () + cj * tmp(1).sparse_matrix_value ();
  }

  Matrix
  ida_dense_cell_jac (Matrix *dfdy, Matrix *dfdyp, double cj)
  {
    return (*dfdy) + cj * (*dfdyp);
  }

  SparseMatrix
  ida_sparse_cell_jac (SparseMatrix *spdfdy, SparseMatrix *spdfdyp,
                       double cj)
  {
    return (*spdfdy) + cj * (*spdfdyp);
  }

  octave_value_list
  do_ode15 (octave_function *ida_fcn,
            const ColumnVector& tspan,
            const int numt,
            const realtype t0,
            const ColumnVector& y0,
            const ColumnVector& yp0,
            const octave_scalar_map& options)
  {
    octave_value_list retval;

    // Create object
    IDA dae (t0, y0, yp0, ida_fcn, ida_user_function);

    // Set Jacobian
    bool havejac = options.getfield ("havejac").bool_value ();

    bool havejacsparse = options.getfield ("havejacsparse").bool_value ();

    bool havejacfun = options.getfield ("havejacfun").bool_value ();

    Matrix ida_dfdy, ida_dfdyp, *dfdy, *dfdyp;
    SparseMatrix ida_spdfdy, ida_spdfdyp, *spdfdy, *spdfdyp;
    octave_function *ida_jac;
    Cell jaccell;

    if (havejac)
      {
        if (havejacfun)
          {
            ida_jac = options.getfield ("Jacobian").function_value ();

            if (havejacsparse)
              dae.set_jacobian (ida_jac, ida_sparse_jac);
            else
              dae.set_jacobian (ida_jac, ida_dense_jac);
          }
        else
          {
            jaccell = options.getfield ("Jacobian").cell_value ();

            if (havejacsparse)
              {
                ida_spdfdy = jaccell(0).sparse_matrix_value ();
                ida_spdfdyp = jaccell(1).sparse_matrix_value ();
                spdfdy = &ida_spdfdy;
                spdfdyp = &ida_spdfdyp;
                dae.set_jacobian (spdfdy, spdfdyp, ida_sparse_cell_jac);
              }
            else
              {
                ida_dfdy = jaccell(0).matrix_value ();
                ida_dfdyp = jaccell(1).matrix_value ();
                dfdy = &ida_dfdy;
                dfdyp = &ida_dfdyp;
                dae.set_jacobian (dfdy, dfdyp, ida_dense_cell_jac);
              }
          }
      }

    // Initialize IDA
    dae.initialize ();

    // Set tolerances
    realtype rel_tol = options.getfield("RelTol").double_value ();

    bool haveabstolvec = options.getfield ("haveabstolvec").bool_value ();

    if (haveabstolvec)
      {
        ColumnVector abs_tol = options.getfield("AbsTol").vector_value ();

        dae.set_tolerance (abs_tol, rel_tol);
      }
    else
      {
        realtype abs_tol = options.getfield("AbsTol").double_value ();

        dae.set_tolerance (abs_tol, rel_tol);
      }

    //Set max step
    realtype maxstep = options.getfield("MaxStep").double_value ();

    dae.set_maxstep (maxstep);

    //Set initial step
    if (! options.getfield("InitialStep").isempty ())
      {
        realtype initialstep = options.getfield("InitialStep").double_value ();

        dae.set_initialstep (initialstep);
      }

    //Set max order FIXME: it doesn't work
    int maxorder = options.getfield("MaxOrder").int_value ();

    dae.set_maxorder (maxorder);

    //Set Refine
    const int refine = options.getfield("Refine").int_value ();

    bool haverefine = (refine > 1);

    octave_function *output_fcn = nullptr;
    ColumnVector outputsel;

    // OutputFcn
    bool haveoutputfunction
      = options.getfield("haveoutputfunction").bool_value ();

    if (haveoutputfunction)
      output_fcn = options.getfield("OutputFcn").function_value ();

    // OutputSel
    bool haveoutputsel = options.getfield("haveoutputselection").bool_value ();

    if (haveoutputsel)
      outputsel = options.getfield("OutputSel").vector_value ();

    octave_function *event_fcn = nullptr;

    // Events
    bool haveeventfunction
      = options.getfield("haveeventfunction").bool_value ();

    if (haveeventfunction)
      event_fcn = options.getfield("Events").function_value ();

    // Set up linear solver
    dae.set_up ();

    // Integrate
    retval = dae.integrate (numt, tspan, y0, yp0, refine,
                            haverefine, haveoutputfunction,
                            output_fcn, haveoutputsel, outputsel,
                            haveeventfunction, event_fcn);

    // Statistics
    bool havestats = options.getfield("havestats").bool_value ();

    if (havestats)
      dae.print_stat ();

    return retval;
  }
}
#endif


DEFUN_DLD (__ode15__, args, ,
           doc: /* -*- texinfo -*-
@deftypefn {} {@var{t}, @var{y} =} __ode15__ (@var{fun}, @var{tspan}, @var{y0}, @var{yp0}, @var{options})
Undocumented internal function.
@end deftypefn */)
{

#if defined (HAVE_SUNDIALS)

  // Check number of parameters
  int nargin = args.length ();

  if (nargin != 5)
    print_usage ();

  // Check odefun
  octave_function *ida_fcn = nullptr;

  octave_value f_arg = args(0);

  if (! f_arg.is_function_handle ())
    error ("__ode15__: odefun must be a function handle");

  ida_fcn = f_arg.function_value ();

  // Check input tspan
  ColumnVector tspan
    = args(1).xvector_value ("__ode15__: TRANGE must be a vector of numbers");

  int numt = tspan.numel ();

  realtype t0 = tspan (0);

  if (numt < 2)
    error ("__ode15__: TRANGE must contain at least 2 elements");
  else if (! tspan.issorted () || tspan(0) == tspan(numt - 1))
    error ("__ode15__: TRANGE must be strictly monotonic");

  // input y0 and yp0
  ColumnVector y0
    = args(2).xvector_value ("__ode15__: initial state y0 must be a vector");

  ColumnVector yp0
    = args(3).xvector_value ("__ode15__: initial state yp0 must be a vector");


  if (y0.numel () != yp0.numel ())
    error ("__ode15__: initial state y0 and yp0 must have the same length");
  else if (y0.numel () < 1)
    error ("__ode15__: initial state yp0 must be a vector or a scalar");


  if (! args(4).isstruct ())
    error ("__ode15__: OPTS argument must be a structure");

  octave_scalar_map options
    = args(4).xscalar_map_value ("__ode15__: OPTS argument must be a scalar structure");


  return octave::do_ode15 (ida_fcn, tspan, numt, t0, y0, yp0, options);


#else

  octave_unused_parameter (args);

  err_disabled_feature ("__ode15__", "sundials_ida, sundials_nvecserial");

#endif
}
