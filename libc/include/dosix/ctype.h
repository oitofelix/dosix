/*
  ctype.h -- Character predicates and conversion

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

#ifndef _INC_DOSIX_CTYPE
#define _INC_DOSIX_CTYPE

#include <dosix/compiler.h>

#ifndef _DOSIX_LIBC_SRC
#define isalnum _dosix_isalnum
#define isalpha _dosix_isalpha
#define iscntrl _dosix_iscntrl
#define isdigit _dosix_isdigit
#define isgraph _dosix_isgraph
#define islower _dosix_islower
#define isprint _dosix_isprint
#define ispunct _dosix_ispunct
#define isspace _dosix_isspace
#define isupper _dosix_isupper
#define isxdigit _dosix_isxdigit
#define __isascii _dosix__isascii
#define __toascii _dosix__toascii
#define tolower _dosix_tolower
#define _tolower _dosix__tolower
#define toupper _dosix_toupper
#define _toupper _dosix__toupper
#define __iscsym _dosix___iscsym
#define __iscsymf _dosix___iscsymf

#ifndef __STRICT_ANSI__
#define isascii __isascii
#define toascii __toascii
#define iscsym __iscsym
#define iscsymf __iscsymf
#endif	/* ! __STRICT_ANSI__ */

#endif	/* ! _DOSIX_LIBC_SRC */

#ifdef __cplusplus
extern "C" {
#endif
  /* predicates */
  int __cdecl _dosix_isalnum (int);
  int __cdecl _dosix_isalpha (int);
  int __cdecl _dosix__isascii (int);
  int __cdecl _dosix_iscntrl (int);
  int __cdecl _dosix___iscsym (int);
  int __cdecl _dosix___iscsymf (int);
  int __cdecl _dosix_isdigit (int);
  int __cdecl _dosix_isgraph (int);
  int __cdecl _dosix_islower (int);
  int __cdecl _dosix_isprint (int);
  int __cdecl _dosix_ispunct (int);
  int __cdecl _dosix_isspace (int);
  int __cdecl _dosix_isupper (int);
  int __cdecl _dosix_isxdigit (int);
  /* conversion */
  int __cdecl _dosix__toascii (int);
  int __cdecl _dosix_tolower (int);
  int __cdecl _dosix__tolower (int);
  int __cdecl _dosix_toupper (int);
  int __cdecl _dosix__toupper (int);
#ifdef __cplusplus
}
#endif

#endif	/* ! _INC_DOSIX_CTYPE */
