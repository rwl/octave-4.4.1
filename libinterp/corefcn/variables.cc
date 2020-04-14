/*

Copyright (C) 1993-2018 John W. Eaton
Copyright (C) 2009-2010 VZLU Prague

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

#include <cstdio>
#include <cstring>

#include <iomanip>
#include <set>
#include <string>

#include "file-stat.h"
#include "oct-env.h"
#include "file-ops.h"
#include "glob-match.h"
#include "lo-regexp.h"
#include "str-vec.h"

#include "call-stack.h"
#include "defaults.h"
#include "Cell.h"
#include "defun.h"
#include "dirfns.h"
#include "error.h"
#include "errwarn.h"
#include "help.h"
#include "input.h"
#include "interpreter-private.h"
#include "interpreter.h"
#include "lex.h"
#include "load-path.h"
#include "octave-link.h"
#include "octave-preserve-stream-state.h"
#include "oct-map.h"
#include "ovl.h"
#include "ov.h"
#include "ov-class.h"
#include "ov-usr-fcn.h"
#include "pager.h"
#include "parse.h"
#include "symtab.h"
#include "unwind-prot.h"
#include "utils.h"
#include "variables.h"

// Defines layout for the whos/who -long command
static std::string Vwhos_line_format
  = "  %a:4; %ln:6; %cs:16:6:1;  %rb:12;  %lc:-1;\n";

// Attributes of variables and functions.

// Is this octave_value a valid function?

octave_function *
is_valid_function (const std::string& fcn_name,
                   const std::string& warn_for, bool warn)
{
  octave_function *ans = nullptr;

  if (! fcn_name.empty ())
    {
      octave::symbol_table& symtab
        = octave::__get_symbol_table__ ("is_valid_function");

      octave_value val = symtab.find_function (fcn_name);

      if (val.is_defined ())
        ans = val.function_value (true);
    }

  // FIXME: Should this be "err" and "error_for", rather than warn?
  if (! ans && warn)
    error ("%s: the symbol '%s' is not valid as a function",
           warn_for.c_str (), fcn_name.c_str ());

  return ans;
}

octave_function *
is_valid_function (const octave_value& arg,
                   const std::string& warn_for, bool warn)
{
  octave_function *ans = nullptr;

  std::string fcn_name;

  if (arg.is_string ())
    {
      fcn_name = arg.string_value ();

      ans = is_valid_function (fcn_name, warn_for, warn);
    }
  else if (warn)
    // FIXME: Should this be "err" and "error_for", rather than warn?
    error ("%s: argument must be a string containing function name",
           warn_for.c_str ());

  return ans;
}

octave_function *
extract_function (const octave_value& arg, const std::string& warn_for,
                  const std::string& fname, const std::string& header,
                  const std::string& trailer)
{
  octave_function *retval = nullptr;

  retval = is_valid_function (arg, warn_for, 0);

  if (! retval)
    {
      std::string s = arg.xstring_value ("%s: argument must be a string",
                                         warn_for.c_str ());

      std::string cmd = header;
      cmd.append (s);
      cmd.append (trailer);

      int parse_status;

      octave::eval_string (cmd, true, parse_status, 0);

      if (parse_status != 0)
        error ("%s: '%s' is not valid as a function",
               warn_for.c_str (), fname.c_str ());

      retval = is_valid_function (fname, warn_for, 0);

      if (! retval)
        error ("%s: '%s' is not valid as a function",
               warn_for.c_str (), fname.c_str ());

      warning ("%s: passing function body as a string is obsolete; please use anonymous functions",
               warn_for.c_str ());
    }

  return retval;
}

string_vector
get_struct_elts (const std::string& text)
{
  int n = 1;

  size_t pos = 0;

  size_t len = text.length ();

  while ((pos = text.find ('.', pos)) != std::string::npos)
    {
      if (++pos == len)
        break;

      n++;
    }

  string_vector retval (n);

  pos = 0;

  for (int i = 0; i < n; i++)
    {
      len = text.find ('.', pos);

      if (len != std::string::npos)
        len -= pos;

      retval[i] = text.substr (pos, len);

      if (len != std::string::npos)
        pos += len + 1;
    }

  return retval;
}

static inline bool
is_variable (octave::symbol_table& symtab, const std::string& name)
{
  bool retval = false;

  if (! name.empty ())
    {
      octave::symbol_scope scope = symtab.current_scope ();

      octave_value val = scope ? scope.varval (name) : octave_value ();

      retval = val.is_defined ();
    }

  return retval;
}

string_vector
generate_struct_completions (const std::string& text,
                             std::string& prefix, std::string& hint)
{
  string_vector names;

  size_t pos = text.rfind ('.');
  bool array = false;

  if (pos != std::string::npos)
    {
      if (pos == text.length ())
        hint = "";
      else
        hint = text.substr (pos+1);

      prefix = text.substr (0, pos);

      if (prefix == "")
        {
          array = true;
          prefix = find_indexed_expression (text);
        }

      std::string base_name = prefix;

      pos = base_name.find_first_of ("{(. ");

      if (pos != std::string::npos)
        base_name = base_name.substr (0, pos);

      octave::symbol_table& symtab
        = octave::__get_symbol_table__ ("generate_struct_completions");

      if (is_variable (symtab, base_name))
        {
          int parse_status;

          octave::unwind_protect frame;

          frame.protect_var (discard_error_messages);
          frame.protect_var (discard_warning_messages);

          discard_error_messages = true;
          discard_warning_messages = true;

          try
            {
              octave_value tmp = octave::eval_string (prefix, true, parse_status);

              frame.run ();

              if (tmp.is_defined ()
                  && (tmp.isstruct () || tmp.isjava () || tmp.is_classdef_object ()))
                names = tmp.map_keys ();
            }
          catch (const octave::execution_exception&)
            {
              octave::interpreter::recover_from_exception ();
            }
        }
    }

  // Undo look-back that found the array expression,
  // but insert an extra "." to distinguish from the non-struct case.
  if (array)
    prefix = ".";

  return names;
}

// FIXME: this will have to be much smarter to work "correctly".
bool
looks_like_struct (const std::string& text, char prev_char)
{
  bool retval = (! text.empty ()
                 && (text != "." || prev_char == ')' || prev_char == '}')
                 && text.find_first_of (octave::sys::file_ops::dir_sep_chars ()) == std::string::npos
                 && text.find ("..") == std::string::npos
                 && text.rfind ('.') != std::string::npos);

#if 0
  symbol_record *sr = curr_sym_tab->lookup (text);

  if (sr && ! sr->is_function ())
    {
      int parse_status;

      octave::unwind_protect frame;

      frame.protect_var (discard_error_messages);

      discard_error_messages = true;

      octave_value tmp = eval_string (text, true, parse_status);

      frame.run ();

      retval = (tmp.is_defined () && tmp.isstruct ());
    }
#endif

  return retval;
}

static octave_value
do_isglobal (octave::symbol_table& symtab, const octave_value_list& args)
{
  if (args.length () != 1)
    print_usage ();

  if (! args(0).is_string ())
    error ("isglobal: NAME must be a string");

  octave::symbol_scope scope = symtab.current_scope ();

  std::string name = args(0).string_value ();

  return scope && scope.is_global (name);
}

DEFMETHOD (isglobal, interp, args, ,
           doc: /* -*- texinfo -*-
@deftypefn {} {} isglobal (@var{name})
Return true if @var{name} is a globally visible variable.

For example:

@example
@group
global x
isglobal ("x")
   @result{} 1
@end group
@end example
@seealso{isvarname, exist}
@end deftypefn */)
{
  octave::symbol_table& symtab = interp.get_symbol_table ();

  return do_isglobal (symtab, args);
}

/*
%!test
%! global x;
%! assert (isglobal ("x"), true);

%!error isglobal ()
%!error isglobal ("a", "b")
%!error isglobal (1)
*/

