/*
  string.h -- String handling

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

#ifndef _INC_STRING
#define _INC_STRING

#include <dos/compiler.h>

#define strlwr _strlwr
#define fstrlwr _fstrlwr
#define strupr _strupr
#define fstrupr _fstrupr

#ifdef __cplusplus
extern "C" {
#endif
  /* strlwr */
  char * __cdecl _strlwr (char *);
  char __far * __far _fstrlwr (char __far *);
  /* strupr */
  char * __cdecl _strupr (char *);
  char __far * __far _fstrupr (char __far *);
#ifdef __cplusplus
}
#endif

#endif
