/*
  time.h -- Date and time

  Copyright (C) 2020 Bruno Félix Rezende Ribeiro <oitofelix@gnu.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _INC_DOS_TIME
#define _INC_DOS_TIME

#include <dosix/compiler.h>

#ifndef _DOSIX_LIBC_SRC
#define _strtime _dosix__strtime
#define _strdate _dosix__strdate
#endif	/* ! _DOSIX_LIBC_SRC */

#ifdef __cplusplus
extern "C" {
#endif
  char * __cdecl _dosix__strtime (char *timestr);
  char * __cdecl _dosix__strdate (char *datestr);
#ifdef __cplusplus
}
#endif

#endif	/* ! _INC_DOS_TIME */
