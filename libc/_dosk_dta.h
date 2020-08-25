/*
  _dosk_dta.h -- Disk Transfer Area (DOS 1+)

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

#ifndef _INC__DOSK_DTA
#define _INC__DOSK_DTA

#include "include/dos.h"

union dta_t {
  struct _find_t find_t;	/* Used by findfirst and findnext */
};
extern union dta_t _dosk_dta;
extern union dta_t *_dosk_current_dta;

#endif
