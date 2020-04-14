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

#if defined (HAVE_CONFIG_H)
#  include "config.h"
#endif

#include <QAction>
#include <QActionEvent>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QImage>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QtDebug>
#include <QTimer>
#include <QToolBar>

#include "Canvas.h"
#include "Container.h"
#include "Figure.h"
#include "FigureWindow.h"
#include "MouseModeActionGroup.h"
#include "QtHandlesUtils.h"

#include "file-ops.h"
#include "unwind-prot.h"
#include "utils.h"
#include "version.h"

#include "octave-qt-link.h"

#include "builtin-defun-decls.h"

namespace QtHandles
{

  DECLARE_GENERICEVENTNOTIFY_SENDER(MenuBar, QMenuBar);

  static bool
  hasUiControlChildren (const figure::properties& fp)
  {
    gh_manager::auto_lock lock;

    Matrix kids = fp.get_all_children ();

    for (int i = 0; i < kids.numel (); i++)
      {
        graphics_object go (gh_manager::get_object (kids(i)));

        if (go && (go.isa ("uicontrol") || go.isa ("uipanel")
                   || go.isa ("uibuttongroup")))
          return true;
      }

    return false;
  }

  static bool
  hasUiMenuChildren (const figure::properties& fp)
  {
    gh_manager::auto_lock lock;

    Matrix kids = fp.get_all_children ();

    for (int i = 0; i < kids.numel (); i++)
      {
        graphics_object go (gh_manager::get_object (kids(i)));

        if (go && go.isa ("uimenu") &&
            go.get ("visible").string_value () == "on")
          return true;
      }

    return false;
  }

  static QRect
  boundingBoxToRect (const Matrix& bb)
  {
    QRect r;

    if (bb.numel () == 4)
      {
        r = QRect (octave::math::round (bb(0)), octave::math::round (bb(1)),
                   octave::math::round (bb(2)), octave::math::round (bb(3)));
        if (! r.isValid ())
          r = QRect ();
      }

    return r;
  }

  Figure*
  Figure::create (const graphics_object& go)
  {
    return new Figure (go, new FigureWindow ());
  }

  Figure::Figure (const graphics_object& go, FigureWindow *win)
    : Object (go, win), m_blockUpdates (false), m_figureToolBar (nullptr),
      m_menuBar (nullptr), m_innerRect (), m_outerRect (),
      m_mouseModeGroup (nullptr)
  {
    m_container = new Container (win);
    win->setCentralWidget (m_container);

    figure::properties& fp = properties<figure> ();

    // Status bar
    m_statusBar = win->statusBar ();
    int boffset = 0;

    // Toolbar and menubar
    createFigureToolBarAndMenuBar ();
    int toffset = 0;

    if (fp.toolbar_is ("figure") ||
        (fp.toolbar_is ("auto") && fp.menubar_is ("figure") &&
         ! hasUiControlChildren (fp)))
      {
        toffset += m_figureToolBar->sizeHint ().height ();
        boffset += m_statusBar->sizeHint ().height ();
      }
    else
      {
        m_figureToolBar->hide ();
        m_statusBar->hide ();
      }

    if (fp.menubar_is ("figure") || hasUiMenuChildren (fp))
      toffset += m_menuBar->sizeHint ().height ();
    else
      m_menuBar->hide ();

    m_innerRect = boundingBoxToRect (fp.get_boundingbox (true));
    m_outerRect = boundingBoxToRect (fp.get_boundingbox (false));

    win->setGeometry (m_innerRect.adjusted (0, -toffset, 0, boffset));

    // Enable mouse tracking unconditionally
    enableMouseTracking ();

    // When this constructor gets called all properties are already
    // set, even non default. We force "update" here to get things right.

    // Figure title
    update (figure::properties::ID_NUMBERTITLE);

    // Decide what keyboard events we listen to
    m_container->canvas (m_handle)->setEventMask (0);
    update (figure::properties::ID_KEYPRESSFCN);
    update (figure::properties::ID_KEYRELEASEFCN);

    // modal style
    update (figure::properties::ID_WINDOWSTYLE);

    // Visibility
    update (figure::properties::ID_VISIBLE);

    connect (this, SIGNAL (asyncUpdate (void)),
             this, SLOT (updateContainer (void)));

    win->addReceiver (this);
    m_container->addReceiver (this);
  }

