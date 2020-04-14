/*

Copyright (C) 2004-2018 David Bateman

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

In addition to the terms of the GPL, you are permitted to link
this program with any Open Source program, as defined by the
Open Source Initiative (www.opensource.org)

*/

#if defined (HAVE_CONFIG_H)
#  include "config.h"
#endif

#include <istream>
#include <iostream>
#include <sstream>
#include <vector>

#include "oct-locbuf.h"

#include "call-stack.h"
#include "defun.h"
#include "error.h"
#include "errwarn.h"
#include "interpreter-private.h"
#include "interpreter.h"
#include "oct-hdf5.h"
#include "oct-map.h"
#include "ov-base.h"
#include "ov-fcn-inline.h"
#include "ov-usr-fcn.h"
#include "parse.h"
#include "pr-output.h"
#include "variables.h"

#include "byte-swap.h"
#include "ls-ascii-helper.h"
#include "ls-oct-text.h"
#include "ls-hdf5.h"
#include "ls-utils.h"


DEFINE_OV_TYPEID_FUNCTIONS_AND_DATA (octave_fcn_inline,
                                     "inline function",
                                     "function_handle");

octave_fcn_inline::octave_fcn_inline (const std::string& f,
                                      const string_vector& a,
                                      const std::string& n)
  : octave_fcn_handle (n), iftext (f), ifargs (a)
{
  // Form a string representing the function.

  std::ostringstream buf;

  buf << "@(";

  for (int i = 0; i < ifargs.numel (); i++)
    {
      if (i > 0)
        buf << ", ";

      buf << ifargs(i);
    }

  buf << ") " << iftext;

  int parse_status;
  octave_value anon_fcn_handle = octave::eval_string (buf.str (), true, parse_status);

  if (parse_status == 0)
    {
      octave_fcn_handle *fh = anon_fcn_handle.fcn_handle_value ();

      if (fh)
        {
          fcn = fh->fcn_val ();

          octave_user_function *uf = fcn.user_function_value ();

          if (uf)
            {
              octave::call_stack& cs
                = octave::__get_call_stack__ ("octave_fcn_inline");

              octave_function *curr_fcn = cs.current ();

              if (curr_fcn)
                {
                  octave::symbol_scope parent_scope
                    = curr_fcn->parent_fcn_scope ();

                  if (! parent_scope)
                    parent_scope = curr_fcn->scope ();

                  uf->stash_parent_fcn_scope (parent_scope);
                }
            }
        }
    }

  if (fcn.is_undefined ())
    error ("inline: unable to define function");
}

// This function is supplied to allow a Matlab style class structure
// to be returned..
octave_map
octave_fcn_inline::map_value (void) const
{
  octave_scalar_map m;

  m.assign ("version", 1.0);
  m.assign ("isEmpty", 0.0);
  m.assign ("expr", fcn_text ());

  string_vector args = fcn_arg_names ();

  m.assign ("numArgs", args.numel ());
  m.assign ("args", args);

  std::ostringstream buf;

  for (int i = 0; i < args.numel (); i++)
    buf << args(i) << " = INLINE_INPUTS_{" << i + 1 << "}; ";

  m.assign ("inputExpr", buf.str ());

  return m;
}

bool
octave_fcn_inline::save_ascii (std::ostream& os)
{
  os << "# nargs: " <<  ifargs.numel () << "\n";
  for (int i = 0; i < ifargs.numel (); i++)
    os << ifargs(i) << "\n";
  if (nm.length () < 1)
    // Write an invalid value to flag empty fcn handle name.
    os << "0\n";
  else
    os << nm << "\n";
  os << iftext << "\n";
  return true;
}

bool
octave_fcn_inline::load_ascii (std::istream& is)
{
  int nargs;
  if (extract_keyword (is, "nargs", nargs, true))
    {
      ifargs.resize (nargs);
      for (int i = 0; i < nargs; i++)
        is >> ifargs(i);
      is >> nm;
      if (nm == "0")
        nm = "";

      skip_preceeding_newline (is);

      std::string buf;

      if (is)
        {

          // Get a line of text whitespace characters included,
          // leaving newline in the stream.
          buf = read_until_newline (is, true);
        }

      iftext = buf;

      octave_fcn_inline tmp (iftext, ifargs, nm);
      fcn = tmp.fcn;

      return true;
    }
  else
    return false;
}

