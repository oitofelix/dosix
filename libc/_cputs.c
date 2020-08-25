/*
  _cputs.c -- Write string to stdout

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
#include <stdio.h>
#include "_dosexterr.h"
#include "include/conio.h"

int
_cputs
(const char *string)
{
  assert (string);
  struct _DOSERROR errorinfo = {0};
  for (size_t i = 0; string[i]; i++)
    if (_putch (string[i]) == EOF)
      return _dosexterr (&errorinfo); /* TODO? better error handling */
  return 0;
}
