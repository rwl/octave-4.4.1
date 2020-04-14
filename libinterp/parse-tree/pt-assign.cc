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

#include "error.h"
#include "ov.h"
#include "pt-arg-list.h"
#include "pt-assign.h"

namespace octave
{
  // Simple assignment expressions.

  tree_simple_assignment::tree_simple_assignment (tree_expression *le,
                                                  tree_expression *re,
                                                  bool plhs, int l, int c,
                                                  octave_value::assign_op t)
    : tree_expression (l, c), m_lhs (le), m_rhs (re), m_preserve (plhs),
      m_etype (t)
  { }

  tree_simple_assignment::~tree_simple_assignment (void)
  {
    if (! m_preserve)
      delete m_lhs;

    delete m_rhs;
  }

  std::string
  tree_simple_assignment::oper (void) const
  {
    return octave_value::assign_op_as_string (m_etype);
  }

  tree_expression *
  tree_simple_assignment::dup (symbol_scope& scope) const
  {
    tree_simple_assignment *new_sa
      = new tree_simple_assignment (m_lhs ? m_lhs->dup (scope) : nullptr,
                                    m_rhs ? m_rhs->dup (scope) : nullptr,
                                    m_preserve, m_etype);

    new_sa->copy_base (*this);

    return new_sa;
  }

  // Multi-valued assignment expressions.

  tree_multi_assignment::tree_multi_assignment (tree_argument_list *lst,
                                                tree_expression *r,
                                                bool plhs, int l, int c)
    : tree_expression (l, c), m_lhs (lst), m_rhs (r), m_preserve (plhs)
  { }

  tree_multi_assignment::~tree_multi_assignment (void)
  {
    if (! m_preserve)
      delete m_lhs;

    delete m_rhs;
  }

  std::string
  tree_multi_assignment::oper (void) const
  {
    return octave_value::assign_op_as_string (op_type ());
  }

  tree_expression *
  tree_multi_assignment::dup (symbol_scope&) const
  {
    panic_impossible ();
    return nullptr;
  }
}

/*
%!function varargout = f ()
%!  varargout{1} = nargout;
%!endfunction
%!
%!test
%! [a, ~] = f ();
%! assert (a, 2);
%!test
%! [a, ~, ~, ~, ~] = f ();
%! assert (a, 5);
*/
