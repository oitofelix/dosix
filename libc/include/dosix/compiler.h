/*
  compiler.h -- Compiler compatibility macros

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

#ifndef _INC_DOS_COMPILER
#define _INC_DOS_COMPILER

#define __huge
#define _huge __huge
#define __cdecl
#define _cdecl __cdecl
#define __based(type)
#define _based(type) __based(type)
#define __far
#define _far __far
#define far _far
#define __near
#define _near __near
#define near _near
#define __segment void *
#define _segment __segment

#endif	/* ! _INC_DOS_COMPILER */
