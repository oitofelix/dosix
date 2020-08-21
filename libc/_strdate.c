/*
  _strdate.c -- Current date formatted string

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
#include "include/dos.h"

char *
_strdate
(char *datestr)
{
  assert (datestr);
  struct _dosdate_t date;
  _dos_getdate (&date);
  /* let sprintf fail silently */
  sprintf (datestr,
	   "%02d/%02d/%02d",
	   date.month,
	   date.day,
	   date.year % 100);
  return datestr;
}
