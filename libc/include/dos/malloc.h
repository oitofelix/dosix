/*
  malloc.h -- Memory allocation

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

#ifndef _INC_MALLOC
#define _INC_MALLOC

#include <stddef.h>
#include <dos/compiler.h>

#define _NULLSEG ((__segment) 0)
#define _NULLOFF ((void __based(void) *) 0)

#define _HEAPEMPTY (-1)
#define _HEAPOK (-2)
#define _HEAPBADBEGIN (-3)
#define _HEAPBADNODE (-4)
#define _HEAPEND (-5)
#define _HEAPBADPTR (-6)
#define _FREEENTRY 0
#define _USEDENTRY 1

#ifndef _DOSIX_LIBC_SRC
#define malloc _dosix_malloc
#define realloc _dosix_realloc
#define free _dosix_free
#endif

struct _heapinfo;

typedef struct _heapinfo
{
  struct _heapinfo *_pentry;
  size_t _size;
  int _useflag;
} _HEAPINFO;

#ifdef __cplusplus
extern "C" {
#endif
  /* malloc functions */
  void * __cdecl _dosix_malloc (size_t);
  void __based (void) * __cdecl _bmalloc (__segment, size_t);
  void __far * _fmalloc (size_t);
  void __near * _nmalloc (size_t);
  /* realloc functions */
  void * __cdecl _dosix_realloc (void *, size_t);
  void __based (void) * __cdecl _brealloc (__segment, void __based (void) *, size_t);
  void __far * __cdecl _frealloc (void __far *, size_t);
  void __near * __cdecl _nrealloc (void __near *, size_t);
  /* free functions */
  void __cdecl _dosix_free (void *);
  void __cdecl _bfree (__segment, void __based (void) *);
  void __cdecl _ffree (void __far *);
  void __cdecl _nfree (void __near *);
  /* _msize functions */
  size_t __cdecl _msize (void *);
  size_t __cdecl _bmsize (__segment, void __based (void) *);
  size_t __cdecl _fmsize (void __far *);
  size_t __cdecl _nmsize (void __near *);
  /* _heapchk functions */
  int __cdecl _bheapchk (void *);
  int __cdecl _heapchk (void);
  int __cdecl _fheapchk (void);
  int __cdecl _nheapchk (void);
#ifdef __cplusplus
}
#endif

#endif
