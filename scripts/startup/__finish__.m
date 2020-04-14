## Copyright (C) 2008-2018 Ben Abbott
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
## @deftypefn {} {} __finish__
## Check for the existence of the function/script, @file{finish}, in the
## path or current working directory and execute it.
##
## This function is intended to be executed upon a clean exit from Octave.
## This is accomplished in the system script @file{startup/octaverc} by use of
## the built-in function @code{atexit}.
## @seealso{atexit}
## @end deftypefn

function __finish__ ()

  if (exist ("finish", "file"))
    ## Must use evalin for access to base workspace and user variables.
    ## No argument list for finish because it might be a script, not function.
    evalin ("base", "finish;");
  endif

endfunction


## No test needed for internal helper m-file.
%!assert (1)
