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

#ifndef _INC_STDLIB
#define _INC_STDLIB

#include <stddef.h>
#include <limits.h>
#include <dos/compiler.h>

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

#define onexit_t _onexit_t

#ifndef _DOSIX_LIBC_SRC
#define malloc _dosix_malloc
#define realloc _dosix_realloc
#define free _dosix_free
#define atexit _dosix_atexit
#define exit _dosix_exit
#define _exit _dosix__exit
#define onexit _onexit
#endif

typedef int (__cdecl *_onexit_t) ();
typedef int (__far __cdecl *_fonexit_t) ();

#ifdef __cplusplus
extern "C" {
#endif
  extern int __near __cdecl _doserrno;
  void __cdecl _dosix_exit (int);
  void __cdecl _dosix__exit (int);
  int _dosix_atexit (void (__cdecl *) (void));
  int __far __cdecl _fatexit (void (__cdecl __far *) (void));
  _onexit_t __cdecl _onexit (_onexit_t);
  _fonexit_t __far __cdecl _fonexit (_fonexit_t);
#ifdef __cplusplus
}
#endif

#endif
