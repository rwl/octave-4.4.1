// DO NOT EDIT!
// Generated automatically from /home/jwe/src/octave-stable/liboctave/numeric/LSODE-opts.in.

// This file should not include config.h.  It is only included in other
// C++ source files that should have included config.h before including
// this file.

#include <iomanip>
#include <iostream>

#include "LSODE-opts.h"

#include "defun.h"
#include "pr-output.h"

#include "ovl.h"
#include "utils.h"
#include "pager.h"

static LSODE_options lsode_opts;

#define MAX_TOKENS 3

struct LSODE_options_struct
{
  const char *keyword;
  const char *kw_tok[MAX_TOKENS + 1];
  int min_len[MAX_TOKENS + 1];
  int min_toks_to_match;
};

#define NUM_OPTIONS 8

static LSODE_options_struct LSODE_options_table [] =
{
  { "absolute tolerance",
    { "absolute", "tolerance", nullptr, nullptr, },
    { 1, 0, 0, 0, }, 1, },

  { "relative tolerance",
    { "relative", "tolerance", nullptr, nullptr, },
    { 1, 0, 0, 0, }, 1, },

  { "integration method",
    { "integration", "method", nullptr, nullptr, },
    { 3, 0, 0, 0, }, 1, },

  { "initial step size",
    { "initial", "step", "size", nullptr, },
    { 3, 0, 0, 0, }, 1, },

  { "maximum order",
    { "maximum", "order", nullptr, nullptr, },
    { 2, 1, 0, 0, }, 2, },

  { "maximum step size",
    { "maximum", "step", "size", nullptr, },
    { 2, 1, 0, 0, }, 2, },

  { "minimum step size",
    { "minimum", "step", "size", nullptr, },
    { 2, 0, 0, 0, }, 1, },

  { "step limit",
    { "step", "limit", nullptr, nullptr, },
    { 1, 0, 0, 0, }, 1, },
};

static void
print_LSODE_options (std::ostream& os)
{
  std::ostringstream buf;

  os << "\n"
     << "Options for LSODE include:\n\n"
     << "  keyword                                             value\n"
     << "  -------                                             -----\n";

  LSODE_options_struct *list = LSODE_options_table;

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[0].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    Array<double> val = lsode_opts.absolute_tolerance ();

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

    double val = lsode_opts.relative_tolerance ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[2].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    os << lsode_opts.integration_method () << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[3].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    double val = lsode_opts.initial_step_size ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[4].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    int val = lsode_opts.maximum_order ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[5].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    double val = lsode_opts.maximum_step_size ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[6].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    double val = lsode_opts.minimum_step_size ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[7].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    int val = lsode_opts.step_limit ();

    os << val << "\n";
  }

  os << "\n";
}

