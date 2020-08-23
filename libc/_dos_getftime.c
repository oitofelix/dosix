/*
  _dos_getftime.c -- Get file's last-written date and time (DOS 2+)

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
#include <sys/stat.h>
#include "INT.h"
#include "_dosexterr.h"
#include "timeconv.h"

unsigned
_dos_getftime
(int handle,
 unsigned *date,
 unsigned *time)
{
  struct _DOSERROR errorinfo = {0};
  struct stat fs;
  if (fstat (handle, &fs))
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }
  unsigned err = __dostime_int (&fs.st_mtime,
				date,
				time);
  return err ? err: 0;
}

void
_dosk86_getftime
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FILE_TIME);
  assert (cpu->l.al == INT21_AL_FILE_TIME_GETFTIME);
  unsigned date, time;
  cpu->r.ax = _dos_getftime (cpu->r.bx,
			     &date,
			     &time);
  cpu->r.cx = time;
  cpu->r.dx = date;
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}