static int
symbol_exist (octave::interpreter& interp, const std::string& name,
              const std::string& type = "any")
{
  if (octave::is_keyword (name))
    return 0;

  bool search_any = type == "any";
  bool search_var = type == "var";
  bool search_dir = type == "dir";
  bool search_file = type == "file";
  bool search_builtin = type == "builtin";
  bool search_class = type == "class";

  if (! (search_any || search_var || search_dir || search_file ||
         search_builtin || search_class))
    error (R"(exist: unrecognized type argument "%s")", type.c_str ());

  octave::symbol_table& symtab = interp.get_symbol_table ();

  if (search_any || search_var)
    {
      octave::symbol_scope scope = symtab.current_scope ();

      octave_value val = scope ? scope.varval (name) : octave_value ();

      if (val.is_constant () || val.isobject ()
          || val.is_function_handle ()
          || val.is_anonymous_function ()
          || val.is_inline_function ())
        return 1;

      if (search_var)
        return 0;
    }

  // We shouldn't need to look in the global symbol table, since any name
  // that is visible in the current scope will be in the local symbol table.

  // Command line function which Matlab does not support
  if (search_any)
    {
      octave_value val = symtab.find_cmdline_function (name);

      if (val.is_defined ())
        return 103;
    }

  if (search_any || search_file || search_dir)
    {
      std::string file_name = octave::lookup_autoload (name);

      if (file_name.empty ())
        {
          octave::load_path& lp = interp.get_load_path ();

          file_name = lp.find_fcn (name);
        }

      size_t len = file_name.length ();

      if (len > 0)
        {
          if (search_any || search_file)
            {
              if (len > 4 && (file_name.substr (len-4) == ".oct"
                              || file_name.substr (len-4) == ".mex"))
                return 3;
              else
                return 2;
            }
        }

      file_name = file_in_path (name, "");

      if (file_name.empty ())
        file_name = name;

      octave::sys::file_stat fs (file_name);

      if (fs)
        {
          if (search_any || search_file)
            {
              if (fs.is_dir ())
                return 7;

              len = file_name.length ();

              if (len > 4 && (file_name.substr (len-4) == ".oct"
                              || file_name.substr (len-4) == ".mex"))
                return 3;
              else
                return 2;
            }
          else if (search_dir && fs.is_dir ())
            return 7;
        }

      if (search_file || search_dir)
        return 0;
    }

  if (search_any || search_builtin)
    {
      if (symtab.is_built_in_function_name (name))
        return 5;

      if (search_builtin)
        return 0;
    }

  return 0;
}

int
symbol_exist (const std::string& name, const std::string& type)
{
  octave::interpreter& interp = octave::__get_interpreter__ ("symbol_exist");

  return symbol_exist (interp, name, type);
}


#define GET_IDX(LEN)                                                    \
  static_cast<int> ((LEN-1) * static_cast<double> (rand ()) / RAND_MAX)

std::string
unique_symbol_name (const std::string& basename)
{
  static const std::string alpha
    = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  static size_t len = alpha.length ();

  std::string nm = basename + alpha[GET_IDX (len)];

  size_t pos = nm.length ();

  if (nm.substr (0, 2) == "__")
    nm.append ("__");

  octave::interpreter& interp
    = octave::__get_interpreter__ ("unique_symbol_name");

  while (symbol_exist (interp, nm, "any"))
    nm.insert (pos++, 1, alpha[GET_IDX (len)]);

  return nm;
}

DEFMETHOD (exist, interp, args, ,
           doc: /* -*- texinfo -*-
@deftypefn  {} {@var{c} =} exist (@var{name})
@deftypefnx {} {@var{c} =} exist (@var{name}, @var{type})
Check for the existence of @var{name} as a variable, function, file, directory,
or class.

The return code @var{c} is one of

@table @asis
@item 1
@var{name} is a variable.

@item 2
@var{name} is an absolute filename, an ordinary file in Octave's @code{path},
or (after appending @samp{.m}) a function file in Octave's @code{path}.

@item 3
@var{name} is a @samp{.oct} or @samp{.mex} file in Octave's @code{path}.

@item 5
@var{name} is a built-in function.

@item 7
@var{name} is a directory.

@item 8
@var{name} is a class.  (Note: not currently implemented)

@item 103
@var{name} is a function not associated with a file (entered on the command
line).

@item 0
@var{name} does not exist.
@end table

If the optional argument @var{type} is supplied, check only for symbols of the
specified type.  Valid types are

@table @asis
@item @qcode{"var"}
Check only for variables.

@item @qcode{"builtin"}
Check only for built-in functions.

@item @qcode{"dir"}
Check only for directories.

@item @qcode{"file"}
Check only for files and directories.

@item @qcode{"class"}
Check only for classes.  (Note: This option is accepted, but not currently
implemented)
@end table

If no type is given, and there are multiple possible matches for name,
@code{exist} will return a code according to the following priority list:
variable, built-in function, oct-file, directory, file, class.

@code{exist} returns 2 if a regular file called @var{name} is present in
Octave's search path.  For information about other types of files not on the
search path use some combination of the functions @code{file_in_path} and
@code{stat} instead.

Programming Note: If @var{name} is implemented by a buggy .oct/.mex file,
calling @var{exist} may cause Octave to crash.  To maintain high performance,
Octave trusts .oct/.mex files instead of @nospell{sandboxing} them.

@seealso{file_in_loadpath, file_in_path, dir_in_loadpath, stat}
@end deftypefn */)
{
  int nargin = args.length ();

  if (nargin < 1 || nargin > 2)
    print_usage ();

  std::string name = args(0).xstring_value ("exist: NAME must be a string");

  if (nargin == 2)
    {
      std::string type = args(1).xstring_value ("exist: TYPE must be a string");

      if (type == "class")
        warning (R"(exist: "class" type argument is not implemented)");

      return ovl (symbol_exist (interp, name, type));
    }
  else
    return ovl (symbol_exist (interp, name));
}

/*
%!shared dirtmp, __var1
%! dirtmp = P_tmpdir ();
%! __var1 = 1;

%!assert (exist ("__%Highly_unlikely_name%__"), 0)
%!assert (exist ("__var1"), 1)
%!assert (exist ("__var1", "var"), 1)
%!assert (exist ("__var1", "builtin"), 0)
%!assert (exist ("__var1", "dir"), 0)
%!assert (exist ("__var1", "file"), 0)

%!test
%! if (isunix ())
%!   assert (exist ("/bin/sh"), 2);
%!   assert (exist ("/bin/sh", "file"), 2);
%!   assert (exist ("/bin/sh", "dir"), 0);
%!   assert (exist ("/dev/null"), 2);
%!   assert (exist ("/dev/null", "file"), 2);
%!   assert (exist ("/dev/null", "dir"), 0);
%! endif

%!assert (exist ("print_usage"), 2)
%!assert (exist ("print_usage.m"), 2)
%!assert (exist ("print_usage", "file"), 2)
%!assert (exist ("print_usage", "dir"), 0)

## Don't search path for rooted relative filenames
%!assert (exist ("plot.m", "file"), 2)
%!assert (exist ("./plot.m", "file"), 0)
%!assert (exist ("./%nonexistentfile%", "file"), 0)
%!assert (exist ("%nonexistentfile%", "file"), 0)

## Don't search path for absolute filenames
%!test
%! tname = tempname (pwd ());
%! unwind_protect
%!   ## open/close file to create it, equivalent of touch
%!   fid = fopen (tname, "w");
%!   fclose (fid);
%!   [~, fname] = fileparts (tname);
%!   assert (exist (fullfile (pwd (), fname), "file"), 2);
%! unwind_protect_cleanup
%!   unlink (tname);
%! end_unwind_protect
%! assert (exist (fullfile (pwd (), "%nonexistentfile%"), "file"), 0);

%!testif HAVE_CHOLMOD
%! assert (exist ("chol"), 3);
%! assert (exist ("chol.oct"), 3);
%! assert (exist ("chol", "file"), 3);
%! assert (exist ("chol", "builtin"), 0);

%!assert (exist ("sin"), 5)
%!assert (exist ("sin", "builtin"), 5)
%!assert (exist ("sin", "file"), 0)

%!assert (exist (dirtmp), 7)
%!assert (exist (dirtmp, "dir"), 7)
%!assert (exist (dirtmp, "file"), 7)

%!error exist ()
%!error exist (1,2,3)
%!warning <"class" type argument is not implemented> exist ("a", "class");
%!error <TYPE must be a string> exist ("a", 1)
%!error <NAME must be a string> exist (1)
%!error <unrecognized type argument "foobar"> exist ("a", "foobar")

*/

// Variable values.

static bool
wants_local_change (const octave_value_list& args, int& nargin)
{
  bool retval = false;

  if (nargin == 2)
    {
      if (! args(1).is_string () || args(1).string_value () != "local")
        error_with_cfn (R"(second argument must be "local")");

      nargin = 1;
      retval = true;
    }

  return retval;
}

static octave::unwind_protect *
curr_fcn_unwind_protect_frame (void)
{
  octave::call_stack& cs
    = octave::__get_call_stack__ ("curr_fcn_unwind_protect_frame");

  octave_user_code *curr_usr_code = cs.caller_user_code ();

  octave_user_function *curr_usr_fcn
    = (curr_usr_code && curr_usr_code->is_user_function ()
       ? dynamic_cast<octave_user_function *> (curr_usr_code) : nullptr);

  return curr_usr_fcn ? curr_usr_fcn->unwind_protect_frame () : nullptr;
}

