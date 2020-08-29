/*
  malloc.c -- Memory allocation

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

#define _DOSIX_LIBC_SRC
#define _GNU_SOURCE


/* headers */

#include <assert.h>
#include <errno.h>
#include <err.h>
#include <stdlib.h>
#include <malloc.h>
#include <mcheck.h>
#include <search.h>
#include <dosix/malloc.h>


/* forward declarations */

static
int
heap_entryinfo_cmp
(const void *_a,
 const void *_b);

static
void
bheapchk
(const void *nodep,
 VISIT value,
 int level);


/* global variables */

static
void
*heap_tree;

static
int
heapstatus;


/* auxiliary functions */

static
int
heap_entryinfo_cmp
(const void *_a,
 const void *_b)
{
  const _HEAPINFO *a = (const _HEAPINFO *) _a;
  const _HEAPINFO *b = (const _HEAPINFO *) _b;
  return a->_pentry - b->_pentry;
}


/* malloc functions  */

void *
_dosix_malloc
(size_t size)
{
  _HEAPINFO *entryinfo;
  size_t total_size = sizeof (*entryinfo) + size;
  entryinfo = malloc (total_size);
  if (! entryinfo)
    return (void *)
      NULL;
  *entryinfo = (_HEAPINFO)
    {
     ._pentry =  entryinfo,
     ._size = total_size,
     ._useflag = _USEDENTRY
    };
  _HEAPINFO **_entryinfo = tsearch (entryinfo,
				    &heap_tree,
				    &heap_entryinfo_cmp);
  if (! _entryinfo)
    {
      free (entryinfo);
      return (void *)
	NULL;
    }
  assert (entryinfo == *_entryinfo);
  return (void *)
    (entryinfo + 1);
}

void __based (void) *
_dosix__bmalloc
  (__segment seg,
   size_t size)
{
  return (void __based (void) *)
    _dosix_malloc (size);
}

void __far *
_dosix__fmalloc (size_t size)
{
  return (void __far *)
    _dosix_malloc (size);
}

void __near *
_dosix__nmalloc
(size_t size)
{
  return (void __near *)
    _dosix_malloc (size);
}


/* realloc functions */

void *
_dosix_realloc
(void *memblock,
 size_t size)
{
  if (! memblock)
    return _dosix_malloc (size);
  if (! size)
    {
      _dosix_free (memblock);
      return NULL;
    }
  _HEAPINFO entryinfo =
    {
     ._pentry = memblock - sizeof (entryinfo),
     ._size = sizeof (entryinfo) + size,
     ._useflag = _USEDENTRY
    };
  _HEAPINFO **_entryinfo = tfind (&entryinfo,
				  &heap_tree,
				  &heap_entryinfo_cmp);
  if (! _entryinfo)
    {
      warnx ("realloc (%p, %zx): invalid pointer",
	     memblock,
	     size);
      errno = EINVAL;
      return NULL;
    }
  assert (entryinfo._pentry == (*_entryinfo)->_pentry);
  entryinfo._pentry = realloc (entryinfo._pentry,
			       entryinfo._size);
  if (! entryinfo._pentry)
    return NULL;
  *_entryinfo = entryinfo._pentry;
  **_entryinfo = entryinfo;
  return (void *)
    (*_entryinfo + 1);
}

void __based (void) *
  _dosix__brealloc
  (__segment seg,
   void __based (void) *memblock,
   size_t size)
{
  return (void __based (void) *)
    _dosix_realloc ((void *) memblock,
		    size);
}

void __far *
_dosix__frealloc
(void __far *memblock,
 size_t size)
{
  return (void __far *)
    _dosix_realloc ((void *) memblock,
		    size);
}

void __near *
_dosix__nrealloc
(void __near *memblock,
 size_t size)
{
  return (void __near *)
    _dosix_realloc ((void *) memblock,
		    size);
}


/* free functions */

void
_dosix_free
(void *memblock)
{
  if (! memblock)
    return;
  _HEAPINFO entryinfo =
    {
     ._pentry = memblock - sizeof (entryinfo)
    };
  _HEAPINFO **_entryinfo = tfind (&entryinfo,
				  &heap_tree,
				  &heap_entryinfo_cmp);
  if (! _entryinfo)
    {
      warnx ("free (%p): invalid pointer",
	     memblock);
      errno = EINVAL;
      return;
    }
  assert (entryinfo._pentry == (*_entryinfo)->_pentry);
  void *pnode = tdelete (&entryinfo,
			 &heap_tree,
			 &heap_entryinfo_cmp);
  assert (pnode);
  free (*_entryinfo);
}

void
_dosix__bfree
(__segment seg,
 void __based (void) *memblock)
{
  _dosix_free ((void *) memblock);
}

void
_dosix__ffree
(void __far *memblock)
{
  _dosix_free ((void *) memblock);
}

void
_dosix__nfree
(void __near *memblock)
{
  _dosix_free ((void *) memblock);
}


/* _msize functions */

size_t
_dosix__msize
(void *memblock)
{
  _HEAPINFO entryinfo =
    {
     ._pentry = memblock - sizeof (entryinfo)
    };
  _HEAPINFO **_entryinfo = tfind (&entryinfo,
				  &heap_tree,
				  &heap_entryinfo_cmp);
  if (! _entryinfo)
    {
      warnx ("_msize (%p): invalid pointer",
	     memblock);
      errno = EINVAL;
      return 0;
    }
  assert (entryinfo._pentry == (*_entryinfo)->_pentry);
  return (*_entryinfo)->_size;
}

size_t
_dosix__bmsize
(__segment seg,
 void __based (void) *memblock)
{
  return _dosix__msize ((void *) memblock);
}

size_t
_dosix__fmsize
(void __far *memblock)
{
  return _dosix__msize ((void *) memblock);
}

size_t
_dosix__nmsize
(void __near *memblock)
{
  return _dosix__msize ((void *) memblock);
}


static
void
bheapchk
(const void *nodep,
 VISIT value,
 int level)
{
  if (value != leaf && value != preorder
      || heapstatus != _HEAPOK)
    return;
  const _HEAPINFO *entryinfo = nodep;
  assert (entryinfo->_pentry);
  int status = _dosix__bheapchk (entryinfo->_pentry);
  if (status != _HEAPOK)
    heapstatus = status;
}

int
_dosix__bheapchk
(void *seg)
{
  if (! seg)
    {
      heapstatus = _HEAPOK;
      twalk (&heap_tree,
	     &bheapchk);
      return heapstatus;
    }
  else switch (mprobe (seg))
	 {
	 case MCHECK_DISABLED:
	   return _HEAPEMPTY;
	   break;
	 case MCHECK_OK:
	   return _HEAPOK;
	   break;
	 case MCHECK_HEAD:
	   return _HEAPBADBEGIN;
	   break;
	 case MCHECK_TAIL:
	   return _HEAPBADNODE;
	   break;
	 case MCHECK_FREE:
	   return _HEAPBADNODE;
	   break;
	 }
}

int
_dosix__heapchk
(void)
{
  return _dosix__bheapchk (NULL);
}

int
_dosix__fheapchk
(void)
{
  return _dosix__heapchk ();
}

int
_dosix__nheapchk
(void)
{
  return _dosix__heapchk ();
}
