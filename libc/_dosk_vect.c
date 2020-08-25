/*
  _dosk_vect.c -- Interrupt vector table

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

#include "INT.h"
#include "_dosk_int21_main_dos_api.h"
#include "_dosk_int2f_multiplex.h"
#include "include/dos.h"

syscall_t _dosk_vect[UINT8_MAX] =
  {
   [INT21_MAIN_DOS_API] = _dosk_int21_main_dos_api,
   [INT2F_MULTIPLEX] = _dosk_int2f_multiplex
  };
