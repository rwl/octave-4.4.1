/*

Copyright (C) 2012-2018 John W. Eaton

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

#include "ov-fcn.h"
#include "pt-funcall.h"
#include "pt-walk.h"

namespace octave
{
  // Function call objects.

  void
  tree_funcall::print (std::ostream& os, bool pr_as_read_syntax,
                       bool pr_orig_text)
  {
    print_raw (os, pr_as_read_syntax, pr_orig_text);
  }

  void
  tree_funcall::print_raw (std::ostream& os, bool pr_as_read_syntax,
                           bool pr_orig_text)
  {
    if (pr_orig_text)
      {
        os << original_text ();
      }
    else
      {
        octave_function *fp = m_fcn.function_value ();
        std::string nm = (fp ? fp->name () : "<invalid-function>");

        os << nm << " (";

        octave_idx_type n = m_args.length ();
        for (octave_idx_type i = 0; i < n; i++)
          {
            m_args(i).print_raw (os, pr_as_read_syntax);

            if (i < n - 1)
              os << ", ";
          }

        os << ')';
      }
  }

  tree_funcall *
  tree_funcall::dup (symbol_scope&) const
  {
    tree_funcall *new_fc
      = new tree_funcall (m_fcn, m_args, line (), column ());

    new_fc->copy_base (*new_fc);

    return new_fc;
  }
}
