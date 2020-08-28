/*
  string.c -- String handling

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

#define _DOSIX_LIBC_SRC
#define _GNU_SOURCE


/* headers */

#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <dos/string.h>


/* _strlwr functions */

char *
_strlwr
(char *string)
{
  size_t len = strlen (string);
  for (size_t i = 0; i < len; i++)
    string[i] = tolower (string[i]);
  return string;
}

char __far *
__far
_fstrlwr
(char __far *string)
{
  return (char __far *)
    _strlwr ((char *) string);
}


/* _strupr functions */

char *
_strupr
(char *string)
{
  size_t len = strlen (string);
  for (size_t i = 0; i < len; i++)
    string[i] = toupper (string[i]);
  return string;
}

char __far *
__far
_fstrupr
(char __far *string)
{
  return (char __far *)
    _strupr ((char *) string);
}

/*  */
/* /\* _strdup functions *\/ */
/* char * */
/* _strdup */
/* (const char *string) */
/* { */
/*   size_t len = strlen (string); */
/*   char *new_string = _dosix_malloc (); */
/* } */
