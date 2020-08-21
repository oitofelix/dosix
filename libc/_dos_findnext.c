/*
  _dos_findnext.c -- Find next matching file (DOS 2+)

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

#include <errno.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include "_dosk_dta.h"
#include "_dos_getfileattr.h"
#include "timeconv.h"
#include "_dosexterr.h"
#include "INT.h"
#include "_dosk_get_dta_addr.h"
#include "_dosk_set_dta_addr.h"
#include "include/dos.h"

static struct _DOSERROR errorinfo;

unsigned
_dosk_findnext
(void)
{
  union dta_t *cdta = _dosk_current_dta;
  for (;
       cdta->find_t._gl_pathi < cdta->find_t._glob.gl_pathc;
       cdta->find_t._gl_pathi++)
    {
      char *filename = cdta->find_t._glob.gl_pathv[cdta->find_t._gl_pathi];
      unsigned attrib;
      if (_dos_getfileattr (filename, &attrib))
	continue; /* ignore files for which attributes can’t be queried */
      struct stat fs;
      if (stat (filename, &fs))
	continue;		/* ignore files that can’t be stat’ed */
      /* check if file matches the given attributes */
      if (cdta->find_t._attrib & attrib
	  || (! S_ISDIR (fs.st_mode)
	      && (cdta->find_t._attrib & (_A_HIDDEN | _A_RDONLY
					  | _A_SUBDIR | _A_SYSTEM)
		  || ! cdta->find_t._attrib)))
	{
	  cdta->find_t.attrib = attrib;
	  unsigned err = __dostime_int (&fs.st_mtime,
					&cdta->find_t.wr_date,
					&cdta->find_t.wr_time);
	  if (err) return err;
	  cdta->find_t.size = fs.st_size;
	  memccpy (cdta->find_t.name, filename, 0,
		   sizeof (cdta->find_t.name));	/* memcpy is safer than strcpy */

	  cdta->find_t._gl_pathi++;
	  return 0;
	}
    };

  assert (cdta->find_t._gl_pathi
	  == cdta->find_t._glob.gl_pathc);

  /* Notice that if the caller does’t consume all results the
     glob_t object will leak */
  globfree (&cdta->find_t._glob);
  errorinfo.exterror = EXTERR_NO_MORE_FILES;
  errorinfo.errclass = ERRCLASS_NOT_FOUND;
  errorinfo.action = ERRACT_IGNORE;
  errorinfo.locus = ERRLOCUS_BLOCK_DEV;
  __doskexterr_set (&errorinfo);
  return errorinfo.exterror;
}

int
_dosk86_findnext
(union _REGS *inregs,
 union _REGS *outregs,
 struct _SREGS *segregs)
{
  assert (inregs), assert (outregs);
  assert (inregs->h.ah == INT21_AH_FINDNEXT);
  outregs->x.ax = _dosk_findnext ();
  outregs->x.cflag = outregs->x.ax ? ~0 : 0;
  return outregs->x.ax;
}

unsigned
_dos_findnext
(struct _find_t *fileinfo)	/* File-information buffer */
{
  union dta_t *prev_dta = _dosk_get_dta_addr ();
  _dosk_set_dta_addr ((union dta_t *) fileinfo);
  unsigned ret = _dosk_findnext ();
  _dosk_set_dta_addr (prev_dta);
  if (ret) errno = ENOENT;
  return ret;
}
