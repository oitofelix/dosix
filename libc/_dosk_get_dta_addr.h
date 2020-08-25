/*
  _dosk_get_dta_addr.h -- Get disk transfer area address (DOS 2+)

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

#ifndef _INC__DOSK_GET_DTA_ADDR
#define _INC__DOSK_GET_DTA_ADDR

union dta_t *
_dosk_get_dta_addr
(void);

void
_dosk86_get_dta_addr
(cpu_t *cpu);

#endif
