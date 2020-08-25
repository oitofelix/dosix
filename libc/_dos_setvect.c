/*
  _dos_setvect.c -- Set interrupt vector (DOS 1+)

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
#include "_dosk_vect.h"
#include "INT.h"
#include "include/dos.h"

void
_dos_setvect
(unsigned intnum,
 syscall_t syscall)
{
  assert (syscall);
  assert (intnum < UINT8_MAX);
  _dosk_vect[intnum] = syscall;
}

void
_dosk86_setvect
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_SETVECT);
  _dos_setvect (cpu->l.al,
		_MK_FP (cpu->r.ds, cpu->r.dx));
}
