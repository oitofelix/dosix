/*
  stdlib.h -- General utilities

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

#ifndef _INC_DOSIX_STDLIB
#define _INC_DOSIX_STDLIB

#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <dosix/compiler.h>

#ifndef _DOSIX_LIBC_SRC
#define _doserrno _dosix__doserrno
#define atexit _dosix_atexit
#define exit _dosix_exit
#define _exit _dosix__exit
#define _onexit _dosix__onexit
/* from malloc.h */
#define malloc _dosix_malloc
#define realloc _dosix_realloc
#define free _dosix_free
/* from ctype.h */
#define tolower _dosix_tolower
#define toupper _dosix_toupper
/* from process.h */
#define system _dosix_system

#ifndef __STRICT_ANSI__
#define ecvt _ecvt
#define fcvt _fcvt
#define gcvt _gcvt
#define itoa _itoa
#define ltoa _ltoa
#define onexit _onexit
#define putenv _putenv
#define swab _swab
#define ultoa _ultoa
#define onexit_t _onexit_t
#define sys_errlist _sys_errlist
#define sys_nerr _sys_nerr
#define environ _environ
#define DOS_MODE _DOS_MODE
#define OS2_MODE _OS2_MODE

#ifndef __cplusplus
#define max _dosix__max
#define min _dosix__min
#endif /* ! __cplusplus */

#endif /* ! __STRICT_ANSI__ */

#endif	/* _DOSIX_LIBC_SRC */

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *) 0)
#endif
#endif

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1

#define _MAX_DRIVE 3		  /* max. length of drive component */
#define _MAX_PATH (PATH_MAX + 1)  /* max. length of full pathname */
#define _MAX_DIR (PATH_MAX + 1)   /* max. length of path component */
#define _MAX_FNAME (NAME_MAX + 1) /* max. length of file name component */
#define _MAX_EXT (NAME_MAX + 1)	  /* max. length of extension component */

typedef int (__cdecl *_onexit_t) ();
typedef int (__far __cdecl *_fonexit_t) ();

#ifdef __cplusplus
extern "C" {
#endif
  extern int __near __cdecl _dosix__doserrno;
  void __cdecl _dosix_exit (int);
  void __cdecl _dosix__exit (int);
  int __cdecl _dosix_atexit (void (__cdecl *) (void));
  int __far __cdecl _dosix__fatexit (void (__cdecl __far *) (void));
  _onexit_t __cdecl _dosix__onexit (_onexit_t);
  _fonexit_t __far __cdecl _dosix__fonexit (_fonexit_t);
  /* max, min */
  intmax_t __cdecl _dosix__max (intmax_t, intmax_t);
  intmax_t __cdecl _dosix__min (intmax_t, intmax_t);
  /* from malloc.h */
  void * __cdecl _dosix_malloc (size_t);
  void * __cdecl _dosix_realloc (void *, size_t);
  void __cdecl _dosix_free (void *);
  /* from ctype.h */
  int __cdecl _dosix_tolower (int);
  int __cdecl _dosix_toupper (int);
  /* from system.h */
  int __cdecl _dosix_system (const char *);
#ifdef __cplusplus
}
#endif

#endif	/* ! _INC_DOSIX_STDLIB */
