/*
  _dosk_write_stdout.c -- Write string to standard output (DOS 1+)

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
#include <stddef.h>
#include <stdio.h>
#include "include/dos.h"
#include "INT.h"

unsigned
_dosk_write_stdout
(const char *string)
{
  for (size_t i = 0; string[i] != '$'; i++)
    {
      int ret = putchar (string[i]);
      if (ret == EOF) break;
    }
  return '$';
}

int
_dosk86_write_stdout
(union _REGS *inregs,
 union _REGS *outregs,
 struct _SREGS *segregs)
{
  assert (inregs), assert (outregs), assert (segregs);
  assert (inregs->h.ah == INT21_AH_WRITE_STDOUT);
  outregs->x.ax = _dosk_write_stdout (_MK_FP (segregs->ds,
					      inregs->x.dx));
  return outregs->x.ax;
}