bool
octave_fcn_inline::save_binary (std::ostream& os, bool&)
{
  int32_t tmp = ifargs.numel ();
  os.write (reinterpret_cast<char *> (&tmp), 4);
  for (int i = 0; i < ifargs.numel (); i++)
    {
      tmp = ifargs(i).length ();
      os.write (reinterpret_cast<char *> (&tmp), 4);
      os.write (ifargs(i).c_str (), ifargs(i).length ());
    }
  tmp = nm.length ();
  os.write (reinterpret_cast<char *> (&tmp), 4);
  os.write (nm.c_str (), nm.length ());
  tmp = iftext.length ();
  os.write (reinterpret_cast<char *> (&tmp), 4);
  os.write (iftext.c_str (), iftext.length ());
  return true;
}

bool
octave_fcn_inline::load_binary (std::istream& is, bool swap,
                                octave::mach_info::float_format)
{
  int32_t nargs;
  if (! is.read (reinterpret_cast<char *> (&nargs), 4))
    return false;
  if (swap)
    swap_bytes<4> (&nargs);

  if (nargs < 1)
    return false;
  else
    {
      int32_t tmp;
      ifargs.resize (nargs);
      for (int i = 0; i < nargs; i++)
        {
          if (! is.read (reinterpret_cast<char *> (&tmp), 4))
            return false;
          if (swap)
            swap_bytes<4> (&tmp);

          OCTAVE_LOCAL_BUFFER (char, ctmp, tmp+1);
          is.read (ctmp, tmp);
          ifargs(i) = std::string (ctmp);

          if (! is)
            return false;
        }

      if (! is.read (reinterpret_cast<char *> (&tmp), 4))
        return false;
      if (swap)
        swap_bytes<4> (&tmp);

      OCTAVE_LOCAL_BUFFER (char, ctmp1, tmp+1);
      is.read (ctmp1, tmp);
      nm = std::string (ctmp1);

      if (! is)
        return false;

      if (! is.read (reinterpret_cast<char *> (&tmp), 4))
        return false;
      if (swap)
        swap_bytes<4> (&tmp);

      OCTAVE_LOCAL_BUFFER (char, ctmp2, tmp+1);
      is.read (ctmp2, tmp);
      iftext = std::string (ctmp2);

      if (! is)
        return false;

      octave_fcn_inline ftmp (iftext, ifargs, nm);
      fcn = ftmp.fcn;
    }
  return true;
}

