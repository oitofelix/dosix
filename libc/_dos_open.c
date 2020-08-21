/*
  _dos_open.c -- Open existing file (DOS 2+)

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
#include "INT.h"
#include "include/dos.h"
#include "include/_fcntl.h"
#include "include/share.h"

unsigned
_dos_open
(const char *path,
 unsigned mode,
 int *handle)
{
  assert (path), assert (handle);
  struct _DOSERROR errorinfo = {0};
  int flags = (mode & _O_RDONLY ? O_RDONLY : 0)
    | (mode & _O_WRONLY ? O_WRONLY  : 0)
    | (mode & _O_RDWR ? O_RDWR : 0);
  int fd = open (path, flags);
  if (fd < 0)
    {
      /* TODO? better error handling */
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    }
  if (! (mode & _SH_DENYNO))
    {
      struct flock flock =
	{
	 /* No use for _SH_CMPAT? */
	 .l_type = (mode & _SH_DENYRW ? F_WRLCK : 0)
	 | (mode & _SH_DENYWR ? F_RDLCK : 0)
	 | (mode & _SH_DENYRD ? F_WRLCK : 0),
	 .l_whence = SEEK_SET,
	 .l_start = 0,
	 .l_len = 0		/* Lock the entire file */
	};
      if (fcntl (fd, F_SETLK, &flock) == -1)
	{
	  /* TODO? better error handling */
	  _dosexterr (&errorinfo);
	  return errorinfo.exterror;
	}
    }
  if (mode & _O_NOINHERIT)
    {
      int flags = fcntl (fd, F_GETFD, 0);
      if (flags == -1)
	{
	  /* TODO? better error handling */
	  _dosexterr (&errorinfo);
	  return errorinfo.exterror;
	}
      flags |= FD_CLOEXEC;
      if (fcntl (fd, F_SETFD, flags) == -1)
	{
	  /* TODO? better error handling */
	  _dosexterr (&errorinfo);
	  return errorinfo.exterror;
	}
    }
  *handle = fd;
  return 0;
}

int
_dosk86_open
(union _REGS *inregs,
 union _REGS *outregs,
 struct _SREGS *segregs)
{
  assert (inregs), assert (outregs), assert (segregs);
  assert (inregs->h.ah == INT21_AH_OPEN);
  /* Documentation says:

     CL = attribute mask of files to look for (server call only)

     What should we do about it?  Now it’s simply ignored.
 */
  int handle;
  outregs->x.ax = _dos_open (_MK_FP (segregs->ds, inregs->x.dx),
			     inregs->h.al,
			     &handle);
  outregs->x.cflag = outregs->x.ax ? ~0 : 0;
  if (! outregs->x.ax)
    outregs->x.ax = handle;
  return outregs->x.ax;
}
