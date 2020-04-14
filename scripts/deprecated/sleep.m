## Copyright (C) 1993-2018 John W. Eaton
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
## @deftypefn {} {} sleep (@var{seconds})
##
## @code{sleep} is deprecated and will be removed in Octave version 5.
## Use @code{pause} instead.
##
## Suspend the execution of the program for the given number of seconds.
##
## @seealso{pause}
## @end deftypefn

function sleep (seconds)

  persistent warned = false;
  if (! warned)
    warned = true;
    warning ("Octave:deprecated-function",
             "sleep is obsolete and will be removed from a future version of Octave, please use pause instead");
  endif

  if (nargin == 1)
    pause (seconds);
  else
    print_usage ();
  endif

endfunction


%!test
%! sleep (1);

%!error (sleep ())
%!error (sleep (1, 2))
