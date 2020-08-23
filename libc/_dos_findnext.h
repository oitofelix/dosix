/*
  _dos_findnext.h -- Find next matching file (DOS 2+)

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

#ifndef _INC_FINDNEXT
#define _INC_FINDNEXT

#include "include/dos.h"

unsigned
_dosk_findnext
(void);

void
_dosk86_findnext
(cpu_t *cpu);

unsigned
_dos_findnext
(struct _find_t *fileinfo);

#endif