template <typename T>
static bool
try_local_protect (T& var)
{
  octave::unwind_protect *frame = curr_fcn_unwind_protect_frame ();

  if (frame)
    {
      frame->protect_var (var);
      return true;
    }
  else
    return false;
}

octave_value
set_internal_variable (bool& var, const octave_value_list& args,
                       int nargout, const char *nm)
{
  octave_value retval;

  int nargin = args.length ();

  if (nargout > 0 || nargin == 0)
    retval = var;

  if (wants_local_change (args, nargin))
    {
      if (! try_local_protect (var))
        warning (R"("local" has no effect outside a function)");
    }

  if (nargin > 1)
    print_usage ();

  if (nargin == 1)
    {
      bool bval = args(0).xbool_value ("%s: argument must be a logical value", nm);

      var = bval;
    }

  return retval;
}

octave_value
set_internal_variable (char& var, const octave_value_list& args,
                       int nargout, const char *nm)
{
  octave_value retval;

  int nargin = args.length ();

  if (nargout > 0 || nargin == 0)
    retval = var;

  if (wants_local_change (args, nargin))
    {
      if (! try_local_protect (var))
        warning (R"("local" has no effect outside a function)");
    }

  if (nargin > 1)
    print_usage ();

  if (nargin == 1)
    {
      std::string sval = args(0).xstring_value ("%s: argument must be a single character", nm);

      switch (sval.length ())
        {
        case 1:
          var = sval[0];
          break;

        case 0:
          var = '\0';
          break;

        default:
          error ("%s: argument must be a single character", nm);
          break;
        }
    }

  return retval;
}

octave_value
set_internal_variable (int& var, const octave_value_list& args,
                       int nargout, const char *nm,
                       int minval, int maxval)
{
  octave_value retval;

  int nargin = args.length ();

  if (nargout > 0 || nargin == 0)
    retval = var;

  if (wants_local_change (args, nargin))
    {
      if (! try_local_protect (var))
        warning (R"("local" has no effect outside a function)");
    }

  if (nargin > 1)
    print_usage ();

  if (nargin == 1)
    {
      int ival = args(0).xint_value ("%s: argument must be an integer value", nm);

      if (ival < minval)
        error ("%s: arg must be greater than %d", nm, minval);
      if (ival > maxval)
        error ("%s: arg must be less than or equal to %d", nm, maxval);

      var = ival;
    }

  return retval;
}

octave_value
set_internal_variable (double& var, const octave_value_list& args,
                       int nargout, const char *nm,
                       double minval, double maxval)
{
  octave_value retval;

  int nargin = args.length ();

  if (nargout > 0 || nargin == 0)
    retval = var;

  if (wants_local_change (args, nargin))
    {
      if (! try_local_protect (var))
        warning (R"("local" has no effect outside a function)");
    }

  if (nargin > 1)
    print_usage ();

  if (nargin == 1)
    {
      double dval = args(0).xscalar_value ("%s: argument must be a scalar value", nm);

      if (dval < minval)
        error ("%s: argument must be greater than %g", minval);
      if (dval > maxval)
        error ("%s: argument must be less than or equal to %g", maxval);

      var = dval;
    }

  return retval;
}

octave_value
set_internal_variable (std::string& var, const octave_value_list& args,
                       int nargout, const char *nm, bool empty_ok)
{
  octave_value retval;

  int nargin = args.length ();

  if (nargout > 0 || nargin == 0)
    retval = var;

  if (wants_local_change (args, nargin))
    {
      if (! try_local_protect (var))
        warning (R"("local" has no effect outside a function)");
    }

  if (nargin > 1)
    print_usage ();

  if (nargin == 1)
    {
      std::string sval = args(0).xstring_value ("%s: first argument must be a string", nm);

      if (! empty_ok && sval.empty ())
        error ("%s: value must not be empty", nm);

      var = sval;
    }

  return retval;
}

octave_value
set_internal_variable (int& var, const octave_value_list& args,
                       int nargout, const char *nm, const char **choices)
{
  octave_value retval;
  int nchoices = 0;
  while (choices[nchoices] != nullptr)
    nchoices++;

  int nargin = args.length ();

  assert (var < nchoices);

  if (nargout > 0 || nargin == 0)
    retval = choices[var];

  if (wants_local_change (args, nargin))
    {
      if (! try_local_protect (var))
        warning (R"("local" has no effect outside a function)");
    }

  if (nargin > 1)
    print_usage ();

  if (nargin == 1)
    {
      std::string sval = args(0).xstring_value ("%s: first argument must be a string", nm);

      int i = 0;
      for (; i < nchoices; i++)
        {
          if (sval == choices[i])
            {
              var = i;
              break;
            }
        }
      if (i == nchoices)
        error (R"(%s: value not allowed ("%s"))", nm, sval.c_str ());
    }

  return retval;
}

octave_value
set_internal_variable (std::string& var, const octave_value_list& args,
                       int nargout, const char *nm, const char **choices)
{
  octave_value retval;
  int nchoices = 0;
  while (choices[nchoices] != nullptr)
    nchoices++;

  int nargin = args.length ();

  if (nargout > 0 || nargin == 0)
    retval = var;

  if (wants_local_change (args, nargin))
    {
      if (! try_local_protect (var))
        warning (R"("local" has no effect outside a function)");
    }

  if (nargin > 1)
    print_usage ();

  if (nargin == 1)
    {
      std::string sval = args(0).xstring_value ("%s: first argument must be a string", nm);

      int i = 0;
      for (; i < nchoices; i++)
        {
          if (sval == choices[i])
            {
              var = sval;
              break;
            }
        }
      if (i == nchoices)
        error (R"(%s: value not allowed ("%s"))", nm, sval.c_str ());
    }

  return retval;
}

struct
whos_parameter
{
  char command;
  char modifier;
  int parameter_length;
  int first_parameter_length;
  int balance;
  std::string text;
  std::string line;
};

static void
print_descriptor (std::ostream& os, std::list<whos_parameter> params)
{
  // This method prints a line of information on a given symbol
  std::list<whos_parameter>::iterator i = params.begin ();
  std::ostringstream param_buf;

  octave::preserve_stream_state stream_state (os);

  while (i != params.end ())
    {
      whos_parameter param = *i;

      if (param.command != '\0')
        {
          // Do the actual printing
          switch (param.modifier)
            {
            case 'l':
              os << std::setiosflags (std::ios::left)
                 << std::setw (param.parameter_length);
              param_buf << std::setiosflags (std::ios::left)
                        << std::setw (param.parameter_length);
              break;

            case 'r':
              os << std::setiosflags (std::ios::right)
                 << std::setw (param.parameter_length);
              param_buf << std::setiosflags (std::ios::right)
                        << std::setw (param.parameter_length);
              break;

            case 'c':
              if (param.command != 's')
                {
                  os << std::setiosflags (std::ios::left)
                     << std::setw (param.parameter_length);
                  param_buf << std::setiosflags (std::ios::left)
                            << std::setw (param.parameter_length);
                }
              break;

            default:
              os << std::setiosflags (std::ios::left)
                 << std::setw (param.parameter_length);
              param_buf << std::setiosflags (std::ios::left)
                        << std::setw (param.parameter_length);
            }

          if (param.command == 's' && param.modifier == 'c')
            {
              int a, b;

              if (param.modifier == 'c')
                {
                  a = param.first_parameter_length - param.balance;
                  a = (a < 0 ? 0 : a);
                  b = param.parameter_length - a - param.text.length ();
                  b = (b < 0 ? 0 : b);
                  os << std::setiosflags (std::ios::left) << std::setw (a)
                     << "" << std::resetiosflags (std::ios::left) << param.text
                     << std::setiosflags (std::ios::left)
                     << std::setw (b) << ""
                     << std::resetiosflags (std::ios::left);
                  param_buf << std::setiosflags (std::ios::left)
                            << std::setw (a)
                            << "" << std::resetiosflags (std::ios::left)
                            << param.line
                            << std::setiosflags (std::ios::left)
                            << std::setw (b) << ""
                            << std::resetiosflags (std::ios::left);
                }
            }
          else
            {
              os << param.text;
              param_buf << param.line;
            }
          os << std::resetiosflags (std::ios::left)
             << std::resetiosflags (std::ios::right);
          param_buf << std::resetiosflags (std::ios::left)
                    << std::resetiosflags (std::ios::right);
          i++;
        }
      else
        {
          os << param.text;
          param_buf << param.line;
          i++;
        }
    }

  os << param_buf.str ();
}

