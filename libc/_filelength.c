/*
  _filelength.c -- Return length of file associated with handle

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

#include <sys/stat.h>
#include "_dosexterr.h"

off_t
_filelength
(int handle)
{
  struct _DOSERROR errorinfo = {0};
  struct stat fs;
  if (fstat (handle, &fs))
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return -1;
    }
  return fs.st_size;
}
