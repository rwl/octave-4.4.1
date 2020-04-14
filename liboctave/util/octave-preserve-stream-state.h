/*

Copyright (C) 1993-2018 John W. Eaton

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

#if ! defined (octave_octave_preserve_stream_state_h)
#define octave_octave_preserve_stream_state_h 1

#include "octave-config.h"

#include <ios>

namespace octave
{
  class
  preserve_stream_state
  {
  public:

    preserve_stream_state (std::ios& s)
      : stream (s), oflags (s.flags ()), oprecision (s.precision ()),
        owidth (s.width ()), ofill (s.fill ())
    { }

    ~preserve_stream_state (void)
    {
      stream.flags (oflags);
      stream.precision (oprecision);
      stream.width (owidth);
      stream.fill (ofill);
    }

  private:

    std::ios& stream;
    std::ios::fmtflags oflags;
    std::streamsize oprecision;
    int owidth;
    char ofill;
  };
}

#endif
