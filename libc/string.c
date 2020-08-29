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

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <dosix/malloc.h>
#include <dosix/ctype.h>
#include <dosix/string.h>


/* strlen */
size_t
_dosix_strlen
(const char *string)
{
  assert (string);
  return strlen (string);
}

size_t
_dosix__fstrlen
(const char __far *string)
{
  assert (string);
  return
    _dosix_strlen ((const char *) string);
}


/* _strlwr functions */

char *
_dosix__strlwr
(char *string)
{
  assert (string);
  size_t len = _dosix_strlen (string);
  for (size_t i = 0; i < len; i++)
    string[i] = _dosix_tolower (string[i]);
  return string;
}

char __far *
__far
_dosix__fstrlwr
(char __far *string)
{
  assert (string);
  return (char __far *)
    _dosix__strlwr ((char *) string);
}


/* _strupr functions */

char *
_dosix__strupr
(char *string)
{
  assert (string);
  size_t len = _dosix_strlen (string);
  for (size_t i = 0; i < len; i++)
    string[i] = _dosix_toupper (string[i]);
  return string;
}

char __far *
__far
_dosix__fstrupr
(char __far *string)
{
  assert (string);
  return (char __far *)
    _dosix__strupr ((char *) string);
}


/* strcpy functions */

char *
_dosix_strcpy
(char *string1,
 const char *string2)
{
  assert (string1), assert (string2);
  return strcpy (string1, string2);
}

char __far *
__far
_dosix__fstrcpy
(char __far *string1,
 const char __far *string2)
{
  assert (string1), assert (string2);
  return _dosix_strcpy ((char *) string1,
			(const char *) string2);
}


/* strcat functions */

char *
_dosix_strcat
(char *string1,
 const char *string2)
{
  assert (string1), assert (string2);
  return strcat (string1, string2);
}

char __far *
__far
_dosix__fstrcat
(char __far *string1,
 const char __far *string2)
{
  assert (string1), assert (string2);
  return _dosix_strcat ((char *) string1,
			(const char *) string2);
}


/* _strdup functions */
char *
_dosix__strdup
(const char *string)
{
  assert (string);
  size_t len = _dosix_strlen (string);
  char *new_string = _dosix_malloc (len + 1);
  return _dosix_strcpy (new_string, string);
}

char __far *
__far
_dosix__fstrdup
(const char __far *string)
{
  assert (string);
  return (char __far *)
    _dosix__strdup ((const char *) string);
}

char __near *
__far
_dosix__nstrdup
(const char __far *string)
{
  assert (string);
  return (char __near *)
    _dosix__strdup ((const char *) string);
}
