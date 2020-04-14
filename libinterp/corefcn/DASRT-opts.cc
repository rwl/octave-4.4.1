// DO NOT EDIT!
// Generated automatically from /home/jwe/src/octave-stable/liboctave/numeric/DASRT-opts.in.

// This file should not include config.h.  It is only included in other
// C++ source files that should have included config.h before including
// this file.

#include <iomanip>
#include <iostream>

#include "DASRT-opts.h"

#include "defun.h"
#include "pr-output.h"

#include "ovl.h"
#include "utils.h"
#include "pager.h"

static DASRT_options dasrt_opts;

#define MAX_TOKENS 3

struct DASRT_options_struct
{
  const char *keyword;
  const char *kw_tok[MAX_TOKENS + 1];
  int min_len[MAX_TOKENS + 1];
  int min_toks_to_match;
};

#define NUM_OPTIONS 6

static DASRT_options_struct DASRT_options_table [] =
{
  { "absolute tolerance",
    { "absolute", "tolerance", nullptr, nullptr, },
    { 1, 0, 0, 0, }, 1, },

  { "relative tolerance",
    { "relative", "tolerance", nullptr, nullptr, },
    { 1, 0, 0, 0, }, 1, },

  { "initial step size",
    { "initial", "step", "size", nullptr, },
    { 1, 0, 0, 0, }, 1, },

  { "maximum order",
    { "maximum", "order", nullptr, nullptr, },
    { 1, 1, 0, 0, }, 2, },

  { "maximum step size",
    { "maximum", "step", "size", nullptr, },
    { 1, 1, 0, 0, }, 2, },

  { "step limit",
    { "step", "limit", nullptr, nullptr, },
    { 1, 0, 0, 0, }, 1, },
};

static void
print_DASRT_options (std::ostream& os)
{
  std::ostringstream buf;

  os << "\n"
     << "Options for DASRT include:\n\n"
     << "  keyword                                             value\n"
     << "  -------                                             -----\n";

  DASRT_options_struct *list = DASRT_options_table;

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[0].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    Array<double> val = dasrt_opts.absolute_tolerance ();

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

    Array<double> val = dasrt_opts.relative_tolerance ();

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

    double val = dasrt_opts.initial_step_size ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[3].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    int val = dasrt_opts.maximum_order ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[4].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    double val = dasrt_opts.maximum_step_size ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[5].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    int val = dasrt_opts.step_limit ();

    os << val << "\n";
  }

  os << "\n";
}

static void
set_DASRT_options (const std::string& keyword, const octave_value& val)
{
  DASRT_options_struct *list = DASRT_options_table;

  if (keyword_almost_match (list[0].kw_tok, list[0].min_len,
           keyword, list[0].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> tmp = val.vector_value ();

      dasrt_opts.set_absolute_tolerance (tmp);
    }
  else if (keyword_almost_match (list[1].kw_tok, list[1].min_len,
           keyword, list[1].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> tmp = val.vector_value ();

      dasrt_opts.set_relative_tolerance (tmp);
    }
  else if (keyword_almost_match (list[2].kw_tok, list[2].min_len,
           keyword, list[2].min_toks_to_match, MAX_TOKENS))
    {
      double tmp = val.double_value ();

      dasrt_opts.set_initial_step_size (tmp);
    }
  else if (keyword_almost_match (list[3].kw_tok, list[3].min_len,
           keyword, list[3].min_toks_to_match, MAX_TOKENS))
    {
      int tmp = val.int_value ();

      dasrt_opts.set_maximum_order (tmp);
    }
  else if (keyword_almost_match (list[4].kw_tok, list[4].min_len,
           keyword, list[4].min_toks_to_match, MAX_TOKENS))
    {
      double tmp = val.double_value ();

      dasrt_opts.set_maximum_step_size (tmp);
    }
  else if (keyword_almost_match (list[5].kw_tok, list[5].min_len,
           keyword, list[5].min_toks_to_match, MAX_TOKENS))
    {
      int tmp = val.int_value ();

      dasrt_opts.set_step_limit (tmp);
    }
  else
    {
      warning ("dasrt_options: no match for `%s'", keyword.c_str ());
    }
}

static octave_value_list
show_DASRT_options (const std::string& keyword)
{
  octave_value retval;

  DASRT_options_struct *list = DASRT_options_table;

  if (keyword_almost_match (list[0].kw_tok, list[0].min_len,
           keyword, list[0].min_toks_to_match, MAX_TOKENS))
    {
      Array<double> val = dasrt_opts.absolute_tolerance ();

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
      Array<double> val = dasrt_opts.relative_tolerance ();

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
      double val = dasrt_opts.initial_step_size ();

      retval = val;
    }
  else if (keyword_almost_match (list[3].kw_tok, list[3].min_len,
           keyword, list[3].min_toks_to_match, MAX_TOKENS))
    {
      int val = dasrt_opts.maximum_order ();

      retval = static_cast<double> (val);
    }
  else if (keyword_almost_match (list[4].kw_tok, list[4].min_len,
           keyword, list[4].min_toks_to_match, MAX_TOKENS))
    {
      double val = dasrt_opts.maximum_step_size ();

      retval = val;
    }
  else if (keyword_almost_match (list[5].kw_tok, list[5].min_len,
           keyword, list[5].min_toks_to_match, MAX_TOKENS))
    {
      int val = dasrt_opts.step_limit ();

      retval = static_cast<double> (val);
    }
  else
    {
      warning ("dasrt_options: no match for `%s'", keyword.c_str ());
    }

  return retval;
}

DEFUN (dasrt_options, args, ,
       doc: /* -*- texinfo -*-
@deftypefn  {} {} dasrt_options ()
@deftypefnx {} {val =} dasrt_options (@var{opt})
@deftypefnx {} {} dasrt_options (@var{opt}, @var{val})
Query or set options for the function @code{dasrt}.

When called with no arguments, the names of all available options and
their current values are displayed.

Given one argument, return the value of the option @var{opt}.

When called with two arguments, @code{dasrt_options} sets the option
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
  abs (local error in x(i)) <= ...
      rtol(i) * abs (Y(i)) + atol(i)
@end group
@end example

@item @qcode{"initial step size"}
Differential-algebraic problems may occasionally suffer from severe
scaling difficulties on the first step.  If you know a great deal
about the scaling of your problem, you can help to alleviate this
problem by specifying an initial stepsize.

@item @qcode{"maximum order"}
Restrict the maximum order of the solution method.  This option must
be between 1 and 5, inclusive.

@item @qcode{"maximum step size"}
Setting the maximum stepsize will avoid passing over very large
regions.

@item @qcode{"step limit"}
Maximum number of integration steps to attempt on a single call to the
underlying Fortran code.
@end table
@end deftypefn */)
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin > 2)
    print_usage ();

  if (nargin == 0)
    {
      print_DASRT_options (octave_stdout);
    }
  else
    {
      std::string keyword = args(0).xstring_value ("dasrt_options: expecting keyword as first argument");

      if (nargin == 1)
        retval = show_DASRT_options (keyword);
      else
        set_DASRT_options (keyword, args(1));
    }

  return retval;
}
