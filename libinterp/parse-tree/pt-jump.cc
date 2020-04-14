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

#include "pt-jump.h"

class octave_value_list;

namespace octave
{
  // Break.

  // Nonzero means we're breaking out of a loop or function body.
  int tree_break_command::breaking = 0;

  // Continue.

  // Nonzero means we're jumping to the end of a loop.
  int tree_continue_command::continuing = 0;

  // Return.

  // Nonzero means we're returning from a function.
  int tree_return_command::returning = 0;
}