static void
set_LSODE_options (const std::string& keyword, const octave_value& val)
{
  LSODE_options_struct *list = LSODE_options_table;

  if (keyword_almost_match (list[0].kw_tok, list[0].min_len,
           keyword, list[0].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> tmp = val.vector_value ();

      lsode_opts.set_absolute_tolerance (tmp);
    }
  else if (keyword_almost_match (list[1].kw_tok, list[1].min_len,
           keyword, list[1].min_toks_to_match, MAX_TOKENS))
    {
      double tmp = val.double_value ();

      lsode_opts.set_relative_tolerance (tmp);
    }
  else if (keyword_almost_match (list[2].kw_tok, list[2].min_len,
           keyword, list[2].min_toks_to_match, MAX_TOKENS))
    {
      std::string tmp = val.string_value ();

      lsode_opts.set_integration_method (tmp);
    }
  else if (keyword_almost_match (list[3].kw_tok, list[3].min_len,
           keyword, list[3].min_toks_to_match, MAX_TOKENS))
    {
      double tmp = val.double_value ();

      lsode_opts.set_initial_step_size (tmp);
    }
  else if (keyword_almost_match (list[4].kw_tok, list[4].min_len,
           keyword, list[4].min_toks_to_match, MAX_TOKENS))
    {
      int tmp = val.int_value ();

      lsode_opts.set_maximum_order (tmp);
    }
  else if (keyword_almost_match (list[5].kw_tok, list[5].min_len,
           keyword, list[5].min_toks_to_match, MAX_TOKENS))
    {
      double tmp = val.double_value ();

      lsode_opts.set_maximum_step_size (tmp);
    }
  else if (keyword_almost_match (list[6].kw_tok, list[6].min_len,
           keyword, list[6].min_toks_to_match, MAX_TOKENS))
    {
      double tmp = val.double_value ();

      lsode_opts.set_minimum_step_size (tmp);
    }
  else if (keyword_almost_match (list[7].kw_tok, list[7].min_len,
           keyword, list[7].min_toks_to_match, MAX_TOKENS))
    {
      int tmp = val.int_value ();

      lsode_opts.set_step_limit (tmp);
    }
  else
    {
      warning ("lsode_options: no match for `%s'", keyword.c_str ());
    }
}

static octave_value_list
show_LSODE_options (const std::string& keyword)
{
  octave_value retval;

  LSODE_options_struct *list = LSODE_options_table;

  if (keyword_almost_match (list[0].kw_tok, list[0].min_len,
           keyword, list[0].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> val = lsode_opts.absolute_tolerance ();

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
      double val = lsode_opts.relative_tolerance ();

      retval = val;
    }
  else if (keyword_almost_match (list[2].kw_tok, list[2].min_len,
           keyword, list[2].min_toks_to_match, MAX_TOKENS))
    {
      retval = lsode_opts.integration_method ();
    }
  else if (keyword_almost_match (list[3].kw_tok, list[3].min_len,
           keyword, list[3].min_toks_to_match, MAX_TOKENS))
    {
      double val = lsode_opts.initial_step_size ();

      retval = val;
    }
  else if (keyword_almost_match (list[4].kw_tok, list[4].min_len,
           keyword, list[4].min_toks_to_match, MAX_TOKENS))
    {
      int val = lsode_opts.maximum_order ();

      retval = static_cast<double> (val);
    }
  else if (keyword_almost_match (list[5].kw_tok, list[5].min_len,
           keyword, list[5].min_toks_to_match, MAX_TOKENS))
    {
      double val = lsode_opts.maximum_step_size ();

      retval = val;
    }
  else if (keyword_almost_match (list[6].kw_tok, list[6].min_len,
           keyword, list[6].min_toks_to_match, MAX_TOKENS))
    {
      double val = lsode_opts.minimum_step_size ();

      retval = val;
    }
  else if (keyword_almost_match (list[7].kw_tok, list[7].min_len,
           keyword, list[7].min_toks_to_match, MAX_TOKENS))
    {
      int val = lsode_opts.step_limit ();

      retval = static_cast<double> (val);
    }
  else
    {
      warning ("lsode_options: no match for `%s'", keyword.c_str ());
    }

  return retval;
}

DEFUN (lsode_options, args, ,
       doc: /* -*- texinfo -*-
@deftypefn  {} {} lsode_options ()
@deftypefnx {} {val =} lsode_options (@var{opt})
@deftypefnx {} {} lsode_options (@var{opt}, @var{val})
Query or set options for the function @code{lsode}.

When called with no arguments, the names of all available options and
their current values are displayed.

Given one argument, return the value of the option @var{opt}.

When called with two arguments, @code{lsode_options} sets the option
@var{opt} to value @var{val}.

Options include

@table @asis
@item @qcode{"absolute tolerance"}
Absolute tolerance.  May be either vector or scalar.  If a vector, it
must match the dimension of the state vector.

@item @qcode{"relative tolerance"}
Relative tolerance parameter.  Unlike the absolute tolerance, this
parameter may only be a scalar.

The local error test applied at each integration step is

@example
@group
  abs (local error in x(i)) <= ...
      rtol * abs (y(i)) + atol(i)
@end group
@end example

@item @qcode{"integration method"}
A string specifying the method of integration to use to solve the ODE
system.  Valid values are

@table @asis
@item  @qcode{"adams"}
@itemx @qcode{"non-stiff"}
No Jacobian used (even if it is available).

@item  @qcode{"bdf"}
@itemx @qcode{"stiff"}
Use stiff backward differentiation formula (BDF) method.  If a
function to compute the Jacobian is not supplied, @code{lsode} will
compute a finite difference approximation of the Jacobian matrix.
@end table

@item @qcode{"initial step size"}
The step size to be attempted on the first step (default is determined
automatically).

@item @qcode{"maximum order"}
Restrict the maximum order of the solution method.  If using the Adams
method, this option must be between 1 and 12.  Otherwise, it must be
between 1 and 5, inclusive.

@item @qcode{"maximum step size"}
Setting the maximum stepsize will avoid passing over very large
regions  (default is not specified).

@item @qcode{"minimum step size"}
The minimum absolute step size allowed (default is 0).

@item @qcode{"step limit"}
Maximum number of steps allowed (default is 100000).
@end table
@end deftypefn */)
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin > 2)
    print_usage ();

  if (nargin == 0)
    {
      print_LSODE_options (octave_stdout);
    }
  else
    {
      std::string keyword = args(0).xstring_value ("lsode_options: expecting keyword as first argument");

      if (nargin == 1)
        retval = show_LSODE_options (keyword);
      else
        set_LSODE_options (keyword, args(1));
    }

  return retval;
}