bool
octave_fcn_inline::save_hdf5 (octave_hdf5_id loc_id, const char *name,
                              bool /* save_as_floats */)
{
  bool retval = false;

#if defined (HAVE_HDF5)

  hid_t group_hid = -1;

#if defined (HAVE_HDF5_18)
  group_hid = H5Gcreate (loc_id, name, octave_H5P_DEFAULT, octave_H5P_DEFAULT,
                         octave_H5P_DEFAULT);
#else
  group_hid = H5Gcreate (loc_id, name, 0);
#endif
  if (group_hid < 0) return false;

  size_t len = 0;
  for (int i = 0; i < ifargs.numel (); i++)
    if (len < ifargs(i).length ())
      len = ifargs(i).length ();

  hid_t space_hid, data_hid, type_hid;
  space_hid = data_hid = type_hid = -1;

  // FIXME: Is there a better way of saving string vectors,
  //        than a null padded matrix?

  OCTAVE_LOCAL_BUFFER (hsize_t, hdims, 2);

  // Octave uses column-major, while HDF5 uses row-major ordering
  hdims[1] = ifargs.numel ();
  hdims[0] = len + 1;

  space_hid = H5Screate_simple (2, hdims, nullptr);
  if (space_hid < 0)
    {
      H5Gclose (group_hid);
      return false;
    }
#if defined (HAVE_HDF5_18)
  data_hid = H5Dcreate (group_hid, "args", H5T_NATIVE_CHAR, space_hid,
                        octave_H5P_DEFAULT, octave_H5P_DEFAULT, octave_H5P_DEFAULT);
#else
  data_hid = H5Dcreate (group_hid, "args", H5T_NATIVE_CHAR, space_hid,
                        octave_H5P_DEFAULT);
#endif
  if (data_hid < 0)
    {
      H5Sclose (space_hid);
      H5Gclose (group_hid);
      return false;
    }

  OCTAVE_LOCAL_BUFFER (char, s, ifargs.numel () * (len + 1));

  // Save the args as a null teminated list
  for (int i = 0; i < ifargs.numel (); i++)
    {
      const char *cptr = ifargs(i).c_str ();
      for (size_t j = 0; j < ifargs(i).length (); j++)
        s[i*(len+1)+j] = *cptr++;
      s[ifargs(i).length ()] = '\0';
    }

  retval = H5Dwrite (data_hid, H5T_NATIVE_CHAR, octave_H5S_ALL, octave_H5S_ALL,
                     octave_H5P_DEFAULT, s) >= 0;

  H5Dclose (data_hid);
  H5Sclose (space_hid);

  if (! retval)
    {
      H5Gclose (group_hid);
      return false;
    }

  // attach the type of the variable
  type_hid = H5Tcopy (H5T_C_S1);
  H5Tset_size (type_hid, nm.length () + 1);
  if (type_hid < 0)
    {
      H5Gclose (group_hid);
      return false;
    }

  hdims[0] = 0;
  space_hid = H5Screate_simple (0 , hdims, nullptr);
  if (space_hid < 0)
    {
      H5Tclose (type_hid);
      H5Gclose (group_hid);
      return false;
    }
#if defined (HAVE_HDF5_18)
  data_hid = H5Dcreate (group_hid, "nm",  type_hid, space_hid,
                        octave_H5P_DEFAULT, octave_H5P_DEFAULT, octave_H5P_DEFAULT);
#else
  data_hid = H5Dcreate (group_hid, "nm",  type_hid, space_hid,
                        octave_H5P_DEFAULT);
#endif
  if (data_hid < 0
      || H5Dwrite (data_hid, type_hid, octave_H5S_ALL, octave_H5S_ALL,
                   octave_H5P_DEFAULT, nm.c_str ()) < 0)
    {
      H5Sclose (space_hid);
      H5Tclose (type_hid);
      H5Gclose (group_hid);
      return false;
    }
  H5Dclose (data_hid);

  // attach the type of the variable
  H5Tset_size (type_hid, iftext.length () + 1);
  if (type_hid < 0)
    {
      H5Gclose (group_hid);
      return false;
    }

#if defined (HAVE_HDF5_18)
  data_hid = H5Dcreate (group_hid, "iftext",  type_hid, space_hid,
                        octave_H5P_DEFAULT, octave_H5P_DEFAULT, octave_H5P_DEFAULT);
#else
  data_hid = H5Dcreate (group_hid, "iftext",  type_hid, space_hid,
                        octave_H5P_DEFAULT);
#endif
  if (data_hid < 0
      || H5Dwrite (data_hid, type_hid, octave_H5S_ALL, octave_H5S_ALL,
                   octave_H5P_DEFAULT, iftext.c_str ()) < 0)
    {
      H5Sclose (space_hid);
      H5Tclose (type_hid);
      H5Gclose (group_hid);
      return false;
    }

  H5Dclose (data_hid);
  H5Sclose (space_hid);
  H5Tclose (type_hid);
  H5Gclose (group_hid);

#else
  octave_unused_parameter (loc_id);
  octave_unused_parameter (name);

  warn_save ("hdf5");
#endif

  return retval;
}