// FIXME: This is a bit of a kluge.  We'd like to just use val.dims()
// and if val is an object, expect that dims will call size if it is
// overloaded by a user-defined method.  But there are currently some
// unresolved const issues that prevent that solution from working.
// This same kluge is done in symtab.cc (do_workspace_info), fix there too.

std::string
get_dims_str (const octave_value& val)
{
  octave_value tmp = val;

  Matrix sz = tmp.size ();

  dim_vector dv = dim_vector::alloc (sz.numel ());

  for (octave_idx_type i = 0; i < dv.ndims (); i++)
    dv(i) = sz(i);

  return dv.str ();
}

class
symbol_info_list
{
private:
  struct symbol_info
  {
    symbol_info (const octave::symbol_record& sr,
                 octave::symbol_record::context_id context,
                 const std::string& expr_str = "",
                 const octave_value& expr_val = octave_value ())
      : name (expr_str.empty () ? sr.name () : expr_str),
        varval (),
        is_automatic (sr.is_automatic ()),
        is_complex (varval.iscomplex ()),
        is_formal (sr.is_formal ()),
        is_global (sr.is_global ()),
        is_persistent (sr.is_persistent ())
    {
      varval = (expr_val.is_undefined ()
                ? sr.varval (context) : expr_val);

      is_complex = varval.iscomplex ();
    }

    void display_line (std::ostream& os,
                       const std::list<whos_parameter>& params) const
    {
      std::string dims_str = get_dims_str (varval);

      std::list<whos_parameter>::const_iterator i = params.begin ();

      octave::preserve_stream_state stream_state (os);

      while (i != params.end ())
        {
          whos_parameter param = *i;

          if (param.command != '\0')
            {
              // Do the actual printing.

              switch (param.modifier)
                {
                case 'l':
                  os << std::setiosflags (std::ios::left)
                     << std::setw (param.parameter_length);
                  break;

                case 'r':
                  os << std::setiosflags (std::ios::right)
                     << std::setw (param.parameter_length);
                  break;

                case 'c':
                  if (param.command == 's')
                    {
                      int front = param.first_parameter_length
                                  - dims_str.find ('x');
                      int back = param.parameter_length
                                 - dims_str.length ()
                                 - front;
                      front = (front > 0) ? front : 0;
                      back = (back > 0) ? back : 0;

                      os << std::setiosflags (std::ios::left)
                         << std::setw (front)
                         << ""
                         << std::resetiosflags (std::ios::left)
                         << dims_str
                         << std::setiosflags (std::ios::left)
                         << std::setw (back)
                         << ""
                         << std::resetiosflags (std::ios::left);
                    }
                  else
                    {
                      os << std::setiosflags (std::ios::left)
                         << std::setw (param.parameter_length);
                    }
                  break;

                default:
                  error ("whos_line_format: modifier '%c' unknown",
                         param.modifier);

                  os << std::setiosflags (std::ios::right)
                     << std::setw (param.parameter_length);
                }

              switch (param.command)
                {
                case 'a':
                  {
                    char tmp[6];

                    tmp[0] = (is_automatic ? 'a' : ' ');
                    tmp[1] = (is_complex ? 'c' : ' ');
                    tmp[2] = (is_formal ? 'f' : ' ');
                    tmp[3] = (is_global ? 'g' : ' ');
                    tmp[4] = (is_persistent ? 'p' : ' ');
                    tmp[5] = 0;

                    os << tmp;
                  }
                  break;

                case 'b':
                  os << varval.byte_size ();
                  break;

                case 'c':
                  os << varval.class_name ();
                  break;

                case 'e':
                  os << varval.numel ();
                  break;

                case 'n':
                  os << name;
                  break;

                case 's':
                  if (param.modifier != 'c')
                    os << dims_str;
                  break;

                case 't':
                  os << varval.type_name ();
                  break;

                default:
                  error ("whos_line_format: command '%c' unknown",
                         param.command);
                }

              os << std::resetiosflags (std::ios::left)
                 << std::resetiosflags (std::ios::right);
              i++;
            }
          else
            {
              os << param.text;
              i++;
            }
        }
    }

    std::string name;
    octave_value varval;
    bool is_automatic;
    bool is_complex;
    bool is_formal;
    bool is_global;
    bool is_persistent;
  };

public:
  symbol_info_list (void) : lst () { }

  symbol_info_list (const symbol_info_list& sil) : lst (sil.lst) { }

  symbol_info_list& operator = (const symbol_info_list& sil)
  {
    if (this != &sil)
      lst = sil.lst;

    return *this;
  }

  ~symbol_info_list (void) = default;

  void append (const octave::symbol_record& sr,
               octave::symbol_record::context_id context)
  {
    lst.push_back (symbol_info (sr, context));
  }

  void append (const octave::symbol_record& sr,
               octave::symbol_record::context_id context,
               const std::string& expr_str,
               const octave_value& expr_val)
  {
    lst.push_back (symbol_info (sr, context, expr_str, expr_val));
  }

  size_t size (void) const { return lst.size (); }

  bool empty (void) const { return lst.empty (); }

  octave_map
  map_value (const std::string& caller_function_name, int nesting_level) const
  {
    size_t len = lst.size ();

    Cell name_info (len, 1);
    Cell size_info (len, 1);
    Cell bytes_info (len, 1);
    Cell class_info (len, 1);
    Cell global_info (len, 1);
    Cell sparse_info (len, 1);
    Cell complex_info (len, 1);
    Cell nesting_info (len, 1);
    Cell persistent_info (len, 1);

    std::list<symbol_info>::const_iterator p = lst.begin ();

    for (size_t j = 0; j < len; j++)
      {
        const symbol_info& si = *p++;

        octave_scalar_map ni;

        ni.assign ("function", caller_function_name);
        ni.assign ("level", nesting_level);

        name_info(j) = si.name;
        global_info(j) = si.is_global;
        persistent_info(j) = si.is_persistent;

        octave_value val = si.varval;

        size_info(j) = val.size ();
        bytes_info(j) = val.byte_size ();
        class_info(j) = val.class_name ();
        sparse_info(j) = val.issparse ();
        complex_info(j) = val.iscomplex ();
        nesting_info(j) = ni;
      }

    octave_map info;

    info.assign ("name", name_info);
    info.assign ("size", size_info);
    info.assign ("bytes", bytes_info);
    info.assign ("class", class_info);
    info.assign ("global", global_info);
    info.assign ("sparse", sparse_info);
    info.assign ("complex", complex_info);
    info.assign ("nesting", nesting_info);
    info.assign ("persistent", persistent_info);

    return info;
  }

  void display (std::ostream& os)
  {
    if (! lst.empty ())
      {
        size_t bytes = 0;
        size_t elements = 0;

        std::list<whos_parameter> params = parse_whos_line_format ();

        print_descriptor (os, params);

        octave_stdout << "\n";

        for (const auto& syminfo : lst)
          {
            syminfo.display_line (os, params);

            octave_value val = syminfo.varval;

            elements += val.numel ();
            bytes += val.byte_size ();
          }

        os << "\nTotal is " << elements
           << (elements == 1 ? " element" : " elements")
           << " using " << bytes << (bytes == 1 ? " byte" : " bytes")
           << "\n";
      }
  }

  // Parse the string whos_line_format, and return a parameter list,
  // containing all information needed to print the given
  // attributes of the symbols.
  std::list<whos_parameter> parse_whos_line_format (void)
  {
    int idx;
    size_t format_len = Vwhos_line_format.length ();
    char garbage;
    std::list<whos_parameter> params;

    size_t bytes1;
    int elements1;

    std::string param_string = "abcenst";
    Array<int> param_length (dim_vector (param_string.length (), 1));
    Array<std::string> param_names (dim_vector (param_string.length (), 1));
    size_t pos_a, pos_b, pos_c, pos_e, pos_n, pos_s, pos_t;

    pos_a = param_string.find ('a'); // Attributes
    pos_b = param_string.find ('b'); // Bytes
    pos_c = param_string.find ('c'); // Class
    pos_e = param_string.find ('e'); // Elements
    pos_n = param_string.find ('n'); // Name
    pos_s = param_string.find ('s'); // Size
    pos_t = param_string.find ('t'); // Type

    param_names(pos_a) = "Attr";
    param_names(pos_b) = "Bytes";
    param_names(pos_c) = "Class";
    param_names(pos_e) = "Elements";
    param_names(pos_n) = "Name";
    param_names(pos_s) = "Size";
    param_names(pos_t) = "Type";

    for (size_t i = 0; i < param_string.length (); i++)
      param_length(i) = param_names(i).length ();

    // The attribute column needs size 5.
    param_length(pos_a) = 5;

    // Calculating necessary spacing for name column,
    // bytes column, elements column and class column

    for (const auto& syminfo : lst)
      {
        std::stringstream ss1, ss2;
        std::string str;

        str = syminfo.name;
        param_length(pos_n) = ((str.length ()
                                > static_cast<size_t> (param_length(pos_n)))
                               ? str.length () : param_length(pos_n));

        octave_value val = syminfo.varval;

        str = val.type_name ();
        param_length(pos_t) = ((str.length ()
                                > static_cast<size_t> (param_length(pos_t)))
                               ? str.length () : param_length(pos_t));

        elements1 = val.numel ();
        ss1 << elements1;
        str = ss1.str ();
        param_length(pos_e) = ((str.length ()
                                > static_cast<size_t> (param_length(pos_e)))
                               ? str.length () : param_length(pos_e));

        bytes1 = val.byte_size ();
        ss2 << bytes1;
        str = ss2.str ();
        param_length(pos_b) = ((str.length ()
                                > static_cast<size_t> (param_length(pos_b)))
                               ? str.length () : param_length (pos_b));
      }

    idx = 0;
    while (static_cast<size_t> (idx) < format_len)
      {
        whos_parameter param;
        param.command = '\0';

        if (Vwhos_line_format[idx] == '%')
          {
            bool error_encountered = false;
            param.modifier = 'r';
            param.parameter_length = 0;

            int a = 0;
            int b = -1;
            int balance = 1;
            unsigned int items;
            size_t pos;
            std::string cmd;

            // Parse one command from whos_line_format
            cmd = Vwhos_line_format.substr (idx, Vwhos_line_format.length ());
            pos = cmd.find (';');
            if (pos == std::string::npos)
              error ("parameter without ; in whos_line_format");

            cmd = cmd.substr (0, pos+1);

            idx += cmd.length ();

            // FIXME: use iostream functions instead of sscanf!

            if (cmd.find_first_of ("crl") != 1)
              items = sscanf (cmd.c_str (), "%c%c:%d:%d:%d;",
                              &garbage, &param.command, &a, &b, &balance);
            else
              items = sscanf (cmd.c_str (), "%c%c%c:%d:%d:%d;",
                              &garbage, &param.modifier, &param.command,
                              &a, &b, &balance) - 1;

            if (items < 2)
              error ("whos_line_format: parameter structure without command in whos_line_format");

            // Exception case of bare class command 'c' without modifier 'l/r'
            if (param.modifier == 'c'
                && param_string.find (param.command) == std::string::npos)
              {
                param.modifier = 'r';
                param.command = 'c';
              }

            // Insert data into parameter
            param.first_parameter_length = 0;
            pos = param_string.find (param.command);
            if (pos == std::string::npos)
              error ("whos_line_format: '%c' is not a command", param.command);

            param.parameter_length = param_length(pos);
            param.text = param_names(pos);
            param.line.assign (param_names(pos).length (), '=');

            param.parameter_length = (a > param.parameter_length
                                      ? a : param.parameter_length);
            if (param.command == 's' && param.modifier == 'c' && b > 0)
              param.first_parameter_length = b;

            if (param.command == 's')
              {
                // Have to calculate space needed for printing
                // matrix dimensions Space needed for Size column is
                // hard to determine in prior, because it depends on
                // dimensions to be shown.  That is why it is
                // recalculated for each Size-command int first,
                // rest = 0, total;
                int rest = 0;
                int first = param.first_parameter_length;
                int total = param.parameter_length;

                for (const auto& syminfo : lst)
                  {
                    octave_value val = syminfo.varval;
                    std::string dims_str = get_dims_str (val);
                    int first1 = dims_str.find ('x');
                    int total1 = dims_str.length ();
                    int rest1 = total1 - first1;
                    rest = (rest1 > rest ? rest1 : rest);
                    first = (first1 > first ? first1 : first);
                    total = (total1 > total ? total1 : total);
                  }

                if (param.modifier == 'c')
                  {
                    if (first < balance)
                      first += balance - first;
                    if (rest + balance < param.parameter_length)
                      rest += param.parameter_length - rest - balance;

                    param.parameter_length = first + rest;
                    param.first_parameter_length = first;
                    param.balance = balance;
                  }
                else
                  {
                    param.parameter_length = total;
                    param.first_parameter_length = 0;
                  }
              }
            else if (param.modifier == 'c')
              error ("whos_line_format: modifier 'c' not available for command '%c'",
                     param.command);

            // What happens if whos_line_format contains negative numbers
            // at param_length positions?
            param.balance = (b < 0 ? 0 : param.balance);
            param.first_parameter_length = (b < 0
                                            ? 0
                                            : param.first_parameter_length);
            param.parameter_length = (a < 0
                                      ? 0
                                      : (param.parameter_length
                                         < param_length(pos_s)
                                         ? param_length(pos_s)
                                         : param.parameter_length));

            // Parameter will not be pushed into parameter list if ...
            if (! error_encountered)
              params.push_back (param);
          }
        else
          {
            // Text string, to be printed as it is ...
            std::string text;
            size_t pos;
            text = Vwhos_line_format.substr (idx, Vwhos_line_format.length ());
            pos = text.find ('%');
            if (pos != std::string::npos)
              text = text.substr (0, pos);

            // Push parameter into list ...
            idx += text.length ();
            param.text=text;
            param.line.assign (text.length (), ' ');
            params.push_back (param);
          }
      }

    return params;
  }

private:
  std::list<symbol_info> lst;

};

