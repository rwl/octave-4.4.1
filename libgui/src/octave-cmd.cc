/*

Copyright (C) 2014-2018 Torsten

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

// Author: Torsten <ttl@justmail.de>

#if defined (HAVE_CONFIG_H)
#  include "config.h"
#endif

#include "octave-cmd.h"

#include "octave-qt-link.h"
#include "cmd-edit.h"
#include "builtin-defun-decls.h"
#include "utils.h"

namespace octave
{
  void octave_cmd_exec::execute (void)
  {
    std::string pending_input = octave::command_editor::get_current_line ();

    octave::command_editor::set_initial_input (pending_input);
    octave::command_editor::replace_line (m_cmd.toStdString ());
    octave::command_editor::redisplay ();
    octave::command_editor::accept_line ();
  }

  void octave_cmd_eval::execute (void)
  {
    QString function_name = m_info.fileName ();
    function_name.chop (m_info.suffix ().length () + 1);
    std::string file_path = m_info.absoluteFilePath ().toStdString ();

    std::string pending_input = octave::command_editor::get_current_line ();

    if (valid_identifier (function_name.toStdString ()))
      {
        // valid identifier: call as function with possibility to debug
        std::string path = m_info.absolutePath ().toStdString ();
        if (octave_qt_link::file_in_path (file_path, path))
          octave::command_editor::replace_line (function_name.toStdString ());
      }
    else
      {
        // no valid identifier: use Fsource (), no debug possible
        Fsource (ovl (file_path));
        octave::command_editor::replace_line ("");
      }

    octave::command_editor::set_initial_input (pending_input);
    octave::command_editor::redisplay ();

    octave::command_editor::accept_line ();
  }

  void octave_cmd_debug::execute (void)
  {
    if (m_cmd == "step")
      {
        F__db_next_breakpoint_quiet__ (ovl (m_suppress_dbg_location));
        Fdbstep ();
      }
    else if (m_cmd == "cont")
      {
        F__db_next_breakpoint_quiet__ (ovl (m_suppress_dbg_location));
        Fdbcont ();
      }
    else if (m_cmd == "quit")
      Fdbquit ();
    else
      {
        F__db_next_breakpoint_quiet__ (ovl (m_suppress_dbg_location));
        Fdbstep (ovl (m_cmd.toStdString ()));
      }

    octave::command_editor::interrupt (true);
  }

  // add a command to the queue

  void octave_command_queue::add_cmd (octave_cmd *cmd)
  {
    m_queue_mutex.lock ();
    m_queue.append (cmd);
    m_queue_mutex.unlock ();

    if (m_processing.tryAcquire ())  // if callback not processing, post event
      octave_link::post_event (this, &octave_command_queue::execute_command_callback);
  }

  // callback for executing the command by the worker thread

  void octave_command_queue::execute_command_callback (void)
  {
    bool repost = false;          // flag for reposting event for this callback

    if (! m_queue.isEmpty ())  // list can not be empty here, just to make sure
      {
        m_queue_mutex.lock ();     // critical path

        octave_cmd *cmd = m_queue.takeFirst ();

        if (m_queue.isEmpty ())
          m_processing.release (); // cmd queue empty, processing will stop
        else
          repost = true;          // not empty, repost at end
        m_queue_mutex.unlock ();

        cmd->execute ();

        delete cmd;
      }

    if (repost)  // queue not empty, so repost event for further processing
      octave_link::post_event (this, &octave_command_queue::execute_command_callback);
  }
}
