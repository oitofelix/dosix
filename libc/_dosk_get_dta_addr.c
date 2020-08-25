/*
  _dosk_get_dta_addr.c -- Get disk transfer area address (DOS 2+)

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
#include "INT.h"
#include "include/dos.h"

union dta_t *
_dosk_get_dta_addr
(void)
{
  return _dosk_current_dta;
}

void
_dosk86_get_dta_addr
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GET_DTA_ADDR);
  union dta_t *dta_ptr = _dosk_get_dta_addr ();
  cpu->r.es = _FP_SEG (dta_ptr);
  cpu->r.bx = _FP_OFF (dta_ptr);
}