static octave_value
do_who (octave::interpreter& interp, int argc, const string_vector& argv,
        bool return_list, bool verbose = false, std::string msg = "")
{
  octave_value retval;

  octave::symbol_table& symtab = interp.get_symbol_table ();
  octave::call_stack& cs = interp.get_call_stack ();

  std::string my_name = argv[0];

  bool global_only = false;
  bool have_regexp = false;

  int i;
  for (i = 1; i < argc; i++)
    {
      if (argv[i] == "-file")
        {
          // FIXME: This is an inefficient manner to implement this as the
          // variables are loaded in to a temporary context and then treated.
          // It would be better to refecat symbol_info_list to not store the
          // symbol records and then use it in load-save.cc (do_load) to
          // implement this option there so that the variables are never
          // stored at all.
          if (i == argc - 1)
            error ("%s: -file argument must be followed by a filename",
                   my_name.c_str ());

          std::string nm = argv[i + 1];

          octave::unwind_protect frame;

          // Set up temporary scope.

          octave::symbol_scope tmp_scope ("$dummy_scope$");

          symtab.set_scope (tmp_scope);

          cs.push (tmp_scope, 0);
          frame.add_method (cs, &octave::call_stack::pop);

          octave::feval ("load", octave_value (nm), 0);

          std::string newmsg = "Variables in the file " + nm + ":\n\n";

          retval = do_who (interp, i, argv, return_list, verbose, newmsg);

          return retval;
        }
      else if (argv[i] == "-regexp")
        have_regexp = true;
      else if (argv[i] == "global")
        global_only = true;
      else if (argv[i][0] == '-')
        warning ("%s: unrecognized option '%s'", my_name.c_str (),
                 argv[i].c_str ());
      else
        break;
    }

  int npats = argc - i;
  string_vector pats;
  if (npats > 0)
    {
      pats.resize (npats);
      for (int j = 0; j < npats; j++)
        pats[j] = argv[i+j];
    }
  else
    {
      pats.resize (++npats);
      pats[0] = "*";
    }

  symbol_info_list symbol_stats;
  std::list<std::string> symbol_names;

  octave::symbol_scope scope = symtab.current_scope ();

  octave::symbol_record::context_id context = scope.current_context ();

  for (int j = 0; j < npats; j++)
    {
      std::string pat = pats[j];

      if (have_regexp)
        {
          std::list<octave::symbol_record> tmp
            = (global_only
               ? symtab.regexp_global_variables (pat)
               : symtab.regexp_variables (pat));

          for (const auto& symrec : tmp)
            {
              if (symrec.is_variable (context))
                {
                  if (verbose)
                    symbol_stats.append (symrec, context);
                  else
                    symbol_names.push_back (symrec.name ());
                }
            }
        }
      else
        {
          size_t pos = pat.find_first_of (".({");

          if (pos != std::string::npos && pos > 0)
            {
              if (verbose)
                {
                  // NOTE: we can only display information for
                  // expressions based on global values if the variable is
                  // global in the current scope because we currently have
                  // no way of looking up the base value in the global
                  // scope and then evaluating the arguments in the
                  // current scope.

                  std::string base_name = pat.substr (0, pos);

                  if (scope && scope.is_variable (base_name))
                    {
                      octave::symbol_record sr
                        = symtab.find_symbol (base_name);

                      if (! global_only || sr.is_global ())
                        {
                          int parse_status;

                          octave_value expr_val
                            = octave::eval_string (pat, true, parse_status);

                          symbol_stats.append (sr, context, pat, expr_val);
                        }
                    }
                }
            }
          else
            {
              std::list<octave::symbol_record> tmp
                = (global_only
                   ? symtab.glob_global_variables (pat)
                   : symtab.glob_variables (pat));

              for (const auto& symrec : tmp)
                {
                  if (symrec.is_variable (context))
                    {
                      if (verbose)
                        symbol_stats.append (symrec, context);
                      else
                        symbol_names.push_back (symrec.name ());
                    }
                }
            }
        }
    }

  if (return_list)
    {
      if (verbose)
        {
          std::string caller_function_name;
          octave_function *caller = cs.caller ();
          if (caller)
            caller_function_name = caller->name ();

          retval = symbol_stats.map_value (caller_function_name, 1);
        }
      else
        retval = Cell (string_vector (symbol_names));
    }
  else if (! (symbol_stats.empty () && symbol_names.empty ()))
    {
      if (msg.empty ())
        if (global_only)
          octave_stdout << "Global variables:\n\n";
        else
          octave_stdout << "Variables in the current scope:\n\n";
      else
        octave_stdout << msg;

      if (verbose)
        symbol_stats.display (octave_stdout);
      else
        {
          string_vector names (symbol_names);

          names.list_in_columns (octave_stdout);
        }

      octave_stdout << "\n";
    }

  return retval;
}