bool
octave_fcn_inline::load_hdf5 (octave_hdf5_id loc_id, const char *name)
{
#if defined (HAVE_HDF5)

  hid_t group_hid, data_hid, space_hid, type_hid, type_class_hid, st_id;
  hsize_t rank;
  int slen;

#if defined (HAVE_HDF5_18)
  group_hid = H5Gopen (loc_id, name, octave_H5P_DEFAULT);
#else
  group_hid = H5Gopen (loc_id, name);
#endif
  if (group_hid < 0) return false;

#if defined (HAVE_HDF5_18)
  data_hid = H5Dopen (group_hid, "args", octave_H5P_DEFAULT);
#else
  data_hid = H5Dopen (group_hid, "args");
#endif
  space_hid = H5Dget_space (data_hid);
  rank = H5Sget_simple_extent_ndims (space_hid);

  if (rank != 2)
    {
      H5Dclose (data_hid);
      H5Sclose (space_hid);
      H5Gclose (group_hid);
      return false;
    }

  OCTAVE_LOCAL_BUFFER (hsize_t, hdims, rank);
  OCTAVE_LOCAL_BUFFER (hsize_t, maxdims, rank);

  H5Sget_simple_extent_dims (space_hid, hdims, maxdims);

  ifargs.resize (hdims[1]);

  OCTAVE_LOCAL_BUFFER (char, s1, hdims[0] * hdims[1]);

  if (H5Dread (data_hid, H5T_NATIVE_UCHAR, octave_H5S_ALL, octave_H5S_ALL,
               octave_H5P_DEFAULT, s1) < 0)
    {
      H5Dclose (data_hid);
      H5Sclose (space_hid);
      H5Gclose (group_hid);
      return false;
    }

  H5Dclose (data_hid);
  H5Sclose (space_hid);

  for (size_t i = 0; i < hdims[1]; i++)
    ifargs(i) = std::string (s1 + i*hdims[0]);

#if defined (HAVE_HDF5_18)
  data_hid = H5Dopen (group_hid, "nm", octave_H5P_DEFAULT);
#else
  data_hid = H5Dopen (group_hid, "nm");
#endif

  if (data_hid < 0)
    {
      H5Gclose (group_hid);
      return false;
    }

  type_hid = H5Dget_type (data_hid);
  type_class_hid = H5Tget_class (type_hid);

  if (type_class_hid != H5T_STRING)
    {
      H5Tclose (type_hid);
      H5Dclose (data_hid);
      H5Gclose (group_hid);
      return false;
    }

  space_hid = H5Dget_space (data_hid);
  rank = H5Sget_simple_extent_ndims (space_hid);

  if (rank != 0)
    {
      H5Sclose (space_hid);
      H5Tclose (type_hid);
      H5Dclose (data_hid);
      H5Gclose (group_hid);
      return false;
    }

  slen = H5Tget_size (type_hid);
  if (slen < 0)
    {
      H5Sclose (space_hid);
      H5Tclose (type_hid);
      H5Dclose (data_hid);
      H5Gclose (group_hid);
      return false;
    }

  OCTAVE_LOCAL_BUFFER (char, nm_tmp, slen);

  // create datatype for (null-terminated) string to read into:
  st_id = H5Tcopy (H5T_C_S1);
  H5Tset_size (st_id, slen);

  if (H5Dread (data_hid, st_id, octave_H5S_ALL, octave_H5S_ALL,
               octave_H5P_DEFAULT, nm_tmp) < 0)
    {
      H5Sclose (space_hid);
      H5Tclose (type_hid);
      H5Gclose (group_hid);
      return false;
    }
  H5Tclose (st_id);
  H5Dclose (data_hid);
  nm = nm_tmp;

#if defined (HAVE_HDF5_18)
  data_hid = H5Dopen (group_hid, "iftext", octave_H5P_DEFAULT);
#else
  data_hid = H5Dopen (group_hid, "iftext");
#endif

  if (data_hid < 0)
    {
      H5Gclose (group_hid);
      return false;
    }

  type_hid = H5Dget_type (data_hid);
  type_class_hid = H5Tget_class (type_hid);

  if (type_class_hid != H5T_STRING)
    {
      H5Tclose (type_hid);
      H5Dclose (data_hid);
      H5Gclose (group_hid);
      return false;
    }

  space_hid = H5Dget_space (data_hid);
  rank = H5Sget_simple_extent_ndims (space_hid);

  if (rank != 0)
    {
      H5Sclose (space_hid);
      H5Tclose (type_hid);
      H5Dclose (data_hid);
      H5Gclose (group_hid);
      return false;
    }

  slen = H5Tget_size (type_hid);
  if (slen < 0)
    {
      H5Sclose (space_hid);
      H5Tclose (type_hid);
      H5Dclose (data_hid);
      H5Gclose (group_hid);
      return false;
    }

  OCTAVE_LOCAL_BUFFER (char, iftext_tmp, slen);

  // create datatype for (null-terminated) string to read into:
  st_id = H5Tcopy (H5T_C_S1);
  H5Tset_size (st_id, slen);

  if (H5Dread (data_hid, st_id, octave_H5S_ALL, octave_H5S_ALL,
               octave_H5P_DEFAULT, iftext_tmp) < 0)
    {
      H5Sclose (space_hid);
      H5Tclose (type_hid);
      H5Gclose (group_hid);
      return false;
    }
  H5Tclose (st_id);
  H5Dclose (data_hid);
  iftext = iftext_tmp;

  octave_fcn_inline ftmp (iftext, ifargs, nm);
  fcn = ftmp.fcn;

  return true;

#else
  octave_unused_parameter (loc_id);
  octave_unused_parameter (name);

  warn_load ("hdf5");

  return false;
#endif
}

