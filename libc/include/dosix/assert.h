/*
  assert.h -- Assert macro

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

#ifndef _INC_DOSIX_ASSERT
#define _INC_DOSIX_ASSERT

#include <stddef.h>
#include <dosix/compiler.h>

#ifndef _DOSIX_LIBC_SRC
#  undef assert
#  define assert _dosix_assert
#endif	/* ! DOSIX_LIBC_SRC */

#ifdef NDEBUG
#  define _dosix_assert(expression) ((void) 0)
#else
#define _dosix_assert(expression)			\
  ((expression)						\
   ? (void) 0						\
   : _dosix__assert (#expression, __FILE__, __LINE__))
#  ifdef __cplusplus
extern "C" {
#  endif
  void __cdecl _dosix__assert (const char *, const char *, size_t);
#  ifdef __cplusplus
}
#  endif
#endif /* ! NDEBUG */

#endif	/* ! _INC_DOSIX_ASSERT */
