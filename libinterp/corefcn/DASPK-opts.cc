// DO NOT EDIT!
// Generated automatically from /home/jwe/src/octave-stable/liboctave/numeric/DASPK-opts.in.

// This file should not include config.h.  It is only included in other
// C++ source files that should have included config.h before including
// this file.

#include <iomanip>
#include <iostream>

#include "DASPK-opts.h"

#include "defun.h"
#include "pr-output.h"

#include "ovl.h"
#include "utils.h"
#include "pager.h"

static DASPK_options daspk_opts;

#define MAX_TOKENS 6

struct DASPK_options_struct
{
  const char *keyword;
  const char *kw_tok[MAX_TOKENS + 1];
  int min_len[MAX_TOKENS + 1];
  int min_toks_to_match;
};

#define NUM_OPTIONS 13

static DASPK_options_struct DASPK_options_table [] =
{
  { "absolute tolerance",
    { "absolute", "tolerance", nullptr, nullptr, nullptr, nullptr, nullptr, },
    { 2, 0, 0, 0, 0, 0, 0, }, 1, },

  { "relative tolerance",
    { "relative", "tolerance", nullptr, nullptr, nullptr, nullptr, nullptr, },
    { 1, 0, 0, 0, 0, 0, 0, }, 1, },

  { "compute consistent initial condition",
    { "compute", "consistent", "initial", "condition", nullptr, nullptr, nullptr, },
    { 1, 0, 0, 0, 0, 0, 0, }, 1, },

  { "use initial condition heuristics",
    { "use", "initial", "condition", "heuristics", nullptr, nullptr, nullptr, },
    { 1, 0, 0, 0, 0, 0, 0, }, 1, },

  { "initial condition heuristics",
    { "initial", "condition", "heuristics", nullptr, nullptr, nullptr, nullptr, },
    { 3, 1, 0, 0, 0, 0, 0, }, 2, },

  { "print initial condition info",
    { "print", "initial", "condition", "info", nullptr, nullptr, nullptr, },
    { 1, 0, 0, 0, 0, 0, 0, }, 1, },

  { "exclude algebraic variables from error test",
    { "exclude", "algebraic", "variables", "from", "error", "test", nullptr, },
    { 2, 0, 0, 0, 0, 0, 0, }, 1, },

  { "algebraic variables",
    { "algebraic", "variables", nullptr, nullptr, nullptr, nullptr, nullptr, },
    { 2, 0, 0, 0, 0, 0, 0, }, 1, },

  { "enforce inequality constraints",
    { "enforce", "inequality", "constraints", nullptr, nullptr, nullptr, nullptr, },
    { 2, 0, 0, 0, 0, 0, 0, }, 1, },

  { "inequality constraint types",
    { "inequality", "constraint", "types", nullptr, nullptr, nullptr, nullptr, },
    { 3, 0, 0, 0, 0, 0, 0, }, 1, },

  { "initial step size",
    { "initial", "step", "size", nullptr, nullptr, nullptr, nullptr, },
    { 3, 1, 0, 0, 0, 0, 0, }, 2, },

  { "maximum order",
    { "maximum", "order", nullptr, nullptr, nullptr, nullptr, nullptr, },
    { 1, 1, 0, 0, 0, 0, 0, }, 2, },

  { "maximum step size",
    { "maximum", "step", "size", nullptr, nullptr, nullptr, nullptr, },
    { 1, 1, 0, 0, 0, 0, 0, }, 2, },
};

