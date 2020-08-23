/*
  _dos_setfileattr.c -- Set file attributes (DOS 2+)

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
#include <sys/stat.h>
#include "cpu.h"
#include "INT.h"
#include "include/dos.h"

unsigned
_dos_setfileattr
(const char *path,
 unsigned attrib)
{
  struct _DOSERROR errorinfo;
  struct stat fs;
  if (stat (path, &fs))
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }

  mode_t mode = fs.st_mode;

  if (attrib & _A_RDONLY)
    mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
  else mode |= (S_IWUSR | S_IWGRP | S_IWOTH);

  if (chmod (path, mode))
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }

  return 0;
}

void
_dosk86_setfileattr
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FILE_METADATA);
  assert (cpu->l.al == INT21_AL_FILE_METADATA_SETFILEATTR);
  cpu->r.ax = _dos_setfileattr (_MK_FP (cpu->r.ds, cpu->r.dx),
				cpu->r.cx);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}
