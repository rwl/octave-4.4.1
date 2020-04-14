// DO NOT EDIT!
// Generated automatically from /home/jwe/src/octave-stable/liboctave/numeric/Quad-opts.in.

// This file should not include config.h.  It is only included in other
// C++ source files that should have included config.h before including
// this file.

#include <iomanip>
#include <iostream>

#include "Quad-opts.h"

#include "defun.h"
#include "pr-output.h"

#include "ovl.h"
#include "utils.h"
#include "pager.h"

static Quad_options quad_opts;

#define MAX_TOKENS 4

struct Quad_options_struct
{
  const char *keyword;
  const char *kw_tok[MAX_TOKENS + 1];
  int min_len[MAX_TOKENS + 1];
  int min_toks_to_match;
};

#define NUM_OPTIONS 4

static Quad_options_struct Quad_options_table [] =
{
  { "absolute tolerance",
    { "absolute", "tolerance", nullptr, nullptr, nullptr, },
    { 1, 0, 0, 0, 0, }, 1, },

  { "relative tolerance",
    { "relative", "tolerance", nullptr, nullptr, nullptr, },
    { 1, 0, 0, 0, 0, }, 1, },

  { "single precision absolute tolerance",
    { "single", "precision", "absolute", "tolerance", nullptr, },
    { 1, 1, 1, 0, 0, }, 3, },

  { "single precision relative tolerance",
    { "single", "precision", "relative", "tolerance", nullptr, },
    { 1, 1, 1, 0, 0, }, 3, },
};

static void
print_Quad_options (std::ostream& os)
{
  std::ostringstream buf;

  os << "\n"
     << "Options for Quad include:\n\n"
     << "  keyword                                             value\n"
     << "  -------                                             -----\n";

  Quad_options_struct *list = Quad_options_table;

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[0].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    double val = quad_opts.absolute_tolerance ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[1].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    double val = quad_opts.relative_tolerance ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[2].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    float val = quad_opts.single_precision_absolute_tolerance ();

    os << val << "\n";
  }

  {
    os << "  "
        << std::setiosflags (std::ios::left) << std::setw (50)
        << list[3].keyword
        << std::resetiosflags (std::ios::left)
        << "  ";

    float val = quad_opts.single_precision_relative_tolerance ();

    os << val << "\n";
  }

  os << "\n";
}

static void
set_Quad_options (const std::string& keyword, const octave_value& val)
{
  Quad_options_struct *list = Quad_options_table;

  if (keyword_almost_match (list[0].kw_tok, list[0].min_len,
           keyword, list[0].min_toks_to_match, MAX_TOKENS))
    {
      double tmp = val.double_value ();

      quad_opts.set_absolute_tolerance (tmp);
    }
  else if (keyword_almost_match (list[1].kw_tok, list[1].min_len,
           keyword, list[1].min_toks_to_match, MAX_TOKENS))
    {
      double tmp = val.double_value ();

      quad_opts.set_relative_tolerance (tmp);
    }
  else if (keyword_almost_match (list[2].kw_tok, list[2].min_len,
           keyword, list[2].min_toks_to_match, MAX_TOKENS))
    {
      float tmp = val.float_value ();

      quad_opts.set_single_precision_absolute_tolerance (tmp);
    }
  else if (keyword_almost_match (list[3].kw_tok, list[3].min_len,
           keyword, list[3].min_toks_to_match, MAX_TOKENS))
    {
      float tmp = val.float_value ();

      quad_opts.set_single_precision_relative_tolerance (tmp);
    }
  else
    {
      warning ("quad_options: no match for `%s'", keyword.c_str ());
    }
}

static octave_value_list
show_Quad_options (const std::string& keyword)
{
  octave_value retval;

  Quad_options_struct *list = Quad_options_table;

  if (keyword_almost_match (list[0].kw_tok, list[0].min_len,
           keyword, list[0].min_toks_to_match, MAX_TOKENS))
    {
      double val = quad_opts.absolute_tolerance ();

      retval = val;
    }
  else if (keyword_almost_match (list[1].kw_tok, list[1].min_len,
           keyword, list[1].min_toks_to_match, MAX_TOKENS))
    {
      double val = quad_opts.relative_tolerance ();

      retval = val;
    }
  else if (keyword_almost_match (list[2].kw_tok, list[2].min_len,
           keyword, list[2].min_toks_to_match, MAX_TOKENS))
    {
      float val = quad_opts.single_precision_absolute_tolerance ();

      retval = val;
    }
  else if (keyword_almost_match (list[3].kw_tok, list[3].min_len,
           keyword, list[3].min_toks_to_match, MAX_TOKENS))
    {
      float val = quad_opts.single_precision_relative_tolerance ();

      retval = val;
    }
  else
    {
      warning ("quad_options: no match for `%s'", keyword.c_str ());
    }

  return retval;
}

DEFUN (quad_options, args, ,
       doc: /* -*- texinfo -*-
@deftypefn  {} {} quad_options ()
@deftypefnx {} {val =} quad_options (@var{opt})
@deftypefnx {} {} quad_options (@var{opt}, @var{val})
Query or set options for the function @code{quad}.

When called with no arguments, the names of all available options and
their current values are displayed.

Given one argument, return the value of the option @var{opt}.

When called with two arguments, @code{quad_options} sets the option
@var{opt} to value @var{val}.

Options include

@table @asis
@item @qcode{"absolute tolerance"}
Absolute tolerance; may be zero for pure relative error test.

@item @qcode{"relative tolerance"}
Non-negative relative tolerance.  If the absolute tolerance is zero,
the relative tolerance must be greater than or equal to
@w{@code{max (50*eps, 0.5e-28)}}.

@item @qcode{"single precision absolute tolerance"}
Absolute tolerance for single precision; may be zero for pure relative
error test.

@item @qcode{"single precision relative tolerance"}
Non-negative relative tolerance for single precision.  If the absolute
tolerance is zero, the relative tolerance must be greater than or equal to
@w{@code{max (50*eps, 0.5e-28)}}.
@end table
@end deftypefn */)
{
  octave_value_list retval;

  int nargin = args.length ();

  if (nargin > 2)
    print_usage ();

  if (nargin == 0)
    {
      print_Quad_options (octave_stdout);
    }
  else
    {
      std::string keyword = args(0).xstring_value ("quad_options: expecting keyword as first argument");

      if (nargin == 1)
        retval = show_Quad_options (keyword);
      else
        set_Quad_options (keyword, args(1));
    }

  return retval;
}
