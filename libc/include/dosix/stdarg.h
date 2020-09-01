/*
  stdarg.h -- Support for variable argument functions (ISO)

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

#ifndef _INC_DOSIX_STDARG
#define _INC_DOSIX_STDARG

#include <stdarg.h>
#include <dosix/compiler.h>

#define _dosix_va_start va_start
#define _dosix_va_arg va_arg
#define _dosix_va_end va_end

#endif	/* ! _INC_DOSIX_STDARG */
