/*
  _dos_creat.c -- Create or truncate file (DOS 2+)
                  Create new file (DOS 3.0+)

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
#include <fcntl.h>
#include "_dosexterr.h"
#include "INT.h"

static
unsigned
__dos_creat
(const char* filename,
 unsigned attrib,
 int *handle,
 int flags)
{
  assert (filename), assert (handle);
  struct _DOSERROR errorinfo = {0};
  mode_t mode = S_IRWXU
    & ((attrib & _A_RDONLY)
       ? ~S_IWUSR
       : ~0);
  int fd = open (filename,
		 flags,
		 mode);
  if (fd < 0)
    return _dosexterr (&errorinfo); /* TODO? better error handling */
  *handle = fd;
  return 0;
}

unsigned
_dos_creat
(const char* filename,
 unsigned attrib,
 int *handle)
{
  return __dos_creat (filename,
		      attrib,
		      handle,
		      O_RDWR | O_CREAT | O_TRUNC);
}

void
_dosk86_creat
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_CREAT);
  int handle;
  cpu->r.ax = _dos_creat (_MK_FP (cpu->r.ds, cpu->r.dx),
			  cpu->r.cx,
			  &handle);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
  cpu->r.ax = cpu->r.ax ? cpu->r.ax : handle;
}

unsigned
_dos_creatnew
(const char* filename,
 unsigned attrib,
 int *handle)
{
  return __dos_creat (filename,
		      attrib,
		      handle,
		      O_RDWR | O_CREAT | O_TRUNC | O_EXCL);
}

void
_dosk86_creatnew
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_CREATNEW);
  int handle;
  cpu->r.ax = _dos_creat (_MK_FP (cpu->r.ds, cpu->r.dx),
			  cpu->r.cx,
			  &handle);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
  cpu->r.ax = cpu->r.ax ? cpu->r.ax : handle;
}
