/*
  _dosk_int2f_multiplex.c -- Int 2F handler; multiplex

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
#include "INT.h"
#include "call_syscall.h"
#include "_dosexterr.h"

void
_dosk_int2f_multiplex
(cpu_t *cpu)
{
  assert (cpu);
  syscall_t syscall = NULL;
  switch (cpu->h.ah) /* AH */
    {
    case INT2F_AH_DOS_INTERNAL: /* 0x12 */
      switch (cpu->l.al) /* AL */
	{
	case INT2F_AL_DOS_INTERNAL_EXTERR_SET: /* 0x22 */
	  syscall = __dosk86exterr_set;
	  break;
	}
      break;
    };
  call_syscall (INT2F_MULTIPLEX,
		cpu,
		syscall);
}
