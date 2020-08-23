/*
  _int86x.c -- Execute 8086 interrupt passing most registers

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

#include "cpu.h"
#include "interrupt.h"
#include "include/dos.h"
#include "include/_stdlib.h"

int
_int86x
(int intnum,			/* Interrupt number */
 union _REGS *inregs,		/* Register values on call */
 union _REGS *outregs,		/* Register values on return  */
 struct _SREGS *segregs)	/* Segment-register values on call  */
{
  cpu_t cpu = {0};
  if (inregs)
    cpu = (cpu_t)
      {
       .r.ax = inregs->x.ax,
       .r.bx = inregs->x.bx,
       .r.cx = inregs->x.cx,
       .r.dx = inregs->x.dx,
       .r.si = inregs->x.si,
       .r.di = inregs->x.di,
       .r.flags = inregs->x.cflag
      };
  if (segregs)
    {
      cpu.r.cs = segregs->cs;
      cpu.r.ds = segregs->ds;
      cpu.r.es = segregs->es;
      cpu.r.ss = segregs->ss;
    };
  interrupt (intnum, &cpu);
  if (outregs)
    {
      *outregs = (_REGS)
	{
	 .x =
	 {
	  .ax = cpu.r.ax,
	  .bx = cpu.r.bx,
	  .cx = cpu.r.cx,
	  .dx = cpu.r.dx,
	  .si = cpu.r.si,
	  .di = cpu.r.di,
	  .cflag = cpu.r.flags
	 }
	};
      if (cpu.r.flags)
	_doserrno = cpu.r.ax;
    }
  if (segregs)
    *segregs = (_SREGS)
      {
       .es = cpu.r.es,
       .cs = cpu.r.cs,
       .ss = cpu.r.ss,
       .ds = cpu.r.ds
      };
  return cpu.r.ax;
}
