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

#if ! defined (octave_dialog_h)
#define octave_dialog_h 1

#include <QMutex>
#include <QWaitCondition>
#include <QAbstractButton>
#include <QList>
#include <QItemSelectionModel>
#include <QDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QFileDialog>

// Defined for purposes of sending QList<int> as part of signal.
typedef QList<int> QIntList;

// Defined for purposes of sending QList<float> as part of signal.
typedef QList<float> QFloatList;

namespace octave
{
  class QUIWidgetCreator : public QObject
  {
    Q_OBJECT

  public:

    QUIWidgetCreator (void);

    ~QUIWidgetCreator (void);

  public:

    QString rm_amp (const QString& text);

    void signal_dialog (const QString& message, const QString& title,
                        const QString& icon, const QStringList& button,
                        const QString& defbutton, const QStringList& role)
    {
      // Store button text before a window-manager adds accelerators
      m_button_list = button;

      // Use the last button in the list as the reject result, i.e., when no
      // button is pressed such as in the case of the upper right close tab.
      if (! button.isEmpty ())
        m_dialog_button = button.last ();

      QString xicon = icon;
      if (xicon.isEmpty ())
        xicon = "none";

      emit create_dialog (message, title, xicon, button, defbutton, role);
    };

    int get_dialog_result (void) { return m_dialog_result; }

    QString get_dialog_button (void) { return m_dialog_button; }

    bool signal_listview (const QStringList& list, const QString& mode,
                          int wd, int ht, const QList<int>& initial,
                          const QString& name, const QStringList& prompt,
                          const QString& ok_string,
                          const QString& cancel_string)
    {
      if (list.isEmpty ())
        return false;

      emit create_listview (list, mode, wd, ht, initial, name,
                            prompt, ok_string, cancel_string);

      return true;
    };

    const QIntList * get_list_index (void) { return m_list_index; }

    bool signal_inputlayout (const QStringList& prompt, const QString& title,
                             const QFloatList& nr, const QFloatList& nc,
                             const QStringList& defaults)
    {
      if (prompt.isEmpty ())
        return false;

      emit create_inputlayout (prompt, title, nr, nc, defaults);

      return true;
    };

    const QStringList * get_string_list (void) { return m_string_list; }

    bool signal_filedialog (const QStringList& filters, const QString& title,
                            const QString& filename, const QString& dirname,
                            const QString& multimode)
    {
      emit create_filedialog (filters, title, filename, dirname, multimode);
      return true;
    }

    const QString * get_dialog_path (void) { return m_path_name; }

    void lock (void) { m_mutex.lock (); }
    void wait (void) { m_waitcondition.wait (&m_mutex); }
    void unlock (void) { m_mutex.unlock (); }
    void wake_all (void) { m_waitcondition.wakeAll (); }

  signals:

    void create_dialog (const QString&, const QString&, const QString&,
                        const QStringList&, const QString&, const QStringList&);

    void create_listview (const QStringList&, const QString&, int, int,
                          const QIntList&, const QString&, const QStringList&,
                          const QString&, const QString&);

    void create_inputlayout (const QStringList&, const QString&,
                             const QFloatList&, const QFloatList&,
                             const QStringList&);

    void create_filedialog (const QStringList& filters, const QString& title,
                            const QString& filename, const QString& dirname,
                            const QString& multimode);
  public slots:

    void dialog_button_clicked (QAbstractButton *button);

    void list_select_finished (const QIntList& selected, int button_pressed);

    void input_finished (const QStringList& input, int button_pressed);

    void filedialog_finished (const QStringList& files, const QString& path,
                              int filterindex);

  private:

    int m_dialog_result;
    QString m_dialog_button;

    // A copy of the dialogs button texts
    QStringList m_button_list;

    // The list could conceivably be big.  Not sure how things are
    // stored internally, so keep off of the stack.
    QStringList *m_string_list;
    QIntList *m_list_index;

    QString *m_path_name;

    // GUI objects cannot be accessed in the non-GUI thread.  However,
    // signals can be sent to slots across threads with proper
    // synchronization.  Hence, the use of QWaitCondition.
    QMutex m_mutex;
    QWaitCondition m_waitcondition;
  };

  extern QUIWidgetCreator uiwidget_creator;

  class MessageDialog : public QMessageBox
  {
    Q_OBJECT

  public:

    explicit MessageDialog (const QString& message, const QString& title,
                            const QString& icon, const QStringList& button,
                            const QString& defbutton,
                            const QStringList& role);

  private:

    void closeEvent (QCloseEvent *)
    {
      // Reroute the close tab to a button click so there is only a single
      // route to waking the wait condition.
      emit buttonClicked (nullptr);
    }
  };

  class ListDialog : public QDialog
  {
    Q_OBJECT

    QItemSelectionModel *selector;

  public:

    explicit ListDialog (const QStringList& list, const QString& mode,
                         int width, int height, const QList<int>& initial,
                         const QString& name, const QStringList& prompt,
                         const QString& ok_string,
                         const QString& cancel_string);

    ~ListDialog (void);

  signals:

    void finish_selection (const QIntList&, int);

  public slots:

    void buttonOk_clicked (void);

    void buttonCancel_clicked (void);

    void reject (void);

    void item_double_clicked (const QModelIndex&);

  private:

    QAbstractItemModel *m_model;
  };

  class InputDialog : public QDialog
  {
    Q_OBJECT

    QList<QLineEdit *> input_line;

  public:

    explicit InputDialog (const QStringList& prompt, const QString& title,
                          const QFloatList& nr, const QFloatList& nc,
                          const QStringList& defaults);

  signals:

    void finish_input (const QStringList&, int);

  public slots:

    void buttonOk_clicked (void);

    void buttonCancel_clicked (void);

    void reject (void);
  };

  class FileDialog : public QFileDialog
  {
    Q_OBJECT

  public:

    explicit FileDialog (const QStringList& filters,
                         const QString& title, const QString& filename,
                         const QString& dirname, const QString& multimode);

  signals:

    void finish_input (const QStringList&, const QString&, int);

  private slots:

    void acceptSelection (void);

    void rejectSelection (void);
  };
}

#endif
