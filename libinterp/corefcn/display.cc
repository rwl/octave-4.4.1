/*

Copyright (C) 2009-2018 John W. Eaton

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

#include "singleton-cleanup.h"

#include "cdisplay.h"
#include "defun.h"
#include "display.h"
#include "error.h"
#include "ov.h"
#include "ovl.h"

namespace octave
{
  display_info *display_info::instance = nullptr;

  void
  display_info::init (const std::string& dpy_name, bool query)
  {
    if (query)
      {
        int avail = 0;

        const char *display_name
          = dpy_name.empty () ? nullptr : dpy_name.c_str ();

        const char *msg
          = octave_get_display_info (display_name, &m_ht, &m_wd, &m_dp,
                                     &m_rx, &m_ry, &avail);

        m_dpy_avail = avail;

        if (msg)
          m_err_msg = msg;
      }
  }

  bool
  display_info::instance_ok (bool query)
  {
    bool retval = true;

    if (! instance)
      {
        instance = new display_info (query);

        if (instance)
          singleton_cleanup_list::add (cleanup_instance);
      }

    if (! instance)
      error ("unable to create display_info object!");

    return retval;
  }
}

DEFUN (have_window_system, , ,
       doc: /* -*- texinfo -*-
@deftypefn {} {} have_window_system ()
Return true if a window system is available (X11, Windows, or Apple OS X)
and false otherwise.
@seealso{isguirunning}
@end deftypefn */)
{
  return ovl (octave::display_info::display_available ());
}