  Figure::~Figure (void)
  { }

  static std::string
  mouse_mode_to_string (MouseMode mode)
  {
    switch (mode)
      {
      case NoMode:
        return "none";

      case RotateMode:
        return "rotate";

      case ZoomInMode:
        return "zoom in";

      case ZoomOutMode:
        return "zoom out";

      case PanMode:
        return "pan";

      case TextMode:
        return "text";

      case SelectMode:
        return "select";

      default:
        break;
      }

    return "none";
  }

  static MouseMode
  mouse_mode_from_string (const std::string& mode)
  {
    if (mode == "none")
      return NoMode;
    else if (mode == "rotate")
      return RotateMode;
    else if (mode == "zoom in")
      return ZoomInMode;
    else if (mode == "zoom out")
      return ZoomOutMode;
    else if (mode == "pan")
      return PanMode;
    else if (mode == "text")
      return TextMode;
    else if (mode == "select")
      return SelectMode;
    else
      return NoMode;
  }

  QString
  Figure::fileName (void)
  {
    gh_manager::auto_lock lock;

    const figure::properties& fp = properties<figure> ();

    std::string name = fp.get_filename ();

    return QString::fromStdString (name);
  }

  void
  Figure::setFileName (const QString& name)
  {
    gh_manager::auto_lock lock;

    figure::properties& fp = properties<figure> ();

    fp.set_filename (name.toStdString ());
  }

  MouseMode
  Figure::mouseMode (void)
  {
    gh_manager::auto_lock lock;

    const figure::properties& fp = properties<figure> ();

    std::string mode = fp.get___mouse_mode__ ();

    if (mode == "zoom")
      {
        octave_scalar_map zm = fp.get___zoom_mode__ ().scalar_map_value ();

        std::string direction = zm.getfield ("Direction").string_value ();

        mode += ' ' + direction;
      }

    return mouse_mode_from_string (mode);
  }

  void
  Figure::createFigureToolBarAndMenuBar (void)
  {
    QMainWindow *win = qWidget<QMainWindow> ();

    m_figureToolBar = win->addToolBar (tr ("Figure ToolBar"));
    m_figureToolBar->setMovable (false);
    m_figureToolBar->setFloatable (false);

    m_mouseModeGroup = new MouseModeActionGroup (win);
    connect (m_mouseModeGroup, SIGNAL (modeChanged (MouseMode)),
             SLOT (setMouseMode (MouseMode)));
    m_figureToolBar->addActions (m_mouseModeGroup->actions ());

    QAction *toggle_axes = m_figureToolBar->addAction (tr ("Axes"));
    connect (toggle_axes, SIGNAL (triggered (void)),
             this, SLOT (toggleAxes (void)));

    QAction *toggle_grid = m_figureToolBar->addAction (tr ("Grid"));
    connect (toggle_grid, SIGNAL (triggered (void)),
             this, SLOT (toggleGrid (void)));

    QAction *auto_axes = m_figureToolBar->addAction (tr ("Autoscale"));
    connect (auto_axes, SIGNAL (triggered (void)),
             this, SLOT (autoAxes (void)));

    m_menuBar = new MenuBar (win);
    win->setMenuBar (m_menuBar);

    QMenu *fileMenu = m_menuBar->addMenu (tr ("&File"));
    fileMenu->menuAction ()->setObjectName ("builtinMenu");
    fileMenu->addAction (tr ("&Save"), this, SLOT (fileSaveFigure (bool)));
    fileMenu->addAction (tr ("Save &As"), this, SLOT (fileSaveFigureAs (void)));
    fileMenu->addSeparator ();
    fileMenu->addAction (tr ("&Close Figure"), this,
                         SLOT (fileCloseFigure (void)), Qt::CTRL | Qt::Key_W);

    QMenu *editMenu = m_menuBar->addMenu (tr ("&Edit"));
    editMenu->menuAction ()->setObjectName ("builtinMenu");
    editMenu->addAction (tr ("Cop&y"), this, SLOT (editCopy (bool)),
                         Qt::CTRL | Qt::Key_C);
    editMenu->addSeparator ();
    editMenu->addActions (m_mouseModeGroup->actions ());

    QMenu *helpMenu = m_menuBar->addMenu (tr ("&Help"));
    helpMenu->menuAction ()->setObjectName ("builtinMenu");
    helpMenu->addAction (tr ("About Octave"), this,
                         SLOT (helpAboutOctave (void)));

    m_menuBar->addReceiver (this);
  }

