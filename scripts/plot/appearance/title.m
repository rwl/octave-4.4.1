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
## @deftypefn  {} {} title (@var{string})
## @deftypefnx {} {} title (@var{string}, @var{prop}, @var{val}, @dots{})
## @deftypefnx {} {} title (@var{hax}, @dots{})
## @deftypefnx {} {@var{h} =} title (@dots{})
## Specify the string used as a title for the current axis.
##
## An optional list of @var{property}/@var{value} pairs can be used to change
## the appearance of the created title text object.
##
## If the first argument @var{hax} is an axes handle, then plot into this axes,
## rather than the current axes returned by @code{gca}.
##
## The optional return value @var{h} is a graphics handle to the created text
## object.
## @seealso{xlabel, ylabel, zlabel, text}
## @end deftypefn

## Author: jwe

function h = title (varargin)

  [hax, varargin, nargin] = __plt_get_axis_arg__ ("title", varargin{:});

  if (isempty (hax))
    hax = gca ();
  endif

  if (rem (nargin, 2) != 1)
    print_usage ();
  endif

  htmp = __axis_label__ (hax, "title", varargin{:});

  if (nargout > 0)
    h = htmp;
  endif

endfunction


%!demo
%! clf;
%! title ("Test Title Text");

%!demo
%! clf;
%! title ({"Multi-line"; "Title"; "Text"});

%!demo
%! clf;
%! plot3 ([0,1], [0,1], [0,1]);
%! title ("Test FontSize Property", "fontsize", 16);

%!test
%! hf = figure ("visible", "off");
%! unwind_protect
%!   title ("Test Title Text");
%!   h = get (gca, "title");
%!   assert (get (h, "string"), "Test Title Text");
%! unwind_protect_cleanup
%!   close (hf);
%! end_unwind_protect

%!test
%! hf = figure ("visible", "off");
%! unwind_protect
%!   title ({"Multi-line"; "Title"; "Text"});
%!   h = get (gca, "title");
%!   assert (get (h, "string"), {"Multi-line"; "Title"; "Text"});
%! unwind_protect_cleanup
%!   close (hf);
%! end_unwind_protect

%!test
%! hf = figure ("visible", "off");
%! unwind_protect
%!   set (gca, "fontsize", 5, "titlefontsizemultiplier", 3);
%!   ht = title ("title_string", "color", "r");
%!   assert (get (ht, "fontsize"), 15);
%!   assert (get (ht, "color"), [1 0 0]);
%! unwind_protect_cleanup
%!   close (hf);
%! end_unwind_protect

%!test
%! hf = figure ("visible", "off");
%! unwind_protect
%!   plot3 ([0,1], [0,1], [0,1]);
%!   title ("Test FontSize Property", "fontsize", 16);
%!   h = get (gca, "title");
%!   assert (get (h, "string"), "Test FontSize Property");
%!   assert (get (h, "fontsize"), 16);
%! unwind_protect_cleanup
%!   close (hf);
%! end_unwind_protect

%!test <*49469>
%! hf = figure ("visible", "off");
%! unwind_protect
%!   ht = title ("Test FontSize Property");
%!   set (gca, "fontname", "Liberation Serif")
%!   set (gca, "fontsize", 13)
%!   assert (get (ht, "fontname"), "Liberation Serif");
%!   assert (get (ht, "fontsize"), 13 * get (gca, "titlefontsizemultiplier"));
%! unwind_protect_cleanup
%!   close (hf);
%! end_unwind_protect
