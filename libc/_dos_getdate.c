/*
  _dos_getdate.c -- Get system date (DOS 1+)

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
#include <sys/time.h>
#include "INT.h"
#include "timeconv.h"
#include "include/dos.h"

void
_dos_getdate
(struct _dosdate_t *date)
{
  assert (date);
  struct timeval tv;
  if (gettimeofday (&tv, NULL))
    return;			/* fail silently */
  __dostime_struct (&tv, date, NULL);
}

void
_dosk86_getdate
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GETDATE);
  struct _dosdate_t date;
  _dos_getdate (&date);
  cpu->r.cx = date.year;
  cpu->h.dh = date.month;
  cpu->l.dl = date.day;
  cpu->l.al = date.dayofweek; /* DOS 1.10+ */
}
