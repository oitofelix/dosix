/*
  call_syscall.c -- Call system calls

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

#include <err.h>
#include "include/dos.h"

void
call_syscall
(uint8_t intnum,
 cpu_t *cpu,
 syscall_t syscall)
{
  if (! syscall)
    errx (86,
	  "Not implemented: "
	  "INT %02Xh "
	  "AX=%04Xh "
	  "BX=%04Xh "
	  "CX=%04Xh "
	  "DX=%04Xh ",
	  intnum,
	  cpu->r.ax,
	  cpu->r.bx,
	  cpu->r.cx,
	  cpu->r.dx);
  else
    syscall (cpu);
}
