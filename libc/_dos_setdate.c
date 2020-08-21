/*
  _dos_setdate.c -- Set system date (DOS 1+)

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
#include <sys/time.h>
#include "timeconv.h"
#include "_dosexterr.h"
#include "INT.h"
#include "include/dos.h"

unsigned
_dos_setdate
(struct _dosdate_t *date)
{
  assert (date);
  struct _DOSERROR errorinfo = {0};
  struct _dostime_t time;
  _dos_gettime (&time);
  struct timeval tv;
  unsigned err = __unixtime_struct (date, &time, &tv);
  if (err) return err;
  if (settimeofday (&tv, NULL))
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }
  return 0;
}

int
_dosk86_setdate
(union _REGS *inregs,
 union _REGS *outregs,
 struct _SREGS *segregs)
{
  assert (inregs), assert (outregs);
  assert (inregs->h.ah == INT21_AH_SETDATE);
  struct _dosdate_t date =
    {
     .year = inregs->x.cx,
     .month = inregs->h.dh,
     .day = inregs->h.dl
    };
  /* AL = result
     00h successful
     FFh invalid time, system time unchanged */
  outregs->h.al = _dos_setdate (&date) ? 0xff : 0x00;
  return outregs->x.ax;
}