DEFMETHOD (who, interp, args, nargout,
           doc: /* -*- texinfo -*-
@deftypefn  {} {} who
@deftypefnx {} {} who pattern @dots{}
@deftypefnx {} {} who option pattern @dots{}
@deftypefnx {} {C =} who ("pattern", @dots{})
List currently defined variables matching the given patterns.

Valid pattern syntax is the same as described for the @code{clear} command.
If no patterns are supplied, all variables are listed.

By default, only variables visible in the local scope are displayed.

The following are valid options, but may not be combined.

@table @code
@item global
List variables in the global scope rather than the current scope.

@item -regexp
The patterns are considered to be regular expressions when matching the
variables to display.  The same pattern syntax accepted by the @code{regexp}
function is used.

@item -file
The next argument is treated as a filename.  All variables found within the
specified file are listed.  No patterns are accepted when reading variables
from a file.
@end table

If called as a function, return a cell array of defined variable names
matching the given patterns.
@seealso{whos, isglobal, isvarname, exist, regexp}
@end deftypefn */)
{
  int argc = args.length () + 1;

  string_vector argv = args.make_argv ("who");

  return do_who (interp, argc, argv, nargout == 1);
}

/*
%!test
%! avar = magic (4);
%! ftmp = [tempname() ".mat"];
%! unwind_protect
%!   save (ftmp, "avar");
%!   vars = whos ("-file", ftmp);
%!   assert (numel (vars), 1);
%!   assert (isstruct (vars));
%!   assert (vars.name, "avar");
%!   assert (vars.size, [4, 4]);
%!   assert (vars.class, "double");
%!   assert (vars.bytes, 128);
%! unwind_protect_cleanup
%!   unlink (ftmp);
%! end_unwind_protect
*/

DEFMETHOD (whos, interp, args, nargout,
           doc: /* -*- texinfo -*-
@deftypefn  {} {} whos
@deftypefnx {} {} whos pattern @dots{}
@deftypefnx {} {} whos option pattern @dots{}
@deftypefnx {} {S =} whos ("pattern", @dots{})
Provide detailed information on currently defined variables matching the
given patterns.

Options and pattern syntax are the same as for the @code{who} command.

Extended information about each variable is summarized in a table with the
following default entries.

@table @asis
@item Attr
Attributes of the listed variable.  Possible attributes are:

@table @asis
@item blank
Variable in local scope

@item @code{a}
Automatic variable.  An automatic variable is one created by the
interpreter, for example @code{argn}.

@item @code{c}
Variable of complex type.

@item @code{f}
Formal parameter (function argument).

@item @code{g}
Variable with global scope.

@item @code{p}
Persistent variable.
@end table

@item Name
The name of the variable.

@item Size
The logical size of the variable.  A scalar is 1x1, a vector is
@nospell{1xN} or @nospell{Nx1}, a 2-D matrix is @nospell{MxN}.

@item Bytes
The amount of memory currently used to store the variable.

@item Class
The class of the variable.  Examples include double, single, char, uint16,
cell, and struct.
@end table

The table can be customized to display more or less information through
the function @code{whos_line_format}.

If @code{whos} is called as a function, return a struct array of defined
variable names matching the given patterns.  Fields in the structure
describing each variable are: name, size, bytes, class, global, sparse,
complex, nesting, persistent.
@seealso{who, whos_line_format}
@end deftypefn */)
{
  int argc = args.length () + 1;

  string_vector argv = args.make_argv ("whos");

  return do_who (interp, argc, argv, nargout == 1, true);
}

DEFMETHOD (mlock, interp, args, ,
           doc: /* -*- texinfo -*-
@deftypefn {} {} mlock ()
Lock the current function into memory so that it can't be cleared.
@seealso{munlock, mislocked, persistent}
@end deftypefn */)
{
  if (args.length () != 0)
    print_usage ();

  octave::call_stack& cs = interp.get_call_stack ();

  octave_function *fcn = cs.caller ();

  if (! fcn)
    error ("mlock: invalid use outside a function");

  fcn->lock ();

  return ovl ();
}

DEFMETHOD (munlock, interp, args, ,
           doc: /* -*- texinfo -*-
@deftypefn  {} {} munlock ()
@deftypefnx {} {} munlock (@var{fcn})
Unlock the named function @var{fcn}.

If no function is named then unlock the current function.
@seealso{mlock, mislocked, persistent}
@end deftypefn */)
{
  int nargin = args.length ();

  if (nargin > 1)
    print_usage ();

  if (nargin == 1)
    {
      std::string name = args(0).xstring_value ("munlock: FCN must be a string");

      interp.munlock (name);
    }
  else
    {
      octave::call_stack& cs = interp.get_call_stack ();

      octave_function *fcn = cs.caller ();

      if (! fcn)
        error ("munlock: invalid use outside a function");

      fcn->unlock ();
    }

  return ovl ();
}

DEFMETHOD (mislocked, interp, args, ,
           doc: /* -*- texinfo -*-
@deftypefn  {} {} mislocked ()
@deftypefnx {} {} mislocked (@var{fcn})
Return true if the named function @var{fcn} is locked.

If no function is named then return true if the current function is locked.
@seealso{mlock, munlock, persistent}
@end deftypefn */)
{
  int nargin = args.length ();

  if (nargin > 1)
    print_usage ();

  octave_value retval;

  if (nargin == 1)
    {
      std::string name = args(0).xstring_value ("mislocked: FCN must be a string");

      retval = interp.mislocked (name);
    }
  else
    {
      octave::call_stack& cs = interp.get_call_stack ();

      octave_function *fcn = cs.caller ();

      if (! fcn)
        error ("mislocked: invalid use outside a function");

      retval = fcn->islocked ();
    }

  return retval;
}

// Deleting names from the symbol tables.

static inline bool
name_matches_any_pattern (const std::string& nm, const string_vector& argv,
                          int argc, int idx, bool have_regexp = false)
{
  bool retval = false;

  for (int k = idx; k < argc; k++)
    {
      std::string patstr = argv[k];
      if (! patstr.empty ())
        {
          if (have_regexp)
            {
              if (octave::regexp::is_match (patstr, nm))
                {
                  retval = true;
                  break;
                }
            }
          else
            {
              glob_match pattern (patstr);

              if (pattern.match (nm))
                {
                  retval = true;
                  break;
                }
            }
        }
    }

  return retval;
}

static inline void
maybe_warn_exclusive (bool exclusive)
{
  if (exclusive)
    warning ("clear: ignoring --exclusive option");
}

static void
do_clear_functions (octave::symbol_table& symtab,
                    const string_vector& argv, int argc, int idx,
                    bool exclusive = false)
{
  if (idx == argc)
    symtab.clear_functions ();
  else
    {
      if (exclusive)
        {
          string_vector fcns = symtab.user_function_names ();

          int fcount = fcns.numel ();

          for (int i = 0; i < fcount; i++)
            {
              std::string nm = fcns[i];

              if (! name_matches_any_pattern (nm, argv, argc, idx))
                symtab.clear_function (nm);
            }
        }
      else
        {
          while (idx < argc)
            symtab.clear_function_pattern (argv[idx++]);
        }
    }
}

