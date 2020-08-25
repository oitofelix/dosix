/*
  intr.c -- Execute 8086 interrupt; intr interface

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

#include "interrupt.h"
#include "include/dos.h"

void
intr
(int intnum,
 union REGPACK *regs)
{
  cpu_t cpu = {0};
  if (regs)
    cpu = (cpu_t)
      {
       .r =
       {
	.ax = regs->x.ax,
	.bx = regs->x.bx,
	.cx = regs->x.cx,
	.dx = regs->x.dx,
	.bp = regs->x.bp,
	.si = regs->x.si,
	.di = regs->x.di,
	.ds = regs->x.ds,
	.es = regs->x.es,
	.flags = regs->x.flags
       }
      };
  interrupt (intnum, &cpu);
}
