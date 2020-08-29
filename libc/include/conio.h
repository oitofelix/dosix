/*
  conio.h -- Console and port I/O

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

#ifndef _INC_CONIO
#define _INC_CONIO

#include <dosix/compiler.h>

#ifndef _DOSIX_LIBC_SRC
#define _cputs _dosix__cputs
#define _getch _dosix__getch
#define _getche _dosix__getche
#define _putch _dosix__putch

#ifndef __STRICT_ANSI__
#define cputs _cputs
#define getch _getch
#define getche _getche
#define putch _putch
#endif	/* ! __STRICT_ANSI__ */

#endif	/* ! _DOSIX_LIBC_SRC */

#ifdef __cplusplus
extern "C" {
#endif
  int __cdecl _dosix__cputs (const char *);
  int __cdecl _dosix__getch (void);
  int __cdecl _dosix__getche (void);
  int __cdecl _dosix__putch (int);
#ifdef __cplusplus
}
#endif

#endif	/* ! _INC_CONIO */
