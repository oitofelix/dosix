/*
  _string.h -- String handling

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

#ifndef _INC__STRING
#define _INC__STRING
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

char * _strlwr (char *string);
#define _fstrlwr _strlwr
#define strlwr _strlwr
#define fstrlwr _strlwr

char * _strupr (char *string);
#define _fstrupr _strupr
#define strupr _strupr
#define fstrupr _strupr

#define _strdup strdup

#ifdef __cplusplus
}
#endif

#endif
