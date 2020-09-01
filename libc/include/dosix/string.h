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

#ifndef _INC_DOSIX_STRING
#define _INC_DOSIX_STRING

#include <stddef.h>
#include <dosix/compiler.h>

#ifndef _DOSIX_LIBC_SRC
#define strlen _dosix_strlen
#define strcpy _dosix_strcpy
#define strcat _dosix_strcat
#define _fstrlen _dosix__fstrlen
#define _fstrcpy _dosix__fstrcpy
#define _fstrcat _dosix__fstrcat
#define _strdup _dosix__strdup
#define _fstrdup _dosix__fstrdup
#define _nstrdup _dosix__nstrdup
#define _strlwr _dosix__strlwr
#define _fstrlwr _dosix__fstrlwr
#define _strupr _dosix__strupr
#define _fstrupr _dosix__fstrupr

#ifndef __STRICT_ANSI__
#define memccpy _memccpy
#define memicmp _memicmp
#define movedata _movedata
#define strcmpi _strcmpi
#define stricmp _stricmp
#define strnicmp _strnicmp
#define strnset _strnset
#define strrev _strrev
#define strset _strset
#define strdup _strdup
#define strlwr _strlwr
#define strupr _strupr
#endif	/* ! __STRICT_ANSI__ */

#endif	/* ! _DOSIX_LIBX_SRC */

#ifdef __cplusplus
extern "C" {
#endif
  /* strlen */
  size_t __cdecl _dosix_strlen (const char *);
  size_t __cdecl _dosix__fstrlen (const char __far *string);
  /* strlwr */
  char * __cdecl _dosix__strlwr (char *);
  char __far * __far __cdecl _dosix__fstrlwr (char __far *);
  /* strupr */
  char * __cdecl _dosix__strupr (char *);
  char __far * __far __cdecl _dosix__fstrupr (char __far *);
  /* strcpy */
  char * __cdecl _dosix_strcpy (char *, const char *);
  char __far * __far __cdecl _dosix__fstrcpy (char __far *, const char __far *);
  /* strcat */
  char * __cdecl _dosix_strcat (char *, const char *);
  char __far * __far __cdecl _dosix__fstrcat (char __far *, const char __far *);
  /* strdup */
  char * __cdecl _dosix__strdup (const char *);
  char __far * __far __cdecl _dosix__fstrdup (const char __far *);
  char __near * __far __cdecl _dosix__nstrdup (const char __far *);
#ifdef __cplusplus
}
#endif

#endif	/* ! _INC_DOSIX_STRING */
