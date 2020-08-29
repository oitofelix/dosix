/*
  ctype.c -- Character predicates and conversion

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
#include <ctype.h>
#include <dosix/ctype.h>


/* predicates */

int
_dosix_isalnum
(int c)
{
  return isalnum (c);
}

int
_dosix_isalpha
(int c)
{
  return isalpha (c);
}

int
_dosix__isascii
(int c)
{
  return isascii (c);
}

int
_dosix_iscntrl
(int c)
{
  return iscntrl (c);
}

int
_dosix___iscsym
(int c)
{
  return
    _dosix_isalnum (c)
    || c == '_';
}

int
_dosix___iscsymf
(int c)
{
  return
    _dosix_isalpha (c)
    || c == '_';
}

int
_dosix_isdigit
(int c)
{
  return isdigit (c);
}

int
_dosix_isgraph
(int c)
{
  return isgraph (c);
}

int
_dosix_islower
(int c)
{
  return islower (c);
}

int
_dosix_isprint
(int c)
{
  return isprint (c);
}

int
_dosix_ispunct
(int c)
{
  return ispunct (c);
}

int
_dosix_isspace
(int c)
{
  return isspace (c);
}

int
_dosix_isupper
(int c)
{
  return isupper (c);
}

int
_dosix_isxdigit
(int c)
{
  return isxdigit (c);
}


/* conversion */

int
_dosix__toascii
(int c)
{
  return toascii (c);
}

int
_dosix_tolower
(int c)
{
  return tolower (c);
}

int
_dosix__tolower
(int c)
{
  return _tolower (c);
}

int
_dosix_toupper
(int c)
{
  return toupper (c);
}

int
_dosix__toupper
(int c)
{
  return _toupper (c);
}
