/*
  _dosk_set_dta_addr.c -- Set disk transfer area address (DOS 1+)

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
#include "_dosk_dta.h"
#include "cpu.h"
#include "INT.h"
#include "include/dos.h"

/* The DTA is set to &dta when a program is started.  See
   dta.h. */
void
_dosk_set_dta_addr
(union dta_t *target_dta)
{
  _dosk_current_dta = target_dta;
}

void
_dosk86_set_dta_addr
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_SET_DTA_ADDR);
  _dosk_set_dta_addr (_MK_FP (cpu->r.ds, cpu->r.dx));
}
