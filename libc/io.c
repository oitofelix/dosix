/*
  io.c -- Low-level file handling and I/O functions

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

#include <sys/stat.h>
#include <io.h>


/* _filelength */

off_t
_filelength
(int handle)
{
  struct stat fs;
  if (fstat (handle, &fs))
      return -1L;
  return fs.st_size;
}