static void
do_clear_globals (octave::symbol_table& symtab,
                  const string_vector& argv, int argc, int idx,
                  bool exclusive = false)
{
  octave::symbol_scope scope = symtab.current_scope ();

  if (! scope)
    return;

  if (idx == argc)
    {
      string_vector gvars = symtab.global_variable_names ();

      int gcount = gvars.numel ();

      for (int i = 0; i < gcount; i++)
        {
          std::string name = gvars[i];

          scope.clear_variable (name);
          symtab.clear_global (name);
        }
    }
  else
    {
      if (exclusive)
        {
          string_vector gvars = symtab.global_variable_names ();

          int gcount = gvars.numel ();

          for (int i = 0; i < gcount; i++)
            {
              std::string name = gvars[i];

              if (! name_matches_any_pattern (name, argv, argc, idx))
                {
                  scope.clear_variable (name);
                  symtab.clear_global (name);
                }
            }
        }
      else
        {
          while (idx < argc)
            {
              std::string pattern = argv[idx++];

              scope.clear_variable_pattern (pattern);
              symtab.clear_global_pattern (pattern);
            }
        }
    }
}

static void
do_clear_variables (octave::symbol_table& symtab,
                    const string_vector& argv, int argc, int idx,
                    bool exclusive = false, bool have_regexp = false)
{
  octave::symbol_scope scope = symtab.current_scope ();

  if (! scope)
    return;

  if (idx == argc)
    scope.clear_variables ();
  else
    {
      if (exclusive)
        {
          string_vector lvars = scope.variable_names ();

          int lcount = lvars.numel ();

          for (int i = 0; i < lcount; i++)
            {
              std::string nm = lvars[i];

              if (! name_matches_any_pattern (nm, argv, argc, idx, have_regexp))
                scope.clear_variable (nm);
            }
        }
      else
        {
          if (have_regexp)
            while (idx < argc)
              scope.clear_variable_regexp (argv[idx++]);
          else
            while (idx < argc)
              scope.clear_variable_pattern (argv[idx++]);
        }
    }
}

static void
do_clear_symbols (octave::symbol_table& symtab,
                  const string_vector& argv, int argc, int idx,
                  bool exclusive = false)
{
  if (idx == argc)
    {
      octave::symbol_scope scope = symtab.current_scope ();

      if (scope)
        scope.clear_variables ();
    }
  else
    {
      if (exclusive)
        {
          // FIXME: is this really what we want, or do we
          // somehow want to only clear the functions that are not
          // shadowed by local variables?  It seems that would be a
          // bit harder to do.

          do_clear_variables (symtab, argv, argc, idx, exclusive);
          do_clear_functions (symtab, argv, argc, idx, exclusive);
        }
      else
        {
          while (idx < argc)
            symtab.clear_symbol_pattern (argv[idx++]);
        }
    }
}

static void
do_matlab_compatible_clear (octave::symbol_table& symtab,
                            const string_vector& argv, int argc, int idx)
{
  // This is supposed to be mostly Matlab compatible.

  octave::symbol_scope scope = symtab.current_scope ();

  if (! scope)
    return;

  for (; idx < argc; idx++)
    {
      if (argv[idx] == "all"
          && ! scope.is_local_variable ("all"))
        {
          symtab.clear_all ();
        }
      else if (argv[idx] == "functions"
               && ! scope.is_local_variable ("functions"))
        {
          do_clear_functions (symtab, argv, argc, ++idx);
        }
      else if (argv[idx] == "global"
               && ! scope.is_local_variable ("global"))
        {
          do_clear_globals (symtab, argv, argc, ++idx);
        }
      else if (argv[idx] == "variables"
               && ! scope.is_local_variable ("variables"))
        {
          scope.clear_variables ();
        }
      else if (argv[idx] == "classes"
               && ! scope.is_local_variable ("classes"))
        {
          scope.clear_objects ();
          octave_class::clear_exemplar_map ();
          symtab.clear_all ();
        }
      else
        {
          symtab.clear_symbol_pattern (argv[idx]);
        }
    }
}

#define CLEAR_OPTION_ERROR(cond)                \
  do                                            \
    {                                           \
      if (cond)                                 \
        print_usage ();                         \
    }                                           \
  while (0)

DEFMETHOD (clear, interp, args, ,
           doc: /* -*- texinfo -*-
@deftypefn {} {} clear [options] pattern @dots{}
Delete the names matching the given patterns from the symbol table.

The pattern may contain the following special characters:

@table @code
@item ?
Match any single character.

@item *
Match zero or more characters.

@item [ @var{list} ]
Match the list of characters specified by @var{list}.  If the first
character is @code{!} or @code{^}, match all characters except those
specified by @var{list}.  For example, the pattern @samp{[a-zA-Z]} will
match all lowercase and uppercase alphabetic characters.
@end table

For example, the command

@example
clear foo b*r
@end example

@noindent
clears the name @code{foo} and all names that begin with the letter
@code{b} and end with the letter @code{r}.

If @code{clear} is called without any arguments, all user-defined
variables (local and global) are cleared from the symbol table.

If @code{clear} is called with at least one argument, only the visible
names matching the arguments are cleared.  For example, suppose you have
defined a function @code{foo}, and then hidden it by performing the
assignment @code{foo = 2}.  Executing the command @kbd{clear foo} once
will clear the variable definition and restore the definition of
@code{foo} as a function.  Executing @kbd{clear foo} a second time will
clear the function definition.

The following options are available in both long and short form

@table @code
@item -all, -a
Clear all local and global user-defined variables and all functions from the
symbol table.

@item -exclusive, -x
Clear the variables that don't match the following pattern.

@item -functions, -f
Clear the function names and the built-in symbols names.

@item -global, -g
Clear global symbol names.

@item -variables, -v
Clear local variable names.

@item -classes, -c
Clears the class structure table and clears all objects.

@item -regexp, -r
The arguments are treated as regular expressions as any variables that
match will be cleared.
@end table

With the exception of @code{exclusive}, all long options can be used
without the dash as well.
@seealso{who, whos, exist}
@end deftypefn */)
{
  octave::symbol_table& symtab = interp.get_symbol_table ();

  int argc = args.length () + 1;

  string_vector argv = args.make_argv ("clear");

  if (argc == 1)
    {
      do_clear_globals (symtab, argv, argc, true);
      do_clear_variables (symtab, argv, argc, true);

      octave_link::clear_workspace ();
    }
  else
    {
      int idx = 0;

      bool clear_all = false;
      bool clear_functions = false;
      bool clear_globals = false;
      bool clear_variables = false;
      bool clear_objects = false;
      bool exclusive = false;
      bool have_regexp = false;
      bool have_dash_option = false;

      octave::symbol_scope scope = symtab.current_scope ();

      while (++idx < argc)
        {
          if (argv[idx] == "-all" || argv[idx] == "-a")
            {
              CLEAR_OPTION_ERROR (have_dash_option && ! exclusive);

              have_dash_option = true;
              clear_all = true;
            }
          else if (argv[idx] == "-exclusive" || argv[idx] == "-x")
            {
              have_dash_option = true;
              exclusive = true;
            }
          else if (argv[idx] == "-functions" || argv[idx] == "-f")
            {
              CLEAR_OPTION_ERROR (have_dash_option && ! exclusive);

              have_dash_option = true;
              clear_functions = true;
            }
          else if (argv[idx] == "-global" || argv[idx] == "-g")
            {
              CLEAR_OPTION_ERROR (have_dash_option && ! exclusive);

              have_dash_option = true;
              clear_globals = true;
            }
          else if (argv[idx] == "-variables" || argv[idx] == "-v")
            {
              CLEAR_OPTION_ERROR (have_dash_option && ! exclusive);

              have_dash_option = true;
              clear_variables = true;
            }
          else if (argv[idx] == "-classes" || argv[idx] == "-c")
            {
              CLEAR_OPTION_ERROR (have_dash_option && ! exclusive);

              have_dash_option = true;
              clear_objects = true;
            }
          else if (argv[idx] == "-regexp" || argv[idx] == "-r")
            {
              CLEAR_OPTION_ERROR (have_dash_option && ! exclusive);

              have_dash_option = true;
              have_regexp = true;
            }
          else
            break;
        }

      if (idx <= argc)
        {
          if (! have_dash_option)
            do_matlab_compatible_clear (symtab, argv, argc, idx);
          else
            {
              if (clear_all)
                {
                  maybe_warn_exclusive (exclusive);

                  if (++idx < argc)
                    warning ("clear: ignoring extra arguments after -all");

                  symtab.clear_all ();
                }
              else if (have_regexp)
                {
                  do_clear_variables (symtab, argv, argc, idx, exclusive, true);
                }
              else if (clear_functions)
                {
                  do_clear_functions (symtab, argv, argc, idx, exclusive);
                }
              else if (clear_globals)
                {
                  do_clear_globals (symtab, argv, argc, idx, exclusive);
                }
              else if (clear_variables)
                {
                  do_clear_variables (symtab, argv, argc, idx, exclusive);
                }
              else if (clear_objects)
                {
                  if (scope)
                    scope.clear_objects ();
                  octave_class::clear_exemplar_map ();
                  symtab.clear_all ();
                }
              else
                {
                  do_clear_symbols (symtab, argv, argc, idx, exclusive);
                }
            }

          octave_link::set_workspace ();
        }
    }

  return ovl ();
}

