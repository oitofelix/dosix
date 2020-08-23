/*
  _dos_findfirst.c -- Find first matching file (DOS 2+)

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
#include <stdbool.h>
#include "_dosk_dta.h"
#include "INT.h"
#include "_dosk_get_dta_addr.h"
#include "_dosk_set_dta_addr.h"
#include "_dosexterr.h"
#include "_dos_findnext.h"
#include "include/dos.h"

static int
errfunc
(const char *filename,
 int error_code)
{
  struct _DOSERROR errorinfo = {0};
  switch (error_code)
    {
    case EACCES:
      errorinfo.exterror = EXTERR_ACCESS_DENIED;
      errorinfo.errclass = ERRCLASS_ACCESS_DENIED;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      __doskexterr_set (&errorinfo);
      break;
    case ENOENT:
      errorinfo.exterror = EXTERR_PATH_NOT_FOUND;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      __doskexterr_set (&errorinfo);
      break;
    case EIO:
      errorinfo.exterror = EXTERR_READ_FAULT;
      errorinfo.errclass = ERRCLASS_MEDIA_ERROR;
      errorinfo.action = ERRACT_RETRY_AFTER_USR_INTERV;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      __doskexterr_set (&errorinfo);
      break;
    default:		/* Get generic error handling based on libc */
      errno = error_code;
      break;
    }
  return 1;
}

unsigned
_dosk_findfirst
(char *filename,
 unsigned attrib,
 unsigned append_flag)
{
  struct _DOSERROR errorinfo = {0};
  union dta_t *cdta = _dosk_current_dta;
  glob_t *pglob = &cdta->find_t._glob;
  /* TODO: DOS expects ‘*.*’ to be interpreted as Unix’s ’*’ */
  /* TODO: DOS expects file-system to be case-insensitive */
  switch (glob (filename, GLOB_NOESCAPE | GLOB_NOSORT, &errfunc, pglob))
    {
    case 0:			/* success */
      break;
    case GLOB_ABORTED:
      globfree (pglob);
      _dosexterr (&errorinfo);
      return errorinfo.exterror;
    case GLOB_NOMATCH:
      globfree (pglob);
      errorinfo.exterror = EXTERR_FILE_NOT_FOUND;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      __doskexterr_set (&errorinfo);
      return EXTERR_FILE_NOT_FOUND;
    case GLOB_NOSPACE:
      globfree (pglob);
      errorinfo.exterror = EXTERR_INSUF_MEM;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_MEM_RELATED;
      __doskexterr_set (&errorinfo);
      return EXTERR_INSUF_MEM;
    default:			/* should never get here */
      assert (false);
    };
  assert (cdta->find_t._glob.gl_pathc > 0);
  cdta->find_t._gl_pathi = 0;
  cdta->find_t._filename = filename;
  cdta->find_t._attrib = attrib;
  return _dosk_findnext ();
}

void
_dosk86_findfirst
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FINDFIRST);
  cpu->r.ax = _dosk_findfirst (_MK_FP (cpu->r.ds, cpu->r.dx),
			       cpu->r.cx,
			       cpu->l.al);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}

unsigned
_dos_findfirst
(const char *filename,		/* Target filename */
 unsigned attrib,		/* Target attributes */
 struct _find_t *fileinfo)	/* File-information buffer */
{
  union dta_t *prev_dta = _dosk_get_dta_addr ();
  _dosk_set_dta_addr ((union dta_t *) fileinfo);
  unsigned ret = _dosk_findfirst ((char *) filename, attrib, 0);
  _dosk_set_dta_addr (prev_dta);
  if (ret) errno = ENOENT;
  return ret;
}
