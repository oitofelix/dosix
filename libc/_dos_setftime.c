/*
  _dos_setftime.c -- Set file's last-written date and time (DOS 2+)

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
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "_dosexterr.h"
#include "INT.h"
#include "timeconv.h"

unsigned
_dos_setftime
(int handle,
 unsigned date,
 unsigned time)
{
  time_t _time;
  struct _DOSERROR errorinfo = {0};
  unsigned err = __unixtime_int (date, time, &_time);
  if (err) return err;
  struct stat fs;
  if (fstat (handle, &fs))
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }
  struct timeval tvp[2] =
    {
     {
      .tv_sec = fs.st_atime,
      .tv_usec = 0
     },
     {
      .tv_sec = _time,
      .tv_usec = 0
     }
    };
  if (futimes (handle, tvp))
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }
  return 0;
}

void
_dosk86_setftime
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FILE_TIME);
  assert (cpu->l.al == INT21_AL_FILE_TIME_SETFTIME);
  cpu->r.ax = _dos_setftime (cpu->r.bx,
			     cpu->r.dx,
			     cpu->r.cx);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}