  void
  Figure::updateFigureToolBarAndMenuBar (void)
  {
    if (m_mouseModeGroup)
      {
        m_blockUpdates = true;
        m_mouseModeGroup->setMode (mouseMode ());
        m_blockUpdates = false;
      }
  }

  Container*
  Figure::innerContainer (void)
  {
    return m_container;
  }

  void
  Figure::redraw (void)
  {
    Canvas *canvas = m_container->canvas (m_handle);

    if (canvas)
      {
        canvas->redraw ();
        //canvas->setMouseMode (RotateMode);
      }

    foreach (QFrame *frame,
             qWidget<QWidget> ()->findChildren<QFrame*> ())
      {
        if (frame->objectName () == "UIPanel"
            || frame->objectName () == "UIButtonGroup")
          {
            Object *obj = Object::fromQObject (frame);

            if (obj)
              obj->slotRedraw ();
          }
      }

    updateFigureToolBarAndMenuBar ();
  }

  void
  Figure::print (const QString& file_cmd, const QString& term)
  {
    Canvas *canvas = m_container->canvas (m_handle);

    if (canvas)
      canvas->print (file_cmd, term);
  }

  uint8NDArray
  Figure::slotGetPixels (void)
  {
    uint8NDArray retval;
    Canvas *canvas = m_container->canvas (m_handle);

    if (canvas)
      {
        gh_manager::process_events ();
        gh_manager::auto_lock lock;
        retval = canvas->getPixels ();
      }

    return retval;
  }

  void
  Figure::beingDeleted (void)
  {
    Canvas *canvas = m_container->canvas (m_handle.value (), false);

    if (canvas)
      canvas->blockRedraw (true);

    m_menuBar->removeReceiver (this);
    m_container->removeReceiver (this);
    qWidget<FigureWindow> ()->removeReceiver (this);
  }

