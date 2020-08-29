/*
  process.h -- Process control

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

#ifndef _INC_PROCESS
#define _INC_PROCESS

#include <dosix/compiler.h>

#ifndef _DOSIX_LIBC_SRC
#define exit _dosix_exit
#define _exit _dosix__exit
#define _cexit _dosix__cexit
#define _c_exit _dosix__c_exit
#define system _dosix_system
#endif	/* ! _DOSIX_LIBC_SRC */

#ifdef __cplusplus
extern "C" {
#endif
  /* exit functions */
  void __cdecl _dosix_exit (int);
  void __cdecl _dosix__exit (int);
  void __cdecl _dosix__cexit (void);
  void __cdecl _dosix__c_exit (void);
  /* system */
  int __cdecl _dosix_system (const char *);
#ifdef __cplusplus
}
#endif

#endif	/* ! _INC_PROCESS */
