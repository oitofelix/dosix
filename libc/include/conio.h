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

#include <dos/compiler.h>

#define cputs _cputs
#define getch _getch
#define getche _getche
#define putch _putch

#ifdef __cplusplus
extern "C" {
#endif
  int __cdecl _cputs (const char *);
  int __cdecl _getch (void);
  int __cdecl _getche (void);
  int __cdecl _putch (int);
#ifdef __cplusplus
}
#endif

#endif