  void
  Figure::update (int pId)
  {
    if (m_blockUpdates)
      return;

    figure::properties& fp = properties<figure> ();

    if (fp.is___printing__ ())
      return;

    QMainWindow *win = qWidget<QMainWindow> ();

    // If the window doesn't exist, there's nothing we can do.
    if (! win)
      return;

    m_blockUpdates = true;

    switch (pId)
      {
      case figure::properties::ID_POSITION:
        {
          m_innerRect = boundingBoxToRect (fp.get_boundingbox (true));
          int toffset = 0;
          int boffset = 0;

          foreach (QToolBar *tb, win->findChildren<QToolBar*> ())
            if (! tb->isHidden ())
              toffset += tb->sizeHint ().height ();

          if (! m_menuBar->isHidden ())
            toffset += m_menuBar->sizeHint ().height ();

          if (! m_statusBar->isHidden ())
            boffset += m_statusBar->sizeHint ().height ();

          win->setGeometry (m_innerRect.adjusted (0, -toffset, 0, boffset));
        }
        break;

      case figure::properties::ID_NAME:
      case figure::properties::ID_NUMBERTITLE:
        win->setWindowTitle (Utils::fromStdString (fp.get_title ()));
        break;

      case figure::properties::ID_VISIBLE:
        if (fp.is_visible ())
          {
            QTimer::singleShot (0, win, SLOT (show ()));
            if (! fp.is___gl_window__ ())
              {
                gh_manager::auto_lock lock;
                fp.set ("__gl_window__", "on");
              }
          }
        else
          win->hide ();
        break;

      case figure::properties::ID_TOOLBAR:
        if (fp.toolbar_is ("none"))
          showFigureToolBar (false);
        else if (fp.toolbar_is ("figure"))
          showFigureToolBar (true);
        else // "auto"
          showFigureToolBar (! hasUiControlChildren (fp) &&
                             fp.menubar_is ("figure"));
        break;

      case figure::properties::ID_MENUBAR:
        showMenuBar (fp.menubar_is ("figure"));
        if (fp.toolbar_is ("auto"))
          showFigureToolBar (fp.menubar_is ("figure"));
        break;

      case figure::properties::ID_KEYPRESSFCN:
        if (fp.get_keypressfcn ().isempty ())
          m_container->canvas (m_handle)->clearEventMask (Canvas::KeyPress);
        else
          m_container->canvas (m_handle)->addEventMask (Canvas::KeyPress);
        break;

      case figure::properties::ID_KEYRELEASEFCN:
        if (fp.get_keyreleasefcn ().isempty ())
          m_container->canvas (m_handle)->clearEventMask (Canvas::KeyRelease);
        else
          m_container->canvas (m_handle)->addEventMask (Canvas::KeyRelease);
        break;

      case figure::properties::ID_WINDOWSTYLE:
        if (fp.windowstyle_is ("modal"))
          {
            bool is_visible = win->isVisible ();

            // if window is already visible, need to hide and reshow it in order to
            // make it use the modal settings
            if (is_visible)
              win->setVisible (false);

            win->setWindowModality (Qt::ApplicationModal);
            win->setVisible (is_visible);
          }
        else
          win->setWindowModality (Qt::NonModal);

        break;

      default:
        break;
      }

    m_blockUpdates = false;
  }

  void
  Figure::showFigureToolBar (bool visible)
  {
    if ((! m_figureToolBar->isHidden ()) != visible)
      {
        int dy1 = m_figureToolBar->sizeHint ().height ();
        int dy2 = m_statusBar->sizeHint ().height ();
        QRect r = qWidget<QWidget> ()->geometry ();

        if (! visible)
          r.adjust (0, dy1, 0, -dy2);
        else
          r.adjust (0, -dy1, 0, dy2);

        m_blockUpdates = true;
        qWidget<QWidget> ()->setGeometry (r);
        m_figureToolBar->setVisible (visible);
        m_statusBar->setVisible (visible);
        m_blockUpdates = false;

        updateBoundingBox (false);
      }
  }

  void
  Figure::showMenuBar (bool visible, int h1)
  {
    // Get the height before and after toggling the visibility of builtin menus
    if (h1 <= 0)
      h1 = m_menuBar->sizeHint ().height ();

    foreach (QAction *a, m_menuBar->actions ())
      if (a->objectName () == "builtinMenu")
        a->setVisible (visible);

    int h2 = m_menuBar->sizeHint ().height ();

    // Keep the menubar visible if it contains custom menus
    if (! visible)
      visible = hasUiMenuChildren (properties<figure> ());

    if ((m_menuBar->isVisible () && ! visible)
        || (! m_menuBar->isVisible () && visible))
      {
        int dy = qMax (h1, h2);
        QRect r = qWidget<QWidget> ()->geometry ();

        if (! visible)
          r.adjust (0, dy, 0, 0);
        else
          r.adjust (0, -dy, 0, 0);

        m_blockUpdates = true;
        qWidget<QWidget> ()->setGeometry (r);
        m_menuBar->setVisible (visible);
        m_blockUpdates = false;
      }
    updateBoundingBox (false);
  }

  void
  Figure::updateMenuBar (int height)
  {
    gh_manager::auto_lock lock;
    graphics_object go = object ();

    if (go.valid_object ())
      showMenuBar (Utils::properties<figure> (go).menubar_is ("figure"),
                   height);
  }

