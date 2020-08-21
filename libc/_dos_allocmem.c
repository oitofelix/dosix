/*
  _dos_allocmem.c -- Allocate memory (DOS 2+)

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

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <search.h>
#include "_dosexterr.h"
#include "INT.h"
#include "_dos_allocmem.h"

void *_dosk__allocmem_tree;

static
unsigned
free_paragraphs
(uintptr_t *count)
{
  assert (count);
  struct _DOSERROR errorinfo = {0};
  uintptr_t free_pages = sysconf (_SC_AVPHYS_PAGES);
  if (free_pages == -1)
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }
  uintptr_t page_size = sysconf (_SC_PAGESIZE);
  if (page_size == -1)
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }
  *count = (free_pages * page_size) / 16;
  return 0;
}

unsigned
_dosk__allocmem_error
(uintptr_t *count)
{
  assert (count);
  struct _DOSERROR errorinfo = {0};
  unsigned err = free_paragraphs (count);
  if (err) return err;
  _dosexterr (&errorinfo);
  return errorinfo.exterror;
}

int
_dosk__allocmem_cmp
(const void *_a,
 const void *_b)
{
  const struct allocmem *a = (const struct allocmem *) _a;
  const struct allocmem *b = (const struct allocmem *) _b;
  return a->address - b->address;
}

unsigned
_dos_allocmem
(size_t size,
 uintptr_t *seg)
{
  assert (seg);
  struct _DOSERROR errorinfo = {0};
  if (! size) return 0;
  struct allocmem *allocmem_ptr = malloc (sizeof (*allocmem_ptr));
  allocmem_ptr->length = size * 16;
  allocmem_ptr->address = mmap (NULL,
				allocmem_ptr->length,
				PROT_WRITE | PROT_WRITE | PROT_EXEC,
				MAP_PRIVATE | MAP_ANON,
				0, 0);
  if (allocmem_ptr->address == MAP_FAILED)
    {
      free (allocmem_ptr);
      return _dosk__allocmem_error (seg);
    }
  else
    {
      struct allocmem **_allocmem_ptr = tsearch (allocmem_ptr,
						 &_dosk__allocmem_tree,
						 &_dosk__allocmem_cmp);
      if (! _allocmem_ptr)
	{
	  free (allocmem_ptr);
	  return _dosk__allocmem_error (seg);
	}
      assert (allocmem_ptr == *_allocmem_ptr);
      *seg = (uintptr_t) allocmem_ptr->address;
      return 0;
    }
}

int
_dosk86_allocmem
(union _REGS *inregs,
 union _REGS *outregs,
 struct _SREGS *segregs)
{
  assert (inregs), assert (outregs);
  assert (inregs->h.ah == INT21_AH_ALLOCMEM);
  uintptr_t seg;
  outregs->x.ax = _dos_allocmem (inregs->x.bx,
				 &seg);
  outregs->x.cflag = outregs->x.ax ? ~0 : 0;
  if (outregs->x.cflag)
    outregs->x.bx = seg;
  else
    outregs->x.ax = seg;
  return outregs->x.ax;
}
