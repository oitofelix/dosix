/*
  _putch.c -- Write a single character to the console

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
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "INT.h"
#include "include/dos.h"

int
_putch
(int c)
{
  char ch = c;
  ssize_t count = 0;
  while (count >= 0 && count < sizeof (ch))
    count = TEMP_FAILURE_RETRY (write (STDOUT_FILENO,
				       &ch,
				       sizeof (ch)));
  return count < 0
    ? EOF
    : c;
}

void
_dosk86_putch
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_PUTCH);
  cpu->l.al = _putch (cpu->l.dl);
}