void
octave_fcn_inline::print (std::ostream& os, bool pr_as_read_syntax)
{
  print_raw (os, pr_as_read_syntax);
  newline (os);
}

void
octave_fcn_inline::print_raw (std::ostream& os, bool pr_as_read_syntax) const
{
  std::ostringstream buf;

  if (nm.empty ())
    buf << "f(";
  else
    buf << nm << '(';

  for (int i = 0; i < ifargs.numel (); i++)
    {
      if (i)
        buf << ", ";

      buf << ifargs(i);
    }

  buf << ") = " << iftext;

  octave_print_internal (os, buf.str (), pr_as_read_syntax,
                         current_print_indent_level ());
}

octave_value
octave_fcn_inline::convert_to_str_internal (bool, bool, char type) const
{
  return octave_value (fcn_text (), type);
}

DEFUNX ("inline", Finline, args, ,
        doc: /* -*- texinfo -*-
@deftypefn  {} {} inline (@var{str})
@deftypefnx {} {} inline (@var{str}, @var{arg1}, @dots{})
@deftypefnx {} {} inline (@var{str}, @var{n})
Create an inline function from the character string @var{str}.

If called with a single argument, the arguments of the generated function
are extracted from the function itself.  The generated function arguments
will then be in alphabetical order.  It should be noted that i and j are
ignored as arguments due to the ambiguity between their use as a variable or
their use as an built-in constant.  All arguments followed by a parenthesis
are considered to be functions.  If no arguments are found, a function
taking a single argument named @code{x} will be created.

If the second and subsequent arguments are character strings, they are the
names of the arguments of the function.

If the second argument is an integer @var{n}, the arguments are
@qcode{"x"}, @qcode{"P1"}, @dots{}, @qcode{"P@var{N}"}.

Programming Note: The use of @code{inline} is discouraged and it may be
removed from a future version of Octave.  The preferred way to create
functions from strings is through the use of anonymous functions
(@pxref{Anonymous Functions}) or @code{str2func}.
@seealso{argnames, formula, vectorize, str2func}
@end deftypefn */)
{
  int nargin = args.length ();

  if (nargin == 0)
    print_usage ();

  std::string fun = args(0).xstring_value ("inline: STR argument must be a string");

  string_vector fargs;

  if (nargin == 1)
    {
      bool is_arg = false;
      bool in_string = false;
      std::string tmp_arg;
      size_t i = 0;
      size_t fun_length = fun.length ();

      while (i < fun_length)
        {
          bool terminate_arg = false;
          char c = fun[i++];

          if (in_string)
            {
              if (c == '\'' || c == '\"')
                in_string = false;
            }
          else if (c == '\'' || c == '\"')
            {
              in_string = true;
              if (is_arg)
                terminate_arg = true;
            }
          else if (! isalpha (c) && c != '_')
            if (! is_arg)
              continue;
            else if (isdigit (c))
              tmp_arg.append (1, c);
            else
              {
                // Before we do anything remove trailing whitespaces.
                while (i < fun_length && isspace (c))
                  c = fun[i++];

                // Do we have a variable or a function?
                if (c != '(')
                  terminate_arg = true;
                else
                  {
                    tmp_arg = "";
                    is_arg = false;
                  }
              }
          else if (! is_arg)
            {
              if (c == 'e' || c == 'E')
                {
                  // possible number in exponent form, not arg
                  if (isdigit (fun[i]) || fun[i] == '-' || fun[i] == '+')
                    continue;
                }
              is_arg = true;
              tmp_arg.append (1, c);
            }
          else
            {
              tmp_arg.append (1, c);
            }

          if (terminate_arg || (i == fun_length && is_arg))
            {
              bool have_arg = false;

              for (int j = 0; j < fargs.numel (); j++)
                if (tmp_arg == fargs (j))
                  {
                    have_arg = true;
                    break;
                  }

              if (! have_arg && tmp_arg != "i" && tmp_arg != "j"
                  && tmp_arg != "NaN" && tmp_arg != "nan"
                  && tmp_arg != "Inf" && tmp_arg != "inf"
                  && tmp_arg != "NA" && tmp_arg != "pi"
                  && tmp_arg != "e" && tmp_arg != "eps")
                fargs.append (tmp_arg);

              tmp_arg = "";
              is_arg = false;
            }
        }

      // Sort the arguments into ASCII order.
      fargs.sort ();

      if (fargs.isempty ())
        fargs.append (std::string ("x"));

    }
  else if (nargin == 2 && args(1).isnumeric ())
    {
      if (! args(1).is_scalar_type ())
        error ("inline: N must be an integer");

      int n = args(1).int_value ("inline: N must be an integer");

      if (n < 0)
        error ("inline: N must be a positive integer or zero");

      fargs.resize (n+1);

      fargs(0) = "x";

      for (int i = 1; i < n+1; i++)
        {
          std::ostringstream buf;
          buf << 'P' << i;
          fargs(i) = buf.str ();
        }
    }
  else
    {
      fargs.resize (nargin - 1);

      for (int i = 1; i < nargin; i++)
        {
          std::string s = args(i).xstring_value ("inline: additional arguments must be strings");
          fargs(i-1) = s;
        }
    }

  return ovl (new octave_fcn_inline (fun, fargs));
}