  void
  Figure::updateStatusBar (ColumnVector pt)
  {
    if (! m_statusBar->isHidden ())
      m_statusBar->showMessage (QString ("(%1, %2)")
                                .arg (pt(0), 0, 'g', 5)
                                .arg (pt(1), 0, 'g', 5));
  }

  QWidget*
  Figure::menu (void)
  {
    return qWidget<QMainWindow> ()->menuBar ();
  }

  struct UpdateBoundingBoxData
  {
    Matrix m_bbox;
    bool m_internal;
    graphics_handle m_handle;
    Figure *m_figure;
  };

  void
  Figure::updateBoundingBoxHelper (void *data)
  {
    gh_manager::auto_lock lock;

    UpdateBoundingBoxData *d = reinterpret_cast<UpdateBoundingBoxData *> (data);
    graphics_object go = gh_manager::get_object (d->m_handle);

    if (go.valid_object ())
      {
        figure::properties& fp = Utils::properties<figure> (go);

        fp.set_boundingbox (d->m_bbox, d->m_internal, false);
      }

    delete d;
  }

  void
  Figure::updateBoundingBox (bool internal, int flags)
  {
    QWidget *win = qWidget<QWidget> ();
    Matrix bb (1, 4);

    if (internal)
      {
        QRect r = m_innerRect;

        if (flags & UpdateBoundingBoxPosition)
          r.moveTopLeft (win->mapToGlobal (m_container->pos ()));
        if (flags & UpdateBoundingBoxSize)
          r.setSize (m_container->size ());

        if (r.isValid () && r != m_innerRect)
          {
            m_innerRect = r;

            bb(0) = r.x ();
            bb(1) = r.y ();
            bb(2) = r.width ();
            bb(3) = r.height ();
          }
        else
          return;
      }
    else
      {
        QRect r = m_outerRect;

        if (flags & UpdateBoundingBoxPosition)
          r.moveTopLeft (win->pos ());
        if (flags & UpdateBoundingBoxSize)
          r.setSize (win->frameGeometry ().size ());

        if (r.isValid () && r != m_outerRect)
          {
            m_outerRect = r;

            bb(0) = r.x ();
            bb(1) = r.y ();
            bb(2) = r.width ();
            bb(3) = r.height ();
          }
        else
          return;
      }

    UpdateBoundingBoxData *d = new UpdateBoundingBoxData ();

    d->m_bbox = bb;
    d->m_internal = internal;
    d->m_handle = m_handle;
    d->m_figure = this;

    gh_manager::post_function (Figure::updateBoundingBoxHelper, d);
  }

  void
  Figure::close_figure_callback (void)
  {
    figure::properties& fp = properties<figure> ();
    octave_value fnum = fp.get___myhandle__ ().as_octave_value ();

    Ffeval (ovl ("close", fnum));
  }

  bool
  Figure::eventNotifyBefore (QObject *obj, QEvent *xevent)
  {
    if (! m_blockUpdates)
      {
        // Clicking the toolbar or the menubar makes this figure current
        if (xevent->type () == QEvent::MouseButtonPress)
          {
            figure::properties& fp = properties<figure> ();
            graphics_object root = gh_manager::get_object (0);
            if (fp.get_handlevisibility () == "on")
              root.set ("currentfigure",
                        fp.get___myhandle__ ().as_octave_value ());
          }

        if (obj == m_container)
          {
            // Do nothing...
          }
        else if (obj == m_menuBar)
          {
            switch (xevent->type ())
              {
              case QEvent::ActionChanged:
                m_previousHeight = m_menuBar->sizeHint ().height ();
                break;
              case QEvent::ActionRemoved:
                {
                  QAction *a = dynamic_cast<QActionEvent *> (xevent)->action ();

                  if (! a->isSeparator ()
                      && a->objectName () != "builtinMenu")
                    updateMenuBar ();
                }
                break;

              default:
                break;
              }
          }
        else
          {
            switch (xevent->type ())
              {
              case QEvent::Close:
                xevent->ignore ();
                octave_link::post_event (this, &Figure::close_figure_callback);
                return true;

              default:
                break;
              }
          }
      }

    return false;
  }

