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

#if ! defined (octave_MouseModeActionGroup_h)
#define octave_MouseModeActionGroup_h 1

#include <QList>
#include <QObject>

#include "Figure.h"

class QAction;

namespace QtHandles
{

  class MouseModeActionGroup : public QObject
  {
    Q_OBJECT

  public:
    MouseModeActionGroup (QObject *parent = nullptr);
    ~MouseModeActionGroup (void);

    QList<QAction*> actions (void) const { return m_actions; }

    void setMode (MouseMode mode);

  signals:
    void modeChanged (MouseMode mode);

  private slots:
    void actionToggled (bool checked);

  private:
    QList<QAction*> m_actions;
    QAction *m_current;
  };

};

#endif
