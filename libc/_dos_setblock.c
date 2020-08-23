/*
  _dos_setblock.c -- Resize memory block (DOS 2+)

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

#define _GNU_SOURCE

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <search.h>
#include <errno.h>
#include <sys/mman.h>
#include "_dosexterr.h"
#include "_dos_allocmem.h"
#include "INT.h"

unsigned
_dos_setblock
(size_t size,
 uintptr_t seg,
 size_t *maxsize)
{
  assert (maxsize);
  struct _DOSERROR errorinfo = {0};
  if (! size) return 0;
  struct allocmem allocmem =
    {
     .address = (void *) seg,
     .length = 16 * size
    };
  struct allocmem **allocmem_ptr = tfind (&allocmem,
					  &_dosk__allocmem_tree,
					  &_dosk__allocmem_cmp);
  if (! allocmem_ptr)
    {
      errno = EFAULT;
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }
  assert (allocmem.address == (*allocmem_ptr)->address);
  allocmem.address = mremap (allocmem.address,
			     (*allocmem_ptr)->length,
			     allocmem.length,
			     0);
  if (allocmem.address == (void *) -1)
    return _dosk__allocmem_error (maxsize);
  assert (allocmem.address == (void *) seg);
  **allocmem_ptr = allocmem;
  return 0;
}

void
_dosk86_setblock
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_SETBLOCK);
  uintptr_t maxsize;
  cpu->r.ax = _dos_setblock (cpu->r.bx,
			     cpu->r.es,
			     &maxsize);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
  cpu->r.bx = cpu->r.flags ? maxsize : cpu->r.bx;
}