  void
  Figure::eventNotifyAfter (QObject *watched, QEvent *xevent)
  {
    if (! m_blockUpdates)
      {
        if (watched == m_container)
          {
            switch (xevent->type ())
              {
              case QEvent::Resize:
                updateBoundingBox (true, UpdateBoundingBoxSize);
                break;

              case QEvent::ChildAdded:
                if (dynamic_cast<QChildEvent *> (xevent)->child
                    ()->isWidgetType())
                  {
                    gh_manager::auto_lock lock;
                    update (figure::properties::ID_TOOLBAR);

                    enableMouseTracking ();
                  }
                break;

              case QEvent::ChildRemoved:
                if (dynamic_cast<QChildEvent *> (xevent)->child
                    ()->isWidgetType())
                  {
                    gh_manager::auto_lock lock;
                    update (figure::properties::ID_TOOLBAR);
                  }
                break;

              default:
                break;
              }
          }
        else if (watched == m_menuBar)
          {
            switch (xevent->type ())
              {
              case QEvent::ActionChanged:
                // The menubar may have been resized if no action is visible
                {
                  QAction *a = dynamic_cast<QActionEvent *> (xevent)->action ();
                  if (m_menuBar->sizeHint ().height () != m_previousHeight
                      && a->objectName () != "builtinMenu"
                      && ! a->isSeparator ())
                    updateMenuBar (m_previousHeight);
                }
                break;
              case QEvent::ActionAdded:
                {
                  QAction *a = dynamic_cast<QActionEvent *> (xevent)->action ();

                  if (! a->isSeparator ()
                      && a->objectName () != "builtinMenu"
                      && a->isVisible ())
                    updateMenuBar ();
                }
                break;

              default:
                break;
              }
          }
        else
          {
            switch (xevent->type ())
              {
              case QEvent::Move:
                updateBoundingBox (false, UpdateBoundingBoxPosition);
                updateBoundingBox (true, UpdateBoundingBoxPosition);
                break;

              case QEvent::Resize:
                updateBoundingBox (false, UpdateBoundingBoxSize);
                break;

              default:
                break;
              }
          }
      }
  }

  void
  Figure::helpAboutOctave (void)
  {
    std::string message
      = octave_name_version_copyright_copying_warranty_and_bugs (true);

    QMessageBox::about (qWidget<QMainWindow> (), tr ("About Octave"),
                        QString::fromStdString (message));
  }

  void
  Figure::setMouseMode (MouseMode mode)
  {
    if (m_blockUpdates)
      return;

    gh_manager::auto_lock lock;

    figure::properties& fp = properties<figure> ();

    fp.set___mouse_mode__ (mouse_mode_to_string (mode));

    Canvas *canvas = m_container->canvas (m_handle);

    if (canvas)
      canvas->setCursor (mode);
  }

  void
  Figure::fileSaveFigure (bool prompt)
  {
    QString file = fileName ();

    if (file.isEmpty ())
      {
        prompt = true;

        file = "untitled.ofig";
      }

    if (prompt || file.isEmpty ())
      {
        QFileInfo finfo (file);

        file = QFileDialog::getSaveFileName (qWidget<FigureWindow> (),
                                             tr ("Save Figure As"),
                                             finfo.absoluteFilePath (),
                                             tr ("Octave Figure File (*.ofig);;Vector Image Formats (*.eps *.epsc *.pdf *.svg *.ps *.tikz);;Bitmap Image Formats (*.gif *.jpg *.png *.tiff)"),
                                             nullptr,
                                             QFileDialog::DontUseNativeDialog);
      }

    if (! file.isEmpty ())
      {
        QFileInfo finfo (file);

        setFileName (finfo.absoluteFilePath ());

        octave_link::post_event (this, &Figure::save_figure_callback,
                                 file.toStdString ());
      }
  }

