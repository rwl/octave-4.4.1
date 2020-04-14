/*

Copyright (C) 1999-2018 John W. Eaton

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

#if ! defined (octave_pt_cell_h)
#define octave_pt_cell_h 1

#include "octave-config.h"

#include <iosfwd>

class octave_value;
class octave_value_list;

#include "pt-mat.h"
#include "pt-walk.h"

namespace octave
{
  class symbol_scope;
  class tree_argument_list;

  // General cells.

  class tree_cell : public tree_array_list
  {
  public:

    tree_cell (tree_argument_list *row = nullptr, int l = -1, int c = -1)
      : tree_array_list (row, l, c)
    { }

    // No copying!

    tree_cell (const tree_cell&) = delete;

    tree_cell& operator = (const tree_cell&) = delete;

    ~tree_cell (void) = default;

    bool iscell (void) const { return true; }

    bool rvalue_ok (void) const { return true; }

    tree_expression * dup (symbol_scope& scope) const;

    void accept (tree_walker& tw)
    {
      tw.visit_cell (*this);
    }
  };
}

#if defined (OCTAVE_USE_DEPRECATED_FUNCTIONS)

OCTAVE_DEPRECATED (4.4, "use 'octave::tree_cell' instead")
typedef octave::tree_cell tree_cell;

#endif

#endif
