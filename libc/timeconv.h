/*
  timeconv.h -- Conversion between DOS and Unix time formats

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

#ifndef _INC_TIMECONV
#define _INC_TIMECONV

#include <time.h>
#include "include/dos.h"

unsigned
__unixtime_struct
(const struct _dosdate_t *date,
 const struct _dostime_t *time,
 struct timeval *tv);

unsigned
__dostime_struct
(const struct timeval *tv,
 struct _dosdate_t *date,
 struct _dostime_t *time);

unsigned
__unixtime_int
(unsigned date,
 unsigned time,
 time_t *utime);

unsigned
__dostime_int
(time_t *utime,
 unsigned *date,
 unsigned *time);

#endif
