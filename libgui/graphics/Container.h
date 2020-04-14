/*

Copyright (C) 2011-2018 Michael Goffioul

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

#if ! defined (octave_Container_h)
#define octave_Container_h 1

#include <QWidget>

#include "GenericEventNotify.h"

#include "graphics.h"

namespace QtHandles
{

  DECLARE_GENERICEVENTNOTIFY_SENDER(ContainerBase, QWidget);

  class Canvas;

  class Container : public ContainerBase
  {
  public:
    Container (QWidget *parent);
    ~Container (void);

    Canvas * canvas (const graphics_handle& handle, bool create = true);

  protected:
    void childEvent (QChildEvent *event);
    void resizeEvent (QResizeEvent *event);

  private:
    Canvas *m_canvas;
  };

}

#endif
