## Copyright (C) 2003-2018 John W. Eaton
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
## @deftypefn  {} {@var{fname} =} tmpnam ()
## @deftypefnx {} {@var{fname} =} tmpnam (@var{dir})
## @deftypefnx {} {@var{fname} =} tmpnam (@var{dir}, @var{prefix})
## @code{tmpnam} is deprecated and will be removed in Octave version 6.
## Use @code{tempname} instead.
##
## Return a unique temporary filename as a string.
##
## If @var{prefix} is omitted, a value of @qcode{"oct-"} is used.
##
## If @var{dir} is also omitted, the default directory for temporary files
## (@code{P_tmpdir} is used.  If @var{dir} is provided, it must exist,
## otherwise the default directory for temporary files is used.
##
## Programming Note: Because the named file is not opened by @code{tmpnam},
## it is possible, though relatively unlikely, that it will not be available
## by the time your program attempts to open it.  If this is a concern,
## see @code{tmpfile}.  The functions @code{tmpnam} and @code{tempname} are
## equivalent with the latter provided for @sc{matlab} compatibility.
##
## @strong{Caution}: @code{tmpnam} will be removed in Octave version 6, or
## whatever the version number is two releases from 4.4. Use the equivalent
## @code{tempname} in all new code.
## @seealso{tempname, mkstemp, tempdir, P_tmpdir, tmpfile}
## @end deftypefn

function filename = tmpnam (varargin)

  persistent warned = false;
  if (! warned)
    warned = true;
    warning ("Octave:deprecated-function",
             "tmpnam is obsolete and will be removed from a future version of Octave, please use tempname instead");
  endif

  filename = tempname (varargin{:});

endfunction


## No tests needed for alias.
%!assert (1)
