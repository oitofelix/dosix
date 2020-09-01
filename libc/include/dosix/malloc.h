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

#ifndef _INC_DOSIX_MALLOC
#define _INC_DOSIX_MALLOC

#include <stddef.h>
#include <dosix/compiler.h>

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

/* malloc functions */
#define malloc _dosix_malloc
#define _bmalloc _dosix__bmalloc
#define _fmalloc _dosix__fmalloc
#define _nmalloc _dosix__nmalloc
/* realloc functions */
#define realloc _dosix_realloc
#define _brealloc _dosix__brealloc
#define _frealloc _dosix__frealloc
#define _nrealloc _dosix__nrealloc
/* free functions */
#define free _dosix_free
#define _bfree _dosix__bfree
#define _ffree _dosix__ffree
#define _nfree _dosix__nfree
/* _msize functions */
#define _msize _dosix__msize
#define _bmsize _dosix__bmsize
#define _fmsize _dosix__fmsize
#define _nmsize _dosix__nmsize
/* _heapchk functions */
#define _bheapchk _dosix__bheapchk
#define _heapchk _dosix__heapchk
#define _fheapchk _dosix__fheapchk
#define _nheapchk _dosix__nheapchk

#ifndef __STRICT_ANSI__
#define alloca _alloca
#define halloc _halloc
#define hfree _hfree
#define stackavail _stackavail
#endif	/* ! __STRICT_ANSI__ */

#endif	/* ! _DOSIX_LIBC_SRC */

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
  void __based (void) * __cdecl _dosix__bmalloc (__segment, size_t);
  void __far * _dosix__fmalloc (size_t);
  void __near * _dosix__nmalloc (size_t);
  /* realloc functions */
  void * __cdecl _dosix_realloc (void *, size_t);
  void __based (void) * __cdecl _dosix__brealloc (__segment,
						  void __based (void) *,
						  size_t);
  void __far * __cdecl _dosix__frealloc (void __far *, size_t);
  void __near * __cdecl _dosix__nrealloc (void __near *, size_t);
  /* free functions */
  void __cdecl _dosix_free (void *);
  void __cdecl _dosix__bfree (__segment, void __based (void) *);
  void __cdecl _dosix__ffree (void __far *);
  void __cdecl _dosix__nfree (void __near *);
  /* _msize functions */
  size_t __cdecl _dosix__msize (void *);
  size_t __cdecl _dosix__bmsize (__segment, void __based (void) *);
  size_t __cdecl _dosix__fmsize (void __far *);
  size_t __cdecl _dosix__nmsize (void __near *);
  /* _heapchk functions */
  int __cdecl _dosix__bheapchk (void *);
  int __cdecl _dosix__heapchk (void);
  int __cdecl _dosix__fheapchk (void);
  int __cdecl _dosix__nheapchk (void);
#ifdef __cplusplus
}
#endif

#endif	/* ! _INC_DOSIX_MALLOC */