/*
%!shared fn
%! fn = inline ("x.^2 + 1");
%!assert (feval (fn, 6), 37)
%!assert (fn (6), 37)
%!assert (feval (inline ("sum (x(:))"), [1 2; 3 4]), 10)
%!assert (feval (inline ("sqrt (x^2 + y^2)", "x", "y"), 3, 4), 5)
%!assert (feval (inline ("exp (P1*x) + P2", 3), 3, 4, 5), exp(3*4) + 5)

## Test input validation
%!error inline ()
%!error <STR argument must be a string> inline (1)
%!error <N must be an integer> inline ("2", ones (2,2))
%!error <N must be a positive integer> inline ("2", -1)
%!error <additional arguments must be strings> inline ("2", "x", -1, "y")
*/

DEFUN (formula, args, ,
       doc: /* -*- texinfo -*-
@deftypefn {} {} formula (@var{fun})
Return a character string representing the inline function @var{fun}.

Note that @code{char (@var{fun})} is equivalent to
@code{formula (@var{fun})}.
@seealso{char, argnames, inline, vectorize}
@end deftypefn */)
{
  if (args.length () != 1)
    print_usage ();

  octave_fcn_inline *fn = args(0).fcn_inline_value (true);

  if (! fn)
    error ("formula: FUN must be an inline function");

  return ovl (fn->fcn_text ());
}

