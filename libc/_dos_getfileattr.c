/*
  _dos_getfileattr.c -- Get file attributes (DOS 2+)

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

#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "INT.h"
#include "include/dos.h"

unsigned
_dos_getfileattr
(const char *path,
 unsigned *attrib)
{
  struct _DOSERROR errorinfo = {0};
  struct stat fs;
  if (stat (path, &fs))
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }
  unsigned _attrib = 0;
  if (S_ISDIR (fs.st_mode))
    _attrib = _A_SUBDIR;
  else
    {
      _attrib = _A_NORMAL | _A_ARCH;
      if (access (path, W_OK))
	switch (errno)
	  {
	  case EROFS:
	  case EACCES:
	    _attrib |= _A_RDONLY;
	    break;
	  case ENOENT:
	  default:
	    _dosexterr (&errorinfo);
	    return errorinfo.exterror;
	  }
    }
  *attrib = _attrib;
  return 0;
}

int
_dosk86_getfileattr
(union _REGS *inregs,
 union _REGS *outregs,
 struct _SREGS *segregs)
{
  assert (inregs), assert (outregs), assert (segregs);
  assert (inregs->x.ax == INT21_AX_GETFILEATTR);
  unsigned attrib;
  outregs->x.ax = _dos_getfileattr (_MK_FP (segregs->ds, inregs->x.dx),
				    &attrib);
  outregs->x.cx = attrib;
  outregs->x.cflag = outregs->x.ax ? ~0 : 0;
  return outregs->x.ax;
}