static void
print_DASPK_options (std::ostream& os)
{
  std::ostringstream buf;

  os << "\n"
     << "Options for DASPK include:\n\n"
     << "  keyword                                             value\n"
     << "  -------                                             -----\n";

  DASPK_options_struct *list = DASPK_options_table;

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[0].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    Array<double> val = daspk_opts.absolute_tolerance ();

    if (val.numel () == 1)
      {
        os << val(0) << "\n";
      }
    else
      {
        os << "\n\n";
        Matrix tmp = Matrix (ColumnVector (val));
        octave_print_internal (os, tmp, false, 2);
        os << "\n\n";
      }
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[1].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    Array<double> val = daspk_opts.relative_tolerance ();

    if (val.numel () == 1)
      {
        os << val(0) << "\n";
      }
    else
      {
        os << "\n\n";
        Matrix tmp = Matrix (ColumnVector (val));
        octave_print_internal (os, tmp, false, 2);
        os << "\n\n";
      }
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[2].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    int val = daspk_opts.compute_consistent_initial_condition ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[3].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    int val = daspk_opts.use_initial_condition_heuristics ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[4].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    Array<double> val = daspk_opts.initial_condition_heuristics ();

    if (val.numel () == 1)
      {
        os << val(0) << "\n";
      }
    else
      {
        os << "\n\n";
        Matrix tmp = Matrix (ColumnVector (val));
        octave_print_internal (os, tmp, false, 2);
        os << "\n\n";
      }
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[5].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    int val = daspk_opts.print_initial_condition_info ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[6].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    int val = daspk_opts.exclude_algebraic_variables_from_error_test ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[7].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    Array<octave_idx_type> val = daspk_opts.algebraic_variables ();

    if (val.numel () == 1)
      {
        os << val(0) << "\n";
      }
    else
      {
        os << "\n\n";
        octave_idx_type len = val.numel ();
        Matrix tmp (len, 1);
        for (octave_idx_type i = 0; i < len; i++)
          tmp(i,0) = val(i);
        octave_print_internal (os, tmp, false, 2);
        os << "\n\n";
      }
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[8].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    int val = daspk_opts.enforce_inequality_constraints ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[9].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    Array<octave_idx_type> val = daspk_opts.inequality_constraint_types ();

    if (val.numel () == 1)
      {
        os << val(0) << "\n";
      }
    else
      {
        os << "\n\n";
        octave_idx_type len = val.numel ();
        Matrix tmp (len, 1);
        for (octave_idx_type i = 0; i < len; i++)
          tmp(i,0) = val(i);
        octave_print_internal (os, tmp, false, 2);
        os << "\n\n";
      }
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[10].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    double val = daspk_opts.initial_step_size ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[11].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    int val = daspk_opts.maximum_order ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[12].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    double val = daspk_opts.maximum_step_size ();

    os << val << "\n";
  }

  os << "\n";
}

