/*

Copyright (C) 1993-2018 John W. Eaton

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

#if ! defined (octave_error_h)
#define octave_error_h 1

#include "octave-config.h"

#include <cstdarg>
#include <string>

#include "unwind-prot.h"

class octave_map;
class octave_value_list;
namespace octave
{
  class execution_exception;
}

#define panic_impossible()                                              \
  panic ("impossible state reached in file '%s' at line %d", __FILE__, __LINE__)

extern OCTINTERP_API void reset_error_handler (void);

extern OCTINTERP_API int warning_enabled (const std::string& id);

extern OCTINTERP_API octave::execution_exception
make_execution_exception (const char *who);

extern OCTINTERP_API void
vmessage (const char *name, const char *fmt, va_list args);

extern OCTINTERP_API void message (const char *name, const char *fmt, ...);

OCTAVE_DEPRECATED (4.2, "use 'print_usage' or 'verror' instead")
OCTAVE_NORETURN OCTINTERP_API extern
void vusage (const char *fmt, va_list args);

OCTAVE_DEPRECATED (4.2, "use 'print_usage' or 'error' instead")
OCTAVE_NORETURN OCTINTERP_API extern
void usage (const char *fmt, ...);

extern OCTINTERP_API void vwarning (const char *fmt, va_list args);
extern OCTINTERP_API void warning (const char *fmt, ...);

OCTAVE_NORETURN OCTINTERP_API extern
void verror (const char *fmt, va_list args);

OCTAVE_NORETURN OCTINTERP_API extern
void error (const char *fmt, ...);

OCTAVE_NORETURN OCTINTERP_API extern
void verror (octave::execution_exception&, const char *fmt, va_list args);
OCTAVE_NORETURN OCTINTERP_API extern
void error (octave::execution_exception&, const char *fmt, ...);

OCTAVE_NORETURN OCTINTERP_API extern
void verror_with_cfn (const char *fmt, va_list args);
OCTAVE_NORETURN OCTINTERP_API extern
void error_with_cfn (const char *fmt, ...);

OCTAVE_NORETURN OCTINTERP_API extern
void vparse_error (const char *fmt, va_list args);
OCTAVE_NORETURN OCTINTERP_API extern
void parse_error (const char *fmt, ...);

extern OCTINTERP_API void
vmessage_with_id (const char *id, const char *name,
                  const char *fmt, va_list args);

extern OCTINTERP_API void
message_with_id (const char *id, const char *name, const char *fmt, ...);

OCTAVE_NORETURN OCTINTERP_API extern
void vusage_with_id (const char *id, const char *fmt, va_list args);

OCTAVE_NORETURN OCTINTERP_API extern
void usage_with_id (const char *id, const char *fmt, ...);

extern OCTINTERP_API void
vwarning_with_id (const char *id, const char *fmt, va_list args);

extern OCTINTERP_API void
warning_with_id (const char *id, const char *fmt, ...);

OCTAVE_NORETURN OCTINTERP_API extern
void verror_with_id (const char *id, const char *fmt, va_list args);

OCTAVE_NORETURN OCTINTERP_API extern
void error_with_id (const char *id, const char *fmt, ...);

OCTAVE_NORETURN OCTINTERP_API extern
void verror_with_id_cfn (const char *id, const char *fmt, va_list args);

OCTAVE_NORETURN OCTINTERP_API extern
void error_with_id_cfn (const char *id, const char *fmt, ...);

OCTAVE_NORETURN OCTINTERP_API extern
void vparse_error_with_id (const char *id, const char *fmt, va_list args);

OCTAVE_NORETURN OCTINTERP_API extern
void parse_error_with_id (const char *id, const char *fmt, ...);

OCTAVE_NORETURN OCTINTERP_API extern
void panic (const char *fmt, ...);

// Helper function for print_usage defined in defun.cc.
extern OCTINTERP_API void defun_usage_message (const std::string& msg);

extern OCTINTERP_API octave_value_list
set_warning_state (const std::string& id, const std::string& state);

extern OCTINTERP_API octave_value_list
set_warning_state (const octave_value_list& args);

extern OCTINTERP_API void disable_warning (const std::string& id);
extern OCTINTERP_API void initialize_default_warning_state (void);

// TRUE means that Octave will try to enter the debugger when an error
// is encountered.  This will also inhibit printing of the normal
// traceback message (you will only see the top-level error message).
extern OCTINTERP_API bool Vdebug_on_error;

// TRUE means that Octave will try to enter the debugger when an error
// is encountered within the 'try' section of a 'try' / 'catch' block.
extern OCTINTERP_API bool Vdebug_on_caught;

// TRUE means that Octave will try to enter the debugger when a warning
// is encountered.
extern OCTINTERP_API bool Vdebug_on_warning;

// Current error state.
extern OCTINTERP_API int error_state;

// Current warning state.
extern OCTINTERP_API int warning_state;

// Tell the error handler whether to print messages, or just store
// them for later.  Used for handling errors in eval() and
// the 'unwind_protect' statement.
extern OCTINTERP_API int buffer_error_messages;

// The number of layers of try / catch blocks we're in.  Used to print
// "caught error" instead of "error" when "dbstop if caught error" is on.
extern OCTINTERP_API int in_try_catch;

// TRUE means error messages are turned off.
extern OCTINTERP_API bool discard_error_messages;

// TRUE means warning messages are turned off.
extern OCTINTERP_API bool discard_warning_messages;

// Helper functions to pass last error and warning messages and ids
extern OCTINTERP_API std::string last_error_message (void);
extern OCTINTERP_API std::string last_error_id (void);
extern OCTINTERP_API octave_map last_error_stack (void);
extern OCTINTERP_API std::string last_warning_message (void);
extern OCTINTERP_API std::string last_warning_id (void);

extern OCTINTERP_API void interpreter_try (octave::unwind_protect&);

#endif
