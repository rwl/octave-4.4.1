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

#include <stdlib.h>

#if defined (OCTAVE_USE_WINDOWS_API)
#include <windows.h>
#elif defined (HAVE_FRAMEWORK_CARBON)
#include <Carbon/Carbon.h>
#elif defined (HAVE_X_WINDOWS)
#include <X11/Xlib.h>
#endif

#include "cdisplay.h"

// Programming Note: This file exists so that we can hide system
// header files that make heavy use of macros and C-style casts in a C
// language file and avoid warnings about using old-style casts in C++.
// Additionally, on OS X systems, including the Carbon.h header file
// results in the declaration of a "panic" function that conflicts with
// Octave's global panic function, so Carbon.h can't be included in any
// file that also includes Octave's error.h header file.

// Please do NOT eliminate this file and move code from here to
// display.cc.

const char *
octave_get_display_info (const char *dpy_name, int *ht, int *wd, int *dp,
                         double *rx, double *ry, int *dpy_avail)
{
  const char *msg = NULL;

  *dpy_avail = 0;

#if defined (OCTAVE_USE_WINDOWS_API)

  octave_unused_parameter (dpy_name);

  HDC hdc = GetDC (0);

  if (hdc)
    {
      *dp = GetDeviceCaps (hdc, BITSPIXEL);

      *ht = GetDeviceCaps (hdc, VERTRES);
      *wd = GetDeviceCaps (hdc, HORZRES);

      double ht_mm = GetDeviceCaps (hdc, VERTSIZE);
      double wd_mm = GetDeviceCaps (hdc, HORZSIZE);

      *rx = *wd * 25.4 / wd_mm;
      *ry = *ht * 25.4 / ht_mm;

      *dpy_avail = 1;
    }
  else
    msg = "no graphical display found";

#elif defined (HAVE_FRAMEWORK_CARBON)

  octave_unused_parameter (dpy_name);

  CGDirectDisplayID display = CGMainDisplayID ();

  if (display)
    {
#if defined (HAVE_CARBON_CGDISPLAYBITSPERPIXEL)

      *dp = CGDisplayBitsPerPixel (display);

#else

      /* FIXME: This will only work for MacOS > 10.5.  For earlier versions
         this code is not needed (use CGDisplayBitsPerPixel instead).  */

      CGDisplayModeRef mode = CGDisplayCopyDisplayMode (display);
      CFStringRef pixelEncoding = CGDisplayModeCopyPixelEncoding (mode);

      if (CFStringCompare (pixelEncoding, CFSTR (IO32BitDirectPixels), 0) == 0)
        *dp = 32;
      else if (CFStringCompare (pixelEncoding,
                                CFSTR (IO16BitDirectPixels), 0) == 0)
        *dp = 16;
      else
        *dp = 8;

#endif

      *ht = CGDisplayPixelsHigh (display);
      *wd = CGDisplayPixelsWide (display);

      CGSize sz_mm = CGDisplayScreenSize (display);

      /* For MacOS >= 10.6, CGSize is a struct keeping 2 CGFloat
         values, but the CGFloat typedef is not present on older
         systems, so use double instead.  */

      double ht_mm = sz_mm.height;
      double wd_mm = sz_mm.width;

      *rx = *wd * 25.4 / wd_mm;
      *ry = *ht * 25.4 / ht_mm;

      *dpy_avail = 1;
    }
  else
    msg = "no graphical display found";

#elif defined (HAVE_X_WINDOWS)

  /* If dpy_name is NULL, XopenDisplay will look for DISPLAY in the
     environment.  */

  Display *display = XOpenDisplay (dpy_name);

  if (display)
    {
      Screen *screen = DefaultScreenOfDisplay (display);

      if (screen)
        {
          *dp = DefaultDepthOfScreen (screen);

          *ht = HeightOfScreen (screen);
          *wd = WidthOfScreen (screen);

          int screen_number = XScreenNumberOfScreen (screen);

          double ht_mm = DisplayHeightMM (display, screen_number);
          double wd_mm = DisplayWidthMM (display, screen_number);

          *rx = *wd * 25.4 / wd_mm;
          *ry = *ht * 25.4 / ht_mm;
        }
      else
        msg = "X11 display has no default screen";

      XCloseDisplay (display);

      *dpy_avail = 1;
    }
  else
    msg = "unable to open X11 DISPLAY";

#else

  octave_unused_parameter (dpy_name);
  octave_unused_parameter (ht);
  octave_unused_parameter (wd);
  octave_unused_parameter (dp);
  octave_unused_parameter (rx);
  octave_unused_parameter (ry);

  msg = "no graphical display found";

#endif

  return msg;
}
