/*
  _dosk_int21_main_dos_api.h -- Int 21 handler; main DOS API

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

#ifndef _INC__DOSK_INT21_MAIN_DOS_API
#define _INC__DOSK_INT21_MAIN_DOS_API

#include "include/dos.h"

void
_dosk_int21_main_dos_api
(cpu_t *cpu);

#endif
