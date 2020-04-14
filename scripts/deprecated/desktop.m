## Copyright (C) 2013-2018 John W. Eaton
##
## This file is part of Octave.
##
## Octave is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, see
## <https://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn {} {@var{used} =} desktop ("-inuse")
##
## @code{desktop} is deprecated and will be removed in Octave version 6.
## Use @code{isguirunning} for the equivalent functionality.
##
## Return true if the desktop (GUI) is currently in use.
## @seealso{isguirunning}
## @end deftypefn

## Deprecated in version 4.4

function retval = desktop (arg)

  persistent warned = false;
  if (! warned)
    warned = true;
    warning ("Octave:deprecated-function",
             "desktop is obsolete and will be removed from a future version of Octave, please use isguirunning instead");
  endif

  if (nargin == 0)
    if (isguirunning ())
      return;  # desktop() is a NOP when GUI running
    else
      print_usage ();
    endif
  elseif (nargin > 1)
    error ('desktop: only one argument, "-inuse", is allowed');
  endif

  switch (tolower (arg))
    case "-inuse"
      retval = isguirunning ();
    otherwise
      print_usage ();
  endswitch

endfunction


## Test input validation
%!error <only one argument, "-inuse", is allowed> desktop (1,2)
%!error desktop ("-invalid_option")
