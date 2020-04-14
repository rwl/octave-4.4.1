/*

Copyright (C) 2017-2018 John W. Eaton

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

#if ! defined (octave_interpreter_private_h)
#define octave_interpreter_private_h 1

#include "octave-config.h"

#include <string>

#include "symtab.h"

class cdef_manager;

namespace octave
{
  class bp_table;
  class call_stack;
  class child_list;
  class dynamic_loader;
  class gtk_manager;
  class help_system;
  class interpreter;
  class load_path;
  class tree_evaluator;
  class type_info;

  extern interpreter& __get_interpreter__ (const std::string& who);

  extern dynamic_loader& __get_dynamic_loader__ (const std::string& who);

  extern help_system& __get_help_system__ (const std::string& who);

  extern load_path& __get_load_path__ (const std::string& who);

  extern type_info& __get_type_info__ (const std::string& who);

  extern symbol_table& __get_symbol_table__ (const std::string& who);

  extern symbol_scope __get_current_scope__ (const std::string& who);

  extern symbol_scope __require_current_scope__ (const std::string& who);

  extern tree_evaluator& __get_evaluator__ (const std::string& who);

  extern bp_table& __get_bp_table__ (const std::string& who);

  extern call_stack& __get_call_stack__ (const std::string& who);

  extern child_list& __get_child_list__ (const std::string& who);

  extern cdef_manager& __get_cdef_manager__ (const std::string& who);

  extern gtk_manager& __get_gtk_manager__ (const std::string& who);
}

#endif