DEFUN (whos_line_format, args, nargout,
       doc: /* -*- texinfo -*-
@deftypefn  {} {@var{val} =} whos_line_format ()
@deftypefnx {} {@var{old_val} =} whos_line_format (@var{new_val})
@deftypefnx {} {} whos_line_format (@var{new_val}, "local")
Query or set the format string used by the command @code{whos}.

A full format string is:
@c Set example in small font to prevent overfull line

@smallexample
%[modifier]<command>[:width[:left-min[:balance]]];
@end smallexample

The following command sequences are available:

@table @code
@item %a
Prints attributes of variables (g=global, p=persistent, f=formal parameter,
a=automatic variable).

@item %b
Prints number of bytes occupied by variables.

@item %c
Prints class names of variables.

@item %e
Prints elements held by variables.

@item %n
Prints variable names.

@item %s
Prints dimensions of variables.

@item %t
Prints type names of variables.
@end table

Every command may also have an alignment modifier:

@table @code
@item l
Left alignment.

@item r
Right alignment (default).

@item c
Column-aligned (only applicable to command %s).
@end table

The @code{width} parameter is a positive integer specifying the minimum
number of columns used for printing.  No maximum is needed as the field will
auto-expand as required.

The parameters @code{left-min} and @code{balance} are only available when
the column-aligned modifier is used with the command @samp{%s}.
@code{balance} specifies the column number within the field width which
will be aligned between entries.  Numbering starts from 0 which indicates
the leftmost column.  @code{left-min} specifies the minimum field width to
the left of the specified balance column.

The default format is:

@qcode{"  %a:4; %ln:6; %cs:16:6:1;  %rb:12;  %lc:-1;@xbackslashchar{}n"}

When called from inside a function with the @qcode{"local"} option, the
variable is changed locally for the function and any subroutines it calls.
The original variable value is restored when exiting the function.
@seealso{whos}
@end deftypefn */)
{
  return SET_INTERNAL_VARIABLE (whos_line_format);
}

static std::string Vmissing_function_hook = "__unimplemented__";

DEFUN (missing_function_hook, args, nargout,
       doc: /* -*- texinfo -*-
@deftypefn  {} {@var{val} =} missing_function_hook ()
@deftypefnx {} {@var{old_val} =} missing_function_hook (@var{new_val})
@deftypefnx {} {} missing_function_hook (@var{new_val}, "local")
Query or set the internal variable that specifies the function to call when
an unknown identifier is requested.

When called from inside a function with the @qcode{"local"} option, the
variable is changed locally for the function and any subroutines it calls.
The original variable value is restored when exiting the function.
@seealso{missing_component_hook}
@end deftypefn */)
{
  return SET_INTERNAL_VARIABLE (missing_function_hook);
}

void
maybe_missing_function_hook (const std::string& name)
{
  // Don't do this if we're handling errors.
  if (buffer_error_messages == 0 && ! Vmissing_function_hook.empty ())
    {
      octave::symbol_table& symtab
        = octave::__get_symbol_table__ ("maybe_missing_function_hook");

      octave_value val = symtab.find_function (Vmissing_function_hook);

      if (val.is_defined ())
        {
          // Ensure auto-restoration.
          octave::unwind_protect frame;
          frame.protect_var (Vmissing_function_hook);

          // Clear the variable prior to calling the function.
          const std::string func_name = Vmissing_function_hook;
          Vmissing_function_hook.clear ();

          // Call.
          octave::feval (func_name, octave_value (name));
        }
    }
}

DEFMETHOD (__varval__, interp, args, ,
           doc: /* -*- texinfo -*-
@deftypefn {} {} __varval__ (@var{name})
Return the value of the variable @var{name} directly from the symbol table.
@end deftypefn */)
{
  if (args.length () != 1)
    print_usage ();

  std::string name = args(0).xstring_value ("__varval__: first argument must be a variable name");

  octave::symbol_scope scope = interp.get_current_scope ();

  return scope ? scope.varval (args(0).string_value ()) : octave_value ();
}

static std::string Vmissing_component_hook;

DEFUN (missing_component_hook, args, nargout,
       doc: /* -*- texinfo -*-
@deftypefn  {} {@var{val} =} missing_component_hook ()
@deftypefnx {} {@var{old_val} =} missing_component_hook (@var{new_val})
@deftypefnx {} {} missing_component_hook (@var{new_val}, "local")
Query or set the internal variable that specifies the function to call when
a component of Octave is missing.

This can be useful for packagers that may split the Octave installation into
multiple sub-packages, for example, to provide a hint to users for how to
install the missing components.

When called from inside a function with the @qcode{"local"} option, the
variable is changed locally for the function and any subroutines it calls.
The original variable value is restored when exiting the function.

The hook function is expected to be of the form

@example
@var{fcn} (@var{component})
@end example

Octave will call @var{fcn} with the name of the function that requires the
component and a string describing the missing component.  The hook function
should return an error message to be displayed.
@seealso{missing_function_hook}
@end deftypefn */)
{
  return SET_INTERNAL_VARIABLE (missing_component_hook);
}

// The following functions are deprecated.

void
mlock (void)
{
  octave::interpreter& interp = octave::__get_interpreter__ ("mlock");

  interp.mlock ();
}

void
munlock (const std::string& nm)
{
  octave::interpreter& interp = octave::__get_interpreter__ ("mlock");

  return interp.munlock (nm);
}

bool
mislocked (const std::string& nm)
{
  octave::interpreter& interp = octave::__get_interpreter__ ("mlock");

  return interp.mislocked (nm);
}

void
bind_ans (const octave_value& val, bool print)
{
  octave::tree_evaluator& tw = octave::__get_evaluator__ ("bind_ans");

  tw.bind_ans (val, print);
}

void
clear_mex_functions (void)
{
  octave::symbol_table& symtab =
    octave::__get_symbol_table__ ("clear_mex_functions");

  symtab.clear_mex_functions ();
}

void
clear_function (const std::string& nm)
{
  octave::symbol_table& symtab = octave::__get_symbol_table__ ("clear_function");

  symtab.clear_function (nm);
}

void
clear_variable (const std::string& nm)
{
  octave::symbol_scope scope
    = octave::__get_current_scope__ ("clear_variable");

  if (scope)
    scope.clear_variable (nm);
}

void
clear_symbol (const std::string& nm)
{
  octave::symbol_table& symtab = octave::__get_symbol_table__ ("clear_symbol");

  symtab.clear_symbol (nm);
}

octave_value
lookup_function_handle (const std::string& nm)
{
  octave::symbol_scope scope
    = octave::__get_current_scope__ ("lookup_function_handle");

  octave_value val = scope ? scope.varval (nm) : octave_value ();

  return val.is_function_handle () ? val : octave_value ();
}

octave_value
get_global_value (const std::string& nm, bool silent)
{
  octave::symbol_table& symtab =
    octave::__get_symbol_table__ ("get_global_value");

  octave_value val = symtab.global_varval (nm);

  if (val.is_undefined () && ! silent)
    error ("get_global_value: undefined symbol '%s'", nm.c_str ());

  return val;
}

void
set_global_value (const std::string& nm, const octave_value& val)
{
  octave::symbol_table& symtab =
    octave::__get_symbol_table__ ("set_global_value");

  symtab.global_assign (nm, val);
}

octave_value
get_top_level_value (const std::string& nm, bool silent)
{
  octave::symbol_table& symtab =
    octave::__get_symbol_table__ ("get_top_level_value");

  octave_value val = symtab.top_level_varval (nm);

  if (val.is_undefined () && ! silent)
    error ("get_top_level_value: undefined symbol '%s'", nm.c_str ());

  return val;
}

void
set_top_level_value (const std::string& nm, const octave_value& val)
{
  octave::symbol_table& symtab =
    octave::__get_symbol_table__ ("set_top_level_value");

  symtab.top_level_assign (nm, val);
}
