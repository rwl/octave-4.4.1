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

#if ! defined (octave_sysdep_h)
#define octave_sysdep_h 1

#include "octave-config.h"

#include <cstdio>

#include <string>

#include "lo-ieee.h"
#include "lo-sysdep.h"

extern OCTINTERP_API void sysdep_init (void);

extern OCTINTERP_API void set_application_id (void);

extern OCTINTERP_API void sysdep_cleanup (void);

extern OCTINTERP_API void raw_mode (bool, bool wait = true);

extern OCTINTERP_API FILE * octave_popen (const char *command,
                                          const char *mode);

extern OCTINTERP_API int octave_pclose (FILE *f);

extern OCTINTERP_API int octave_kbhit (bool wait = true);

extern OCTINTERP_API std::string get_P_tmpdir (void);

extern OCTINTERP_API bool same_file_internal (const std::string&,
                                              const std::string&);

#endif
