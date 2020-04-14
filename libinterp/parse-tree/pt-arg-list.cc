/*

Copyright (C) 1996-2018 John W. Eaton

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

#include <iostream>
#include <string>

#include "str-vec.h"

#include "defun.h"
#include "error.h"
#include "ovl.h"
#include "ov.h"
#include "ov-usr-fcn.h"
#include "parse.h"
#include "pt-arg-list.h"
#include "pt-eval.h"
#include "pt-exp.h"
#include "pt-id.h"
#include "pt-idx.h"
#include "pt-pr-code.h"
#include "pt-walk.h"
#include "interpreter.h"
#include "unwind-prot.h"

namespace octave
{
  // Argument lists.

  tree_argument_list::~tree_argument_list (void)
  {
    while (! empty ())
      {
        iterator p = begin ();
        delete *p;
        erase (p);
      }
  }

  bool
  tree_argument_list::has_magic_end (void) const
  {
    for (const tree_expression *elt : *this)
      {
        if (elt && ! elt->is_index_expression () && elt->has_magic_end ())
          return true;
      }

    return false;
  }

  void
  tree_argument_list::append (const element_type& s)
  {
    base_list<tree_expression *>::append (s);

    if (! m_list_includes_magic_end && s && s->has_magic_end ())
      m_list_includes_magic_end = true;

    if (! m_list_includes_magic_tilde && s && s->is_identifier ())
      {
        tree_identifier *id = dynamic_cast<tree_identifier *> (s);
        m_list_includes_magic_tilde = id && id->is_black_hole ();
      }
  }

  bool
  tree_argument_list::all_elements_are_constant (void) const
  {
    for (const tree_expression *elt : *this)
      {
        if (! elt->is_constant ())
          return false;
      }

    return true;
  }

  bool
  tree_argument_list::is_valid_lvalue_list (void) const
  {
    bool retval = true;

    for (const tree_expression *elt : *this)
      {
        // There is no need for a separate check for the magic "~" because it
        // is represented by tree_black_hole, and that is derived from
        // tree_identifier.
        if (! (elt->is_identifier () || elt->is_index_expression ()))
          {
            retval = false;
            break;
          }
      }

    return retval;
  }
}

// FIXME: Is there a way to do this job without global data?

static const octave_value *indexed_object = nullptr;
static int index_position = 0;
static int num_indices = 0;

// END is documented in op-kw-docs.
DEFCONSTMETHOD (end, interp, , ,
                doc: /* -*- texinfo -*-
@deftypefn {} {} end
Last element of an array or the end of any @code{for}, @code{parfor},
@code{if}, @code{do}, @code{while}, @code{function}, @code{switch},
@code{try}, or @code{unwind_protect} block.

As an index of an array, the magic index @qcode{"end"} refers to the
last valid entry in an indexing operation.

Example:

@example
@group
@var{x} = [ 1 2 3; 4 5 6 ];
@var{x}(1,end)
   @result{} 3
@var{x}(end,1)
   @result{} 4
@var{x}(end,end)
   @result{} 6
@end group
@end example
@seealso{for, parfor, if, do, while, function, switch, try, unwind_protect}
@end deftypefn */)
{
  octave_value retval;

  if (! indexed_object)
    error ("invalid use of end");

  if (indexed_object->isobject ())
    {
      octave_value_list args;

      args(2) = num_indices;
      args(1) = index_position + 1;
      args(0) = *indexed_object;

      std::string class_name = indexed_object->class_name ();

      octave::symbol_table& symtab = interp.get_symbol_table ();

      octave_value meth = symtab.find_method ("end", class_name);

      if (meth.is_defined ())
        return octave::feval (meth.function_value (), args, 1);
    }

  dim_vector dv = indexed_object->dims ();
  int ndims = dv.ndims ();

  if (num_indices < ndims)
    {
      for (int i = num_indices; i < ndims; i++)
        dv(num_indices-1) *= dv(i);

      if (num_indices == 1)
        {
          ndims = 2;
          dv.resize (ndims);
          dv(1) = 1;
        }
      else
        {
          ndims = num_indices;
          dv.resize (ndims);
        }
    }

  if (index_position < ndims)
    retval = dv(index_position);
  else
    retval = 1;

  return retval;
}

namespace octave
{
  octave_value_list
  tree_argument_list::convert_to_const_vector (tree_evaluator *tw,
                                               const octave_value *object)
  {
    // END doesn't make sense for functions.  Maybe we need a different
    // way of asking an octave_value object this question?

    bool stash_object = (m_list_includes_magic_end
                         && object
                         && ! (object->is_function ()
                               || object->is_function_handle ()));

    unwind_protect frame;

    if (stash_object)
      {
        frame.protect_var (indexed_object);

        indexed_object = object;
      }

    int len = length ();

    std::list<octave_value_list> args;

    iterator p = begin ();
    for (int k = 0; k < len; k++)
      {
        if (stash_object)
          {
            frame.protect_var (index_position);
            frame.protect_var (num_indices);

            index_position = k;
            num_indices = len;
          }

        tree_expression *elt = *p++;

        if (elt)
          {
            octave_value tmp = tw->evaluate (elt);

            if (tmp.is_cs_list ())
              args.push_back (tmp.list_value ());
            else if (tmp.is_defined ())
              args.push_back (tmp);
          }
        else
          {
            args.push_back (octave_value ());
            break;
          }
      }

    return args;
  }

  string_vector
  tree_argument_list::get_arg_names (void) const
  {
    int len = length ();

    string_vector retval (len);

    int k = 0;

    for (tree_expression *elt : *this)
      retval(k++) = elt->str_print_code ();

    return retval;
  }

  std::list<std::string>
  tree_argument_list::variable_names (void) const
  {
    std::list<std::string> retval;

    for (tree_expression *elt : *this)
      {
        if (elt->is_identifier ())
          {
            tree_identifier *id = dynamic_cast<tree_identifier *> (elt);

            retval.push_back (id->name ());
          }
        else if (elt->is_index_expression ())
          {
            tree_index_expression *idx_expr
              = dynamic_cast<tree_index_expression *> (elt);

            retval.push_back (idx_expr->name ());
          }
      }

    return retval;
  }

  tree_argument_list *
  tree_argument_list::dup (symbol_scope& scope) const
  {
    tree_argument_list *new_list = new tree_argument_list ();

    new_list->m_list_includes_magic_end = m_list_includes_magic_end;
    new_list->m_simple_assign_lhs = m_simple_assign_lhs;

    for (const tree_expression *elt : *this)
      new_list->append (elt ? elt->dup (scope) : nullptr);

    return new_list;
  }
}