  void
  Figure::save_figure_callback (const std::string& file)
  {
    figure::properties& fp = properties<figure> ();
    octave_value fnum = fp.get___myhandle__ ().as_octave_value ();

    size_t flen = file.length ();

    if (flen > 5 && file.substr (flen-5) == ".ofig")
      Ffeval (ovl ("hgsave", fnum, file));
    else
      Ffeval (ovl ("print", fnum, file));
  }

  void
  Figure::copy_figure_callback (const std::string& format)
  {
    std::string msg;

    std::string file = octave::sys::tempnam ("", "oct-", msg) + '.' + format;

    if (file.empty ())
      {
        // Report error?
        return;
      }

    save_figure_callback (file);

    octave_link::copy_image_to_clipboard (file);
  }

  void
  Figure::fileSaveFigureAs (void)
  {
    fileSaveFigure (true);
  }

  void
  Figure::fileCloseFigure (void)
  {
    qWidget<QMainWindow> ()->close ();
  }

  void
  Figure::editCopy (bool /* choose_format */)
  {
    QString format = "png";

#if 0

    // FIXME: allow choice of image formats.

    if (choose_format)
      {
        QFileInfo finfo (file);

        format = QFileDialog::getSaveFileName (qWidget<FigureWindow> (),
                                               tr ("Save Figure As"),
                                               finfo.absoluteFilePath (), 0, 0,
                                               QFileDialog::DontUseNativeDialog);
      }
#endif

    octave_link::post_event (this, &Figure::copy_figure_callback,
                             format.toStdString ());
  }

  void
  Figure::addCustomToolBar (QToolBar *bar, bool visible)
  {
    QMainWindow *win = qWidget<QMainWindow> ();

    if (! visible)
      win->addToolBar (bar);
    else
      {
        QSize sz = bar->sizeHint ();
        QRect r = win->geometry ();
        //qDebug () << "Figure::addCustomToolBar:" << r;

        r.adjust (0, -sz.height (), 0, 0);

        m_blockUpdates = true;
        win->setGeometry (r);
        win->addToolBarBreak ();
        win->addToolBar (bar);
        m_blockUpdates = false;

        //qDebug () << "Figure::addCustomToolBar:" << win->geometry ();
        updateBoundingBox (false);
      }
  }

  void
  Figure::showCustomToolBar (QToolBar *bar, bool visible)
  {
    QMainWindow *win = qWidget<QMainWindow> ();

    if ((! bar->isHidden ()) != visible)
      {
        QSize sz = bar->sizeHint ();
        QRect r = win->geometry ();

        if (visible)
          r.adjust (0, -sz.height (), 0, 0);
        else
          r.adjust (0, sz.height (), 0, 0);

        m_blockUpdates = true;
        win->setGeometry (r);
        bar->setVisible (visible);
        m_blockUpdates = false;

        updateBoundingBox (false);
      }
  }

  void
  Figure::updateContainer (void)
  {
    redraw ();
  }

  void
  Figure::toggleAxes (void)
  {
    Canvas *canvas = m_container->canvas (m_handle);

    if (canvas)
      canvas->toggleAxes (m_handle);
  }

  void
  Figure::toggleGrid (void)
  {
    Canvas *canvas = m_container->canvas (m_handle);

    if (canvas)
      canvas->toggleGrid (m_handle);
  }

  void
  Figure::autoAxes (void)
  {
    Canvas *canvas = m_container->canvas (m_handle);

    if (canvas)
      canvas->autoAxes (m_handle);
  }

  void
  Figure::enableMouseTracking (void)
  {
    // Enable mouse tracking on every widgets
    m_container->setMouseTracking (true);
    m_container->canvas (m_handle)->qWidget ()->setMouseTracking (true);
    foreach (QWidget *w, m_container->findChildren<QWidget*> ())
      w->setMouseTracking (true);
  }

}
