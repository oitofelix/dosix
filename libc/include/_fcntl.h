/*
  _fcntl.h -- File Control Operations

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

#ifndef _INC__FCNTL
#define _INC__FCNTL
#include <fcntl.h>
#define _O_RDONLY    0x0000 /* open for reading only */
#define _O_WRONLY    0x0001 /* open for writing only */
#define _O_RDWR      0x0002 /* open for reading and writing */
#define _O_APPEND    0x0008 /* writes done at eof */
#define _O_NOINHERIT 0x0080 /* child process doesn't inherit file */
#define _O_CREAT     0x0100 /* create and open file */
#define _O_TRUNC     0x0200 /* open and truncate */
#define _O_EXCL      0x0400 /* open only if file doesn't already exist */
/* _O_TEXT files have <cr><lf> sequences translated to <lf> on
   read()'s, and <lf> sequences translated to <cr><lf> on write()'s */
#define _O_TEXT      0x4000 /* file mode is text (translated) */
#define _O_BINARY    0x8000 /* file mode is binary (untranslated) */
#define _O_RAW    _O_BINARY
#endif
