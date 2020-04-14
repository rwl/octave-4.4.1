/*

Copyright (C) 2005-2018 David Bateman

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

#include <limits>
#include <string>

#include "Matrix.h"
#include "eigs-base.h"
#include "unwind-prot.h"

#include "defun-dld.h"
#include "error.h"
#include "errwarn.h"
#include "oct-map.h"
#include "ov.h"
#include "ovl.h"
#include "pager.h"
#include "parse.h"
#include "variables.h"

#if defined (HAVE_ARPACK)

// Global pointer for user defined function.
static octave_function *eigs_fcn = nullptr;

// Have we warned about imaginary values returned from user function?
static bool warned_imaginary = false;

// Is this a recursive call?
static int call_depth = 0;

ColumnVector
eigs_func (const ColumnVector& x, int& eigs_error)
{
  ColumnVector retval;
  octave_value_list args;
  args(0) = x;

  if (eigs_fcn)
    {
      octave_value_list tmp;

      try
        {
          tmp = octave::feval (eigs_fcn, args, 1);
        }
      catch (octave::execution_exception& e)
        {
          err_user_supplied_eval (e, "eigs");
        }

      if (tmp.length () && tmp(0).is_defined ())
        {
          if (! warned_imaginary && tmp(0).iscomplex ())
            {
              warning ("eigs: ignoring imaginary part returned from user-supplied function");
              warned_imaginary = true;
            }

          retval = tmp(0).xvector_value ("eigs: evaluation of user-supplied function failed");
        }
      else
        {
          eigs_error = 1;
          err_user_supplied_eval ("eigs");
        }
    }

  return retval;
}

ComplexColumnVector
eigs_complex_func (const ComplexColumnVector& x, int& eigs_error)
{
  ComplexColumnVector retval;
  octave_value_list args;
  args(0) = x;

  if (eigs_fcn)
    {
      octave_value_list tmp;

      try
        {
          tmp = octave::feval (eigs_fcn, args, 1);
        }
      catch (octave::execution_exception& e)
        {
          err_user_supplied_eval (e, "eigs");
        }

      if (tmp.length () && tmp(0).is_defined ())
        {
          retval = tmp(0).complex_vector_value ("eigs: evaluation of user-supplied function failed");
        }
      else
        {
          eigs_error = 1;
          err_user_supplied_eval ("eigs");
        }
    }

  return retval;
}

#endif

DEFMETHOD_DLD (__eigs__, interp, args, nargout,
               doc: /* -*- texinfo -*-
@deftypefn  {} {@var{d} =} __eigs__ (@var{A})
@deftypefnx {} {@var{d} =} __eigs__ (@var{A}, @var{k})
@deftypefnx {} {@var{d} =} __eigs__ (@var{A}, @var{k}, @var{sigma})
@deftypefnx {} {@var{d} =} __eigs__ (@var{A}, @var{k}, @var{sigma}, @var{opts})
@deftypefnx {} {@var{d} =} __eigs__ (@var{A}, @var{B})
@deftypefnx {} {@var{d} =} __eigs__ (@var{A}, @var{B}, @var{k})
@deftypefnx {} {@var{d} =} __eigs__ (@var{A}, @var{B}, @var{k}, @var{sigma})
@deftypefnx {} {@var{d} =} __eigs__ (@var{A}, @var{B}, @var{k}, @var{sigma}, @var{opts})
@deftypefnx {} {@var{d} =} __eigs__ (@var{af}, @var{n})
@deftypefnx {} {@var{d} =} __eigs__ (@var{af}, @var{n}, @var{B})
@deftypefnx {} {@var{d} =} __eigs__ (@var{af}, @var{n}, @var{k})
@deftypefnx {} {@var{d} =} __eigs__ (@var{af}, @var{n}, @var{B}, @var{k})
@deftypefnx {} {@var{d} =} __eigs__ (@var{af}, @var{n}, @var{k}, @var{sigma})
@deftypefnx {} {@var{d} =} __eigs__ (@var{af}, @var{n}, @var{B}, @var{k}, @var{sigma})
@deftypefnx {} {@var{d} =} __eigs__ (@var{af}, @var{n}, @var{k}, @var{sigma}, @var{opts})
@deftypefnx {} {@var{d} =} __eigs__ (@var{af}, @var{n}, @var{B}, @var{k}, @var{sigma}, @var{opts})
@deftypefnx {} {[@var{V}, @var{d}] =} __eigs__ (@var{A}, @dots{})
@deftypefnx {} {[@var{V}, @var{d}] =} __eigs__ (@var{af}, @var{n}, @dots{})
@deftypefnx {} {[@var{V}, @var{d}, @var{flag}] =} __eigs__ (@var{A}, @dots{})
@deftypefnx {} {[@var{V}, @var{d}, @var{flag}] =} __eigs__ (@var{af}, @var{n}, @dots{})
Undocumented internal function.
@end deftypefn */)
{
#if defined (HAVE_ARPACK)

  int nargin = args.length ();

  if (nargin == 0)
    print_usage ();

  octave_value_list retval;

  std::string fcn_name;
  octave_idx_type n = 0;
  octave_idx_type k = 6;
  Complex sigma = 0.;
  double sigmar, sigmai;
  bool have_sigma = false;
  std::string typ = "LM";
  Matrix amm, bmm, bmt;
  ComplexMatrix acm, bcm, bct;
  SparseMatrix asmm, bsmm, bsmt;
  SparseComplexMatrix ascm, bscm, bsct;
  int b_arg = 0;
  bool have_b = false;
  bool have_a_fun = false;
  bool a_is_complex = false;
  bool b_is_complex = false;
  bool symmetric = false;
  bool sym_tested = false;
  bool cholB = false;
  bool a_is_sparse = false;
  ColumnVector permB;
  int arg_offset = 0;
  double tol = std::numeric_limits<double>::epsilon ();
  int maxit = 300;
  int disp = 0;
  octave_idx_type p = -1;
  ColumnVector resid;
  ComplexColumnVector cresid;
  octave_idx_type info = 1;

  warned_imaginary = false;

  octave::unwind_protect frame;

  frame.protect_var (call_depth);
  call_depth++;

  if (call_depth > 1)
    error ("eigs: invalid recursive call");

  if (args(0).is_function_handle () || args(0).is_inline_function ()
      || args(0).is_string ())
    {
      if (args(0).is_string ())
        {
          std::string name = args(0).string_value ();
          std::string fname = "function y = ";
          fcn_name = unique_symbol_name ("__eigs_fcn__");
          fname.append (fcn_name);
          fname.append ("(x) y = ");
          eigs_fcn = extract_function (args(0), "eigs", fcn_name, fname,
                                       "; endfunction");
        }
      else
        eigs_fcn = args(0).function_value ();

      if (! eigs_fcn)
        error ("eigs: unknown function");

      if (nargin < 2)
        error ("eigs: incorrect number of arguments");

      n = args(1).nint_value ();
      arg_offset = 1;
      have_a_fun = true;
    }
  else
    {
      if (args(0).iscomplex ())
        {
          if (args(0).issparse ())
            {
              ascm = (args(0).sparse_complex_matrix_value ());
              a_is_sparse = true;
            }
          else
            acm = (args(0).complex_matrix_value ());
          a_is_complex = true;
          symmetric = false; // ARPACK doesn't special case complex symmetric
          sym_tested = true;
        }
      else
        {
          if (args(0).issparse ())
            {
              asmm = (args(0).sparse_matrix_value ());
              a_is_sparse = true;
            }
          else
            {
              amm = (args(0).matrix_value ());
            }
        }
    }

  // Note hold off reading B until later to avoid issues of double
  // copies of the matrix if B is full/real while A is complex.
  if (nargin > 1 + arg_offset
      && ! (args(1 + arg_offset).is_real_scalar ()))
    {
      if (args(1+arg_offset).iscomplex ())
        {
          b_arg = 1+arg_offset;
          have_b = true;
          b_is_complex = true;
          arg_offset++;
        }
      else
        {
          b_arg = 1+arg_offset;
          have_b = true;
          arg_offset++;
        }
    }

  if (nargin > (1+arg_offset))
    k = args(1+arg_offset).nint_value ();

  if (nargin > (2+arg_offset))
    {
      if (args(2+arg_offset).is_string ())
        {
          typ = args(2+arg_offset).string_value ();

          // Use STL function to convert to upper case
          transform (typ.begin (), typ.end (), typ.begin (), toupper);

          sigma = 0.;
        }
      else
        {
          sigma = args(2+arg_offset).xcomplex_value ("eigs: SIGMA must be a scalar or a string");

          have_sigma = true;
        }
    }

  sigmar = sigma.real ();
  sigmai = sigma.imag ();

  if (nargin > (3+arg_offset))
    {
      if (! args(3+arg_offset).isstruct ())
        error ("eigs: OPTS argument must be a structure");

      octave_scalar_map map = args(3+arg_offset).xscalar_map_value ("eigs: OPTS argument must be a scalar structure");

      octave_value tmp;

      // issym is ignored for complex matrix inputs
      tmp = map.getfield ("issym");
      if (tmp.is_defined () && ! sym_tested)
        {
          symmetric = tmp.double_value () != 0.;
          sym_tested = true;
        }

      // isreal is ignored if A is not a function
      tmp = map.getfield ("isreal");
      if (tmp.is_defined () && have_a_fun)
        a_is_complex = ! (tmp.double_value () != 0.);

      tmp = map.getfield ("tol");
      if (tmp.is_defined ())
        tol = tmp.double_value ();

      tmp = map.getfield ("maxit");
      if (tmp.is_defined ())
        maxit = tmp.nint_value ();

      tmp = map.getfield ("p");
      if (tmp.is_defined ())
        p = tmp.nint_value ();

      tmp = map.getfield ("v0");
      if (tmp.is_defined ())
        {
          if (a_is_complex || b_is_complex)
            cresid = ComplexColumnVector (tmp.complex_vector_value ());
          else
            resid = ColumnVector (tmp.vector_value ());
        }

      tmp = map.getfield ("disp");
      if (tmp.is_defined ())
        disp = tmp.nint_value ();

      tmp = map.getfield ("cholB");
      if (tmp.is_defined ())
        cholB = tmp.double_value () != 0.;

      tmp = map.getfield ("permB");
      if (tmp.is_defined ())
        permB = ColumnVector (tmp.vector_value ()) - 1.0;
    }

  if (nargin > (4+arg_offset))
    error ("eigs: incorrect number of arguments");

  // Test undeclared (no issym) matrix inputs for symmetry
  if (! sym_tested && ! have_a_fun)
    {
      if (a_is_sparse)
        symmetric = asmm.issymmetric ();
      else
        symmetric = amm.issymmetric ();
    }

  if (have_b)
    {
      if (a_is_complex || b_is_complex)
        {
          if (a_is_sparse)
            bscm = args(b_arg).sparse_complex_matrix_value ();
          else
            bcm = args(b_arg).complex_matrix_value ();
        }
      else
        {
          if (a_is_sparse)
            bsmm = args(b_arg).sparse_matrix_value ();
          else
            bmm = args(b_arg).matrix_value ();
        }
    }

  // Mode 1 for SM mode seems unstable for some reason.
  // Use Mode 3 instead, with sigma = 0.
  if (! have_sigma && typ == "SM")
    have_sigma = true;

  octave_idx_type nconv;
  if (a_is_complex || b_is_complex)
    {
      ComplexMatrix eig_vec;
      ComplexColumnVector eig_val;

      if (have_a_fun)
        nconv = EigsComplexNonSymmetricFunc
                (eigs_complex_func, n, typ, sigma, k, p, info, eig_vec,
                 eig_val, cresid, octave_stdout, tol, (nargout > 1), cholB,
                 disp, maxit);
      else if (have_sigma)
        {
          if (a_is_sparse)
            nconv = EigsComplexNonSymmetricMatrixShift
                    (ascm, sigma, k, p, info, eig_vec, eig_val, bscm, permB,
                     cresid, octave_stdout, tol, (nargout > 1), cholB, disp,
                     maxit);
          else
            nconv = EigsComplexNonSymmetricMatrixShift
                    (acm, sigma, k, p, info, eig_vec, eig_val, bcm, permB,
                     cresid, octave_stdout, tol, (nargout > 1), cholB, disp,
                     maxit);
        }
      else
        {
          if (a_is_sparse)
            nconv = EigsComplexNonSymmetricMatrix
                    (ascm, typ, k, p, info, eig_vec, eig_val, bscm, permB,
                     cresid, octave_stdout, tol, (nargout > 1), cholB, disp,
                     maxit);
          else
            nconv = EigsComplexNonSymmetricMatrix
                    (acm, typ, k, p, info, eig_vec, eig_val, bcm, permB,
                     cresid, octave_stdout, tol, (nargout > 1), cholB, disp,
                     maxit);
        }

      if (nargout < 2)
        retval(0) = eig_val;
      else
        retval = ovl (eig_vec, ComplexDiagMatrix (eig_val), double (info));
    }
  else if (sigmai != 0.)
    {
      // Promote real problem to a complex one.
      ComplexMatrix eig_vec;
      ComplexColumnVector eig_val;

      if (have_a_fun)
        nconv = EigsComplexNonSymmetricFunc
                (eigs_complex_func, n, typ,  sigma, k, p, info, eig_vec,
                 eig_val, cresid, octave_stdout, tol, (nargout > 1), cholB,
                 disp, maxit);
      else
        {
          if (a_is_sparse)
            nconv = EigsComplexNonSymmetricMatrixShift
                    (SparseComplexMatrix (asmm), sigma, k, p, info, eig_vec,
                     eig_val, SparseComplexMatrix (bsmm), permB, cresid,
                     octave_stdout, tol, (nargout > 1), cholB, disp, maxit);
          else
            nconv = EigsComplexNonSymmetricMatrixShift
                    (ComplexMatrix (amm), sigma, k, p, info, eig_vec,
                     eig_val, ComplexMatrix (bmm), permB, cresid,
                     octave_stdout, tol, (nargout > 1), cholB, disp, maxit);
        }

      if (nargout < 2)
        retval(0) = eig_val;
      else
        retval = ovl (eig_vec, ComplexDiagMatrix (eig_val), double (info));
    }
  else
    {
      if (symmetric)
        {
          Matrix eig_vec;
          ColumnVector eig_val;

          if (have_a_fun)
            nconv = EigsRealSymmetricFunc
                    (eigs_func, n, typ, sigmar, k, p, info, eig_vec,
                     eig_val, resid, octave_stdout, tol, (nargout > 1),
                     cholB, disp, maxit);
          else if (have_sigma)
            {
              if (a_is_sparse)
                nconv = EigsRealSymmetricMatrixShift
                        (asmm, sigmar, k, p, info, eig_vec, eig_val, bsmm,
                         permB, resid, octave_stdout, tol, (nargout > 1),
                         cholB, disp, maxit);
              else
                nconv = EigsRealSymmetricMatrixShift
                        (amm, sigmar, k, p, info, eig_vec, eig_val, bmm,
                         permB, resid, octave_stdout, tol, (nargout > 1),
                         cholB, disp, maxit);
            }
          else
            {
              if (a_is_sparse)
                nconv = EigsRealSymmetricMatrix
                        (asmm, typ, k, p, info, eig_vec, eig_val, bsmm,
                         permB, resid, octave_stdout, tol, (nargout > 1),
                         cholB, disp, maxit);
              else
                nconv = EigsRealSymmetricMatrix
                        (amm, typ, k, p, info, eig_vec, eig_val, bmm, permB,
                         resid, octave_stdout, tol, (nargout > 1), cholB,
                         disp, maxit);
            }

          if (nargout < 2)
            retval(0) = eig_val;
          else
            retval = ovl (eig_vec, DiagMatrix (eig_val), double (info));
        }
      else
        {
          ComplexMatrix eig_vec;
          ComplexColumnVector eig_val;

          if (have_a_fun)
            nconv = EigsRealNonSymmetricFunc
                    (eigs_func, n, typ, sigmar, k, p, info, eig_vec,
                     eig_val, resid, octave_stdout, tol, (nargout > 1),
                     cholB, disp, maxit);
          else if (have_sigma)
            {
              if (a_is_sparse)
                nconv = EigsRealNonSymmetricMatrixShift
                        (asmm, sigmar, k, p, info, eig_vec, eig_val, bsmm,
                         permB, resid, octave_stdout, tol, (nargout > 1),
                         cholB, disp, maxit);
              else
                nconv = EigsRealNonSymmetricMatrixShift
                        (amm, sigmar, k, p, info, eig_vec, eig_val, bmm,
                         permB, resid, octave_stdout, tol, (nargout > 1),
                         cholB, disp, maxit);
            }
          else
            {
              if (a_is_sparse)
                nconv = EigsRealNonSymmetricMatrix
                        (asmm, typ, k, p, info, eig_vec, eig_val, bsmm,
                         permB, resid, octave_stdout, tol, (nargout > 1),
                         cholB, disp, maxit);
              else
                nconv = EigsRealNonSymmetricMatrix
                        (amm, typ, k, p, info, eig_vec, eig_val, bmm, permB,
                         resid, octave_stdout, tol, (nargout > 1), cholB,
                         disp, maxit);
            }

          if (nargout < 2)
            retval(0) = eig_val;
          else
            retval = ovl (eig_vec, ComplexDiagMatrix (eig_val), double (info));
        }
    }

  if (nconv <= 0)
    warning_with_id ("Octave:eigs:UnconvergedEigenvalues",
                     "eigs: None of the %d requested eigenvalues converged", k);
  else if (nconv < k)
    warning_with_id ("Octave:eigs:UnconvergedEigenvalues",
                     "eigs: Only %d of the %d requested eigenvalues converged",
                     nconv, k);

  if (! fcn_name.empty ())
    {
      octave::symbol_table& symtab = interp.get_symbol_table ();

      symtab.clear_function (fcn_name);
    }

  return retval;

#else

  octave_unused_parameter (interp);
  octave_unused_parameter (args);
  octave_unused_parameter (nargout);

  err_disabled_feature ("eigs", "ARPACK");

#endif
}

/*
## No test needed for internal helper function.
%!assert (1)
*/