static void
set_DASPK_options (const std::string& keyword, const octave_value& val)
{
  DASPK_options_struct *list = DASPK_options_table;

  if (keyword_almost_match (list[0].kw_tok, list[0].min_len,
           keyword, list[0].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> tmp = val.vector_value ();

      daspk_opts.set_absolute_tolerance (tmp);
    }
  else if (keyword_almost_match (list[1].kw_tok, list[1].min_len,
           keyword, list[1].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> tmp = val.vector_value ();

      daspk_opts.set_relative_tolerance (tmp);
    }
  else if (keyword_almost_match (list[2].kw_tok, list[2].min_len,
           keyword, list[2].min_toks_to_match, MAX_TOKENS))
    {
      int tmp = val.int_value ();

      daspk_opts.set_compute_consistent_initial_condition (tmp);
    }
  else if (keyword_almost_match (list[3].kw_tok, list[3].min_len,
           keyword, list[3].min_toks_to_match, MAX_TOKENS))
    {
      int tmp = val.int_value ();

      daspk_opts.set_use_initial_condition_heuristics (tmp);
    }
  else if (keyword_almost_match (list[4].kw_tok, list[4].min_len,
           keyword, list[4].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> tmp = val.vector_value ();

      daspk_opts.set_initial_condition_heuristics (tmp);
    }
  else if (keyword_almost_match (list[5].kw_tok, list[5].min_len,
           keyword, list[5].min_toks_to_match, MAX_TOKENS))
    {
      int tmp = val.int_value ();

      daspk_opts.set_print_initial_condition_info (tmp);
    }
  else if (keyword_almost_match (list[6].kw_tok, list[6].min_len,
           keyword, list[6].min_toks_to_match, MAX_TOKENS))
    {
      int tmp = val.int_value ();

      daspk_opts.set_exclude_algebraic_variables_from_error_test (tmp);
    }
  else if (keyword_almost_match (list[7].kw_tok, list[7].min_len,
           keyword, list[7].min_toks_to_match, MAX_TOKENS))
    {
      Array<int> tmp = val.int_vector_value ();

      daspk_opts.set_algebraic_variables (tmp);
    }
  else if (keyword_almost_match (list[8].kw_tok, list[8].min_len,
           keyword, list[8].min_toks_to_match, MAX_TOKENS))
    {
      int tmp = val.int_value ();

      daspk_opts.set_enforce_inequality_constraints (tmp);
    }
  else if (keyword_almost_match (list[9].kw_tok, list[9].min_len,
           keyword, list[9].min_toks_to_match, MAX_TOKENS))
    {
      Array<int> tmp = val.int_vector_value ();

      daspk_opts.set_inequality_constraint_types (tmp);
    }
  else if (keyword_almost_match (list[10].kw_tok, list[10].min_len,
           keyword, list[10].min_toks_to_match, MAX_TOKENS))
    {
      double tmp = val.double_value ();

      daspk_opts.set_initial_step_size (tmp);
    }
  else if (keyword_almost_match (list[11].kw_tok, list[11].min_len,
           keyword, list[11].min_toks_to_match, MAX_TOKENS))
    {
      int tmp = val.int_value ();

      daspk_opts.set_maximum_order (tmp);
    }
  else if (keyword_almost_match (list[12].kw_tok, list[12].min_len,
           keyword, list[12].min_toks_to_match, MAX_TOKENS))
    {
      double tmp = val.double_value ();

      daspk_opts.set_maximum_step_size (tmp);
    }
  else
    {
      warning ("daspk_options: no match for `%s'", keyword.c_str ());
    }
}

static octave_value_list
show_DASPK_options (const std::string& keyword)
{
  octave_value retval;

  DASPK_options_struct *list = DASPK_options_table;

  if (keyword_almost_match (list[0].kw_tok, list[0].min_len,
           keyword, list[0].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> val = daspk_opts.absolute_tolerance ();

      if (val.numel () == 1)
        {
          retval = val(0);
        }
      else
        {
          retval = ColumnVector (val);
        }
    }
  else if (keyword_almost_match (list[1].kw_tok, list[1].min_len,
           keyword, list[1].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> val = daspk_opts.relative_tolerance ();

      if (val.numel () == 1)
        {
          retval = val(0);
        }
      else
        {
          retval = ColumnVector (val);
        }
    }
  else if (keyword_almost_match (list[2].kw_tok, list[2].min_len,
           keyword, list[2].min_toks_to_match, MAX_TOKENS))
    {
      int val = daspk_opts.compute_consistent_initial_condition ();

      retval = static_cast<double> (val);
    }
  else if (keyword_almost_match (list[3].kw_tok, list[3].min_len,
           keyword, list[3].min_toks_to_match, MAX_TOKENS))
    {
      int val = daspk_opts.use_initial_condition_heuristics ();

      retval = static_cast<double> (val);
    }
  else if (keyword_almost_match (list[4].kw_tok, list[4].min_len,
           keyword, list[4].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> val = daspk_opts.initial_condition_heuristics ();

      if (val.numel () == 1)
        {
          retval = val(0);
        }
      else
        {
          retval = ColumnVector (val);
        }
    }
  else if (keyword_almost_match (list[5].kw_tok, list[5].min_len,
           keyword, list[5].min_toks_to_match, MAX_TOKENS))
    {
      int val = daspk_opts.print_initial_condition_info ();

      retval = static_cast<double> (val);
    }
  else if (keyword_almost_match (list[6].kw_tok, list[6].min_len,
           keyword, list[6].min_toks_to_match, MAX_TOKENS))
    {
      int val = daspk_opts.exclude_algebraic_variables_from_error_test ();

      retval = static_cast<double> (val);
    }
  else if (keyword_almost_match (list[7].kw_tok, list[7].min_len,
           keyword, list[7].min_toks_to_match, MAX_TOKENS))
    {
      Array<octave_idx_type> val = daspk_opts.algebraic_variables ();

      if (val.numel () == 1)
        {
          retval = static_cast<double> (val(0));
        }
      else
        {
          octave_idx_type len = val.numel ();
          ColumnVector tmp (len);
          for (octave_idx_type i = 0; i < len; i++)
            tmp(i) = val(i);
          retval = tmp;
        }
    }
  else if (keyword_almost_match (list[8].kw_tok, list[8].min_len,
           keyword, list[8].min_toks_to_match, MAX_TOKENS))
    {
      int val = daspk_opts.enforce_inequality_constraints ();

      retval = static_cast<double> (val);
    }
  else if (keyword_almost_match (list[9].kw_tok, list[9].min_len,
           keyword, list[9].min_toks_to_match, MAX_TOKENS))
    {
      Array<octave_idx_type> val = daspk_opts.inequality_constraint_types ();

      if (val.numel () == 1)
        {
          retval = static_cast<double> (val(0));
        }
      else
        {
          octave_idx_type len = val.numel ();
          ColumnVector tmp (len);
          for (octave_idx_type i = 0; i < len; i++)
            tmp(i) = val(i);
          retval = tmp;
        }
    }
  else if (keyword_almost_match (list[10].kw_tok, list[10].min_len,
           keyword, list[10].min_toks_to_match, MAX_TOKENS))
    {
      double val = daspk_opts.initial_step_size ();

      retval = val;
    }
  else if (keyword_almost_match (list[11].kw_tok, list[11].min_len,
           keyword, list[11].min_toks_to_match, MAX_TOKENS))
    {
      int val = daspk_opts.maximum_order ();

      retval = static_cast<double> (val);
    }
  else if (keyword_almost_match (list[12].kw_tok, list[12].min_len,
           keyword, list[12].min_toks_to_match, MAX_TOKENS))
    {
      double val = daspk_opts.maximum_step_size ();

      retval = val;
    }
  else
    {
      warning ("daspk_options: no match for `%s'", keyword.c_str ());
    }

  return retval;
}

DEFUN (daspk_options, args, ,
       doc: /* -*- texinfo -*-
@deftypefn  {} {} daspk_options ()
@deftypefnx {} {val =} daspk_options (@var{opt})
@deftypefnx {} {} daspk_options (@var{opt}, @var{val})
Query or set options for the function @code{daspk}.

When called with no arguments, the names of all available options and
their current values are displayed.

Given one argument, return the value of the option @var{opt}.

When called with two arguments, @code{daspk_options} sets the option
@var{opt} to value @var{val}.

Options include

@table @asis
@item @qcode{"absolute tolerance"}
Absolute tolerance.  May be either vector or scalar.  If a vector, it
must match the dimension of the state vector, and the relative
tolerance must also be a vector of the same length.

@item @qcode{"relative tolerance"}
Relative tolerance.  May be either vector or scalar.  If a vector, it
must match the dimension of the state vector, and the absolute
tolerance must also be a vector of the same length.

The local error test applied at each integration step is

@example
@group
  abs (local error in x(i))
       <= rtol(i) * abs (Y(i)) + atol(i)
@end group
@end example

@item @qcode{"compute consistent initial condition"}
Denoting the differential variables in the state vector by @samp{Y_d}
and the algebraic variables by @samp{Y_a}, @code{ddaspk} can solve
one of two initialization problems:

@enumerate
@item Given Y_d, calculate Y_a and Y'_d

@item Given Y', calculate Y.
@end enumerate

In either case, initial values for the given components are input, and
initial guesses for the unknown components must also be provided as
input.  Set this option to 1 to solve the first problem, or 2 to solve
the second (the default is 0, so you must provide a set of
initial conditions that are consistent).

If this option is set to a nonzero value, you must also set the
@qcode{"algebraic variables"} option to declare which variables in the
problem are algebraic.

@item @qcode{"use initial condition heuristics"}
Set to a nonzero value to use the initial condition heuristics options
described below.

@item @qcode{"initial condition heuristics"}
A vector of the following parameters that can be used to control the
initial condition calculation.

@table @code
@item MXNIT
Maximum number of Newton iterations (default is 5).

@item MXNJ
Maximum number of Jacobian evaluations (default is 6).

@item MXNH
Maximum number of values of the artificial stepsize parameter to be
tried if the @qcode{"compute consistent initial condition"} option has
been set to 1 (default is 5).

Note that the maximum total number of Newton iterations allowed is
@code{MXNIT*MXNJ*MXNH} if the @qcode{"compute consistent initial
condition"} option has been set to 1 and @code{MXNIT*MXNJ} if it is
set to 2.

@item LSOFF
Set to a nonzero value to disable the linesearch algorithm (default is
0).

@item STPTOL
Minimum scaled step in linesearch algorithm (default is eps^(2/3)).

@item EPINIT
Swing factor in the Newton iteration convergence test.  The test is
applied to the residual vector, premultiplied by the approximate
Jacobian.  For convergence, the weighted RMS norm of this vector
(scaled by the error weights) must be less than @code{EPINIT*EPCON},
where @code{EPCON} = 0.33 is the analogous test constant used in the
time steps.  The default is @code{EPINIT} = 0.01.
@end table

@item @qcode{"print initial condition info"}
Set this option to a nonzero value to display detailed information
about the initial condition calculation (default is 0).

@item @qcode{"exclude algebraic variables from error test"}
Set to a nonzero value to exclude algebraic variables from the error
test.  You must also set the @qcode{"algebraic variables"} option to
declare which variables in the problem are algebraic (default is 0).

@item @qcode{"algebraic variables"}
A vector of the same length as the state vector.  A nonzero element
indicates that the corresponding element of the state vector is an
algebraic variable (i.e., its derivative does not appear explicitly
in the equation set).

This option is required by the
@qcode{"compute consistent initial condition"} and
@qcode{"exclude algebraic variables from error test"} options.

@item @qcode{"enforce inequality constraints"}
Set to one of the following values to enforce the inequality
constraints specified by the @qcode{"inequality constraint types"}
option (default is 0).

@enumerate
@item To have constraint checking only in the initial condition calculation.

@item To enforce constraint checking during the integration.

@item To enforce both options 1 and 2.
@end enumerate

@item @qcode{"inequality constraint types"}
A vector of the same length as the state specifying the type of
inequality constraint.  Each element of the vector corresponds to an
element of the state and should be assigned one of the following
codes

@table @asis
@item -2
Less than zero.

@item -1
Less than or equal to zero.

@item 0
Not constrained.

@item 1
Greater than or equal to zero.

@item 2
Greater than zero.
@end table

This option only has an effect if the
@qcode{"enforce inequality constraints"} option is nonzero.

@item @qcode{"initial step size"}
Differential-algebraic problems may occasionally suffer from severe
scaling difficulties on the first step.  If you know a great deal
about the scaling of your problem, you can help to alleviate this
problem by specifying an initial stepsize (default is computed
automatically).

@item @qcode{"maximum order"}
Restrict the maximum order of the solution method.  This option must
be between 1 and 5, inclusive (default is 5).

@item @qcode{"maximum step size"}
Setting the maximum stepsize will avoid passing over very large
regions (default is not specified).
@end table
@end deftypefn */)
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin > 2)
    print_usage ();

  if (nargin == 0)
    {
      print_DASPK_options (octave_stdout);
    }
  else
    {
      std::string keyword = args(0).xstring_value ("daspk_options: expecting keyword as first argument");

      if (nargin == 1)
        retval = show_DASPK_options (keyword);
      else
        set_DASPK_options (keyword, args(1));
    }

  return retval;
}
