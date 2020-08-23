/*
  _dos_freemem.c -- Free memory (DOS 2+)

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
#include <stdint.h>
#include <search.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "_dosexterr.h"
#include "_dos_allocmem.h"
#include "INT.h"

unsigned
_dos_freemem
(uintptr_t seg)
{
  assert (seg);
  struct _DOSERROR errorinfo = {0};
  struct allocmem allocmem =
    {
     .address = (void *) seg,
     .length = 0
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
  if (munmap (allocmem.address,
	      (*allocmem_ptr)->length))
    {
      errorinfo.exterror = EXTERR_MCB_DESTROYED;
      errorinfo.errclass = ERRCLASS_INTERN_SYS_ERROR;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_MEM_RELATED;
      __doskexterr_set (&errorinfo);
      errno = EINVAL;
      return errorinfo.exterror;
    }
  void *pnode = tdelete (&allocmem,
			 &_dosk__allocmem_tree,
			 &_dosk__allocmem_cmp);
  assert (pnode);
  free (*allocmem_ptr);
  return 0;
}

void
_dosk86_freemem
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FREEMEM);
  cpu->r.ax = _dos_freemem (cpu->r.es);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}
