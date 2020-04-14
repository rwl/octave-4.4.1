/*

Copyright (C) 2013-2018 John W. Eaton
Copyright (C) 2013-2018 Daniel J. Sebald

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

#include "dialog.h"

#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QListView>
#include <QFileInfo>
// Could replace most of these with #include <QtGui>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>

namespace octave
{
  QUIWidgetCreator uiwidget_creator;

  QUIWidgetCreator::QUIWidgetCreator (void)
    : QObject (), m_dialog_result (-1), m_dialog_button (),
      m_string_list (new QStringList ()), m_list_index (new QIntList ()),
      m_path_name (new QString ())
  { }

  QUIWidgetCreator::~QUIWidgetCreator (void)
  {
    delete m_string_list;
    delete m_list_index;
    delete m_path_name;
  }

  QString QUIWidgetCreator::rm_amp (const QString& text)
  {
    QString text_wo_amp = text;
    text_wo_amp.replace (QRegExp ("&(\\w)"), "\\1");
    return text_wo_amp;
  }

  void QUIWidgetCreator::dialog_button_clicked (QAbstractButton *button)
  {
    // Wait for link thread to go to sleep state.
    lock ();

    // Check for a matching button text while ignoring accelerators because
    // the window manager may have added one in the passed button
    QString text_clean = rm_amp (button->text ());
    for (int i = 0; i < m_button_list.count (); i++)
      {
        if (rm_amp (m_button_list.at (i)) == text_clean)
          {
            m_dialog_button = m_button_list.at (i); // text w/o extra accelerator
            break;
          }
      }

    // The value should always be 1 for the Octave functions.
    m_dialog_result = 1;

    unlock ();

    // Wake up Octave process so that it continues.
    wake_all ();
  }

  void QUIWidgetCreator::list_select_finished (const QIntList& selected,
                                               int button_pressed)
  {
    // Wait for link thread to go to sleep state.
    lock ();

    // Store the value so that builtin functions can retrieve.
    *m_list_index = selected;
    m_dialog_result = button_pressed;

    unlock ();

    // Wake up Octave process so that it continues.
    wake_all ();
  }

  void QUIWidgetCreator::input_finished (const QStringList& input,
                                         int button_pressed)
  {
    // Wait for link thread to go to sleep state.
    lock ();

    // Store the value so that builtin functions can retrieve.
    *m_string_list = input;
    m_dialog_result = button_pressed;

    unlock ();

    // Wake up Octave process so that it continues.
    wake_all ();
  }

  void QUIWidgetCreator::filedialog_finished (const QStringList& files,
                                              const QString& path,
                                              int filterindex)
  {
    // Wait for link thread to go to sleep state.
    lock ();

    // Store the value so that builtin functions can retrieve.
    *m_string_list = files;
    m_dialog_result = filterindex;
    *m_path_name = path;

    unlock ();

    // Wake up Octave process so that it continues.
    wake_all ();
  }

  MessageDialog::MessageDialog (const QString& message,
                                const QString& title,
                                const QString& qsicon,
                                const QStringList& qsbutton,
                                const QString& defbutton,
                                const QStringList& role)
    : QMessageBox (QMessageBox::NoIcon, title.isEmpty () ? " " : title,
                   message, nullptr, nullptr)
  {
    // Create a NonModal message.
    setWindowModality (Qt::NonModal);

    // Interpret the icon string, because enumeration QMessageBox::Icon can't
    // easily be made to pass through a signal.
    QMessageBox::Icon eicon = QMessageBox::NoIcon;
    if (qsicon == "error")
      eicon = QMessageBox::Critical;
    else if (qsicon == "warn")
      eicon = QMessageBox::Warning;
    else if (qsicon == "help")
      eicon = QMessageBox::Information;
    else if (qsicon == "quest")
      eicon = QMessageBox::Question;
    setIcon (eicon);

    int N = (qsbutton.size () < role.size () ? qsbutton.size () : role.size ());
    if (N == 0)
      addButton (QMessageBox::Ok);
    else
      {
        for (int i = 0; i < N; i++)
          {
            // Interpret the button role string, because enumeration
            // QMessageBox::ButtonRole can't be made to pass through a signal.
            QString srole = role.at (i);
            QMessageBox::ButtonRole erole = QMessageBox::InvalidRole;
            if (srole == "ResetRole")
              erole = QMessageBox::ResetRole;
            else if (srole == "YesRole")
              erole = QMessageBox::YesRole;
            else if (srole == "NoRole")
              erole = QMessageBox::NoRole;
            else if (srole == "RejectRole")
              erole = QMessageBox::RejectRole;
            else if (srole == "AcceptRole")
              erole = QMessageBox::AcceptRole;

            QPushButton *pbutton = addButton (qsbutton.at (i), erole);
            if (qsbutton.at (i) == defbutton)
              setDefaultButton (pbutton);
            // Make the last button the button pressed when <esc> key activated.
            if (i == N-1)
              {
                // FIXME: Why define and then immediately test value?
#define ACTIVE_ESCAPE 1
#if ACTIVE_ESCAPE
                setEscapeButton (pbutton);
#else
                setEscapeButton (0);
#endif
#undef ACTIVE_ESCAPE
              }
          }
      }

    connect (this, SIGNAL (buttonClicked (QAbstractButton *)),
             &uiwidget_creator,
             SLOT (dialog_button_clicked (QAbstractButton *)));
  }

  ListDialog::ListDialog (const QStringList& list, const QString& mode,
                          int wd, int ht, const QList<int>& initial,
                          const QString& title, const QStringList& prompt,
                          const QString& ok_string,
                          const QString& cancel_string)
    : QDialog (), m_model (new QStringListModel (list))
  {
    QListView *view = new QListView;
    view->setModel (m_model);

    if (mode == "single")
      view->setSelectionMode (QAbstractItemView::SingleSelection);
    else if (mode == "multiple")
      view->setSelectionMode (QAbstractItemView::ExtendedSelection);
    else
      view->setSelectionMode (QAbstractItemView::NoSelection);

    selector = view->selectionModel ();
    int i = 0;
    for (QList<int>::const_iterator it = initial.begin ();
         it != initial.end (); it++)
      {
        QModelIndex idx = m_model->index (initial.value (i++) - 1, 0,
                                          QModelIndex ());
        selector->select (idx, QItemSelectionModel::Select);
      }

    bool fixed_layout = false;
    if (wd > 0 && ht > 0)
      {
        view->setFixedSize (wd, ht);
        fixed_layout = true;
      }

    view->setEditTriggers (QAbstractItemView::NoEditTriggers);

    QVBoxLayout *listLayout = new QVBoxLayout;
    if (! prompt.isEmpty ())
      {
        // For now, assume html-like Rich Text.  May be incompatible
        // with something down the road, but just testing capability.
        QString prompt_string;
        for (int j = 0; j < prompt.length (); j++)
          {
            if (j > 0)
              // FIXME: Why define and then immediately test value?
#define RICH_TEXT 1
#if RICH_TEXT
              prompt_string.append ("<br>");
#else
              prompt_string.append ("\n");
#endif
            prompt_string.append (prompt.at (j));
          }
        QLabel *plabel = new QLabel (prompt_string);
#if RICH_TEXT
        plabel->setTextFormat (Qt::RichText);
#endif
#undef RICH_TEXT
        listLayout->addWidget (plabel);
      }
    listLayout->addWidget (view);
    QPushButton *select_all = new QPushButton (tr ("Select All"));
    select_all->setVisible (mode == "multiple");
    listLayout->addWidget (select_all);

    QPushButton *buttonOk = new QPushButton (ok_string);
    QPushButton *buttonCancel = new QPushButton (cancel_string);
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch (1);
    buttonsLayout->addWidget (buttonOk);
    buttonsLayout->addWidget (buttonCancel);
    buttonOk->setDefault (true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout (listLayout);
    mainLayout->addSpacing (12);
    mainLayout->addLayout (buttonsLayout);
    setLayout (mainLayout);
    if (fixed_layout)
      layout ()->setSizeConstraint (QLayout::SetFixedSize);

    // If empty, make blank rather than use default OS behavior.
    setWindowTitle (title.isEmpty () ? " " : title);

    connect (select_all, SIGNAL (clicked ()),
             view, SLOT (selectAll ()));

    connect (buttonOk, SIGNAL (clicked ()),
             this, SLOT (buttonOk_clicked ()));

    connect (buttonCancel, SIGNAL (clicked ()),
             this, SLOT (buttonCancel_clicked ()));

    connect (this, SIGNAL (finish_selection (const QIntList&, int)),
             &uiwidget_creator,
             SLOT (list_select_finished (const QIntList&, int)));

    connect (view, SIGNAL (doubleClicked (const QModelIndex&)),
             this, SLOT (item_double_clicked (const QModelIndex&)));
  }

  ListDialog::~ListDialog (void)
  {
    delete m_model;
  }

  void ListDialog::buttonOk_clicked (void)
  {
    // Store information about what button was pressed so that builtin
    // functions can retrieve.
    QModelIndexList selected_index = selector->selectedIndexes ();
    QIntList selected_int;

    for (int i = 0; i < selected_index.size (); i++)
      selected_int << selected_index.at (i).row () + 1;

    emit finish_selection (selected_int, 1);

    done (QDialog::Accepted);
  }

  void ListDialog::buttonCancel_clicked (void)
  {
    // Store information about what button was pressed so that builtin
    // functions can retrieve.
    QIntList empty;

    emit finish_selection (empty, 0);

    done (QDialog::Rejected);
  }

  void ListDialog::reject (void)
  {
    buttonCancel_clicked ();
  }

  void ListDialog::item_double_clicked (const QModelIndex&)
  {
    buttonOk_clicked ();
  }

  InputDialog::InputDialog (const QStringList& prompt, const QString& title,
                            const QFloatList& nr, const QFloatList& nc,
                            const QStringList& defaults)
    : QDialog ()
  {

#define LINE_EDIT_FOLLOWS_PROMPT 0

#if LINE_EDIT_FOLLOWS_PROMPT
    // Prompt on left followed by input on right.
    QGridLayout *promptInputLayout = new QGridLayout;
#else
    // Prompt aligned above input.
    QVBoxLayout *promptInputLayout = new QVBoxLayout;
#endif
    int N_gridrows = prompt.size ();
    for (int i = 0; i < N_gridrows; i++)
      {
        QLabel *label = new QLabel (prompt.at (i));
        QLineEdit *line_edit = new QLineEdit ();
        if (i < defaults.size ())
          line_edit->setText (defaults.at (i));
        if (i < nr.size () && nr.at (i) > 0)
          {
            QSize qsize = line_edit->sizeHint ();
            int intval = qsize.height () * nr.at (i);
            line_edit->setFixedHeight (intval);
            if (i < nc.size () && nc.at (i) > 0)
              {
                intval = qsize.height () * nc.at (i) / 2;
                line_edit->setFixedWidth (intval);
              }
          }
        input_line << line_edit;
#if LINE_EDIT_FOLLOWS_PROMPT
        promptInputLayout->addWidget (label, i + 1, 0);
        promptInputLayout->addWidget (line_edit, i + 1, 1);
#else
        promptInputLayout->addWidget (label);
        promptInputLayout->addWidget (line_edit);
#endif
      }
#undef LINE_EDIT_FOLLOWS_PROMPT

    QPushButton *buttonOk = new QPushButton ("OK");
    QPushButton *buttonCancel = new QPushButton ("Cancel");
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch (1);
    buttonsLayout->addWidget (buttonOk);
    buttonsLayout->addWidget (buttonCancel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout (promptInputLayout);
    mainLayout->addSpacing (12);
    mainLayout->addLayout (buttonsLayout);
    setLayout (mainLayout);

    // If empty, make blank rather than use default OS behavior.
    setWindowTitle (title.isEmpty () ? " " : title);

    connect (buttonOk, SIGNAL (clicked ()),
             this, SLOT (buttonOk_clicked ()));

    connect (buttonCancel, SIGNAL (clicked ()),
             this, SLOT (buttonCancel_clicked ()));

    connect (this, SIGNAL (finish_input (const QStringList&, int)),
             &uiwidget_creator,
             SLOT (input_finished (const QStringList&, int)));
  }

  void InputDialog::buttonOk_clicked (void)
  {
    // Store information about what button was pressed so that builtin
    // functions can retrieve.
    QStringList string_result;
    for (int i = 0; i < input_line.size (); i++)
      string_result << input_line.at (i)->text ();
    emit finish_input (string_result, 1);
    done (QDialog::Accepted);
  }

  void InputDialog::buttonCancel_clicked (void)
  {
    // Store information about what button was pressed so that builtin
    // functions can retrieve.
    QStringList empty;
    emit finish_input (empty, 0);
    done (QDialog::Rejected);
  }

  void InputDialog::reject (void)
  {
    buttonCancel_clicked ();
  }

  FileDialog::FileDialog (const QStringList& name_filters, const QString& title,
                          const QString& filename, const QString& dirname,
                          const QString& multimode)
    : QFileDialog ()
  {
    // Create a NonModal message.
    setWindowModality (Qt::NonModal);

    setWindowTitle (title.isEmpty () ? " " : title);
    setDirectory (dirname);

    if (multimode == "on")         // uigetfile multiselect=on
      {
        setFileMode (QFileDialog::ExistingFiles);
        setAcceptMode (QFileDialog::AcceptOpen);
      }
    else if (multimode == "create") // uiputfile
      {
        setFileMode (QFileDialog::AnyFile);
        setAcceptMode (QFileDialog::AcceptSave);
        setOption (QFileDialog::DontConfirmOverwrite, false);
        setConfirmOverwrite (true);
      }
    else if (multimode == "dir")    // uigetdir
      {
        setFileMode (QFileDialog::Directory);
        setOption (QFileDialog::ShowDirsOnly, true);
        setOption (QFileDialog::HideNameFilterDetails, true);
        setAcceptMode (QFileDialog::AcceptOpen);
      }
    else                           // uigetfile multiselect=off
      {
        setFileMode (QFileDialog::ExistingFile);
        setAcceptMode (QFileDialog::AcceptOpen);
      }

    setNameFilters (name_filters);

    selectFile (filename);

    connect (this,
             SIGNAL (finish_input (const QStringList&, const QString&, int)),
             &uiwidget_creator,
             SLOT (filedialog_finished (const QStringList&, const QString&,
                                        int)));
    connect (this, SIGNAL (accepted ()), this, SLOT (acceptSelection ()));
    connect (this, SIGNAL (rejected ()), this, SLOT (rejectSelection ()));
  }

  void FileDialog::rejectSelection (void)
  {
    QStringList empty;
    emit finish_input (empty, "", 0);
  }

  void FileDialog::acceptSelection (void)
  {
    QStringList string_result;
    QString path;
    int idx = 1;

    string_result = selectedFiles ();

    if (testOption (QFileDialog::ShowDirsOnly)  == true &&
        string_result.size () > 0)
      {
        path = string_result[0];
      }
    else
      {
        path = directory ().absolutePath ();
      }

    // Matlab expects just the filename, whereas the file dialog gave us
    // full path names, so fix it.

    for (int i = 0; i < string_result.size (); i++)
      string_result[i] = QFileInfo (string_result[i]).fileName ();

    // if not showing only dirs, add end slash for the path component
    if (testOption (QFileDialog::ShowDirsOnly)  == false)
      path += '/';

    // convert to native slashes
    path = QDir::toNativeSeparators (path);

    QStringList name_filters = nameFilters ();
    idx = name_filters.indexOf (selectedNameFilter ()) + 1;

    // send the selected info
    emit finish_input (string_result, path, idx);
  }
}
