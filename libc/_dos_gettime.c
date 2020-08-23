/*
  _dos_gettime.c -- Get system time (DOS 1+)

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
#include "timeconv.h"
#include "cpu.h"
#include "INT.h"
#include "include/dos.h"

void
_dos_gettime
(struct _dostime_t *time)
{
  assert (time);
  struct timeval tv;
  if (gettimeofday (&tv, NULL))
    return;			/* fail silently */
  __dostime_struct (&tv, NULL, time);
}

void
_dosk86_gettime
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GETTIME);
  struct _dostime_t time;
  _dos_gettime (&time);
  cpu->h.ch = time.hour;
  cpu->l.cl = time.minute;
  cpu->h.dh = time.second;
  cpu->l.dl = time.hsecond;
}
