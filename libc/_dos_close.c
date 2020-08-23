/*
  _dos_close.c -- Close file (DOS 2+)

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
#include <unistd.h>
#include "_dosexterr.h"
#include "INT.h"

unsigned
_dos_close
(int handle)
{
  struct _DOSERROR errorinfo = {0};
  if (close (handle))
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }
  return 0;
}

void
_dosk86_close
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_CLOSE);
  cpu->r.ax = _dos_close (cpu->r.bx);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}