/*
%!assert (formula (fn), "x.^2 + 1")
%!assert (formula (fn), char (fn))

## Test input validation
%!error formula ()
%!error formula (1, 2)
%!error <FUN must be an inline function> formula (1)
*/

DEFUN (argnames, args, ,
       doc: /* -*- texinfo -*-
@deftypefn {} {} argnames (@var{fun})
Return a cell array of character strings containing the names of the
arguments of the inline function @var{fun}.
@seealso{inline, formula, vectorize}
@end deftypefn */)
{
  if (args.length () != 1)
    print_usage ();

  octave_fcn_inline *fn = args(0).fcn_inline_value (true);

  if (! fn)
    error ("argnames: FUN must be an inline function");

  string_vector t1 = fn->fcn_arg_names ();

  Cell t2 (dim_vector (t1.numel (), 1));

  for (int i = 0; i < t1.numel (); i++)
    t2(i) = t1(i);

  return ovl (t2);
}

/*
%!assert (argnames (fn), {"x"})
%!assert (argnames (inline ("1e-3*y + 2e4*z")), {"y"; "z"})
%!assert (argnames (inline ("2", 2)), {"x"; "P1"; "P2"})

## Test input validation
%!error argnames ()
%!error argnames (1, 2)
%!error <FUN must be an inline function> argnames (1)
*/

DEFUN (vectorize, args, ,
       doc: /* -*- texinfo -*-
@deftypefn {} {} vectorize (@var{fun})
Create a vectorized version of the inline function @var{fun} by replacing
all occurrences of @code{*}, @code{/}, etc., with @code{.*}, @code{./}, etc.

This may be useful, for example, when using inline functions with numerical
integration or optimization where a vector-valued function is expected.

@example
@group
fcn = vectorize (inline ("x^2 - 1"))
   @result{} fcn = f(x) = x.^2 - 1
quadv (fcn, 0, 3)
   @result{} 6
@end group
@end example
@seealso{inline, formula, argnames}
@end deftypefn */)
{
  if (args.length () != 1)
    print_usage ();

  std::string old_func;
  octave_fcn_inline *old = nullptr;
  bool func_is_string = true;

  if (args(0).is_string ())
    old_func = args(0).string_value ();
  else
    {
      func_is_string = false;

      old = args(0).fcn_inline_value (true);
      if (! old)
        error ("vectorize: FUN must be a string or inline function");

      old_func = old->fcn_text ();
    }

  size_t len = old_func.length ();
  std::string new_func;
  new_func.reserve (len + 10);

  size_t i = 0;
  while (i < len)
    {
      char t1 = old_func[i];

      if (t1 == '*' || t1 == '/' || t1 == '\\' || t1 == '^')
        {
          if (i && old_func[i-1] != '.')
            new_func.push_back ('.');

          // Special case for ** operator.
          if (t1 == '*' && i < (len - 1) && old_func[i+1] == '*')
            {
              new_func.push_back ('*');
              i++;
            }
        }
      new_func.push_back (t1);
      i++;
    }

  if (func_is_string)
    return ovl (new_func);
  else
    return ovl (new octave_fcn_inline (new_func, old->fcn_arg_names ()));
}

/*
%!assert (char (vectorize (fn)), "x.^2 + 1")
%!assert (char (vectorize (inline ("1e-3*y + 2e4*z"))), "1e-3.*y + 2e4.*z")
%!assert (char (vectorize (inline ("2**x^5"))), "2.**x.^5")
%!assert (vectorize ("x.^2 + 1"), "x.^2 + 1")
%!assert (vectorize ("1e-3*y + 2e4*z"), "1e-3.*y + 2e4.*z")
%!assert (vectorize ("2**x^5"), "2.**x.^5")

## Test input validation
%!error vectorize ()
%!error vectorize (1, 2)
%!error <FUN must be a string or inline function> vectorize (1)
*/
