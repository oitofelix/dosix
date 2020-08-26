/*
  _dosk_int21_main_dos_api.c -- Int 21 handler; main DOS API

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
#include <stddef.h>
#include "INT.h"
#include "call_syscall.h"
#include "_dosk_write_stdout.h"
#include "_dosk_set_dta_addr.h"
#include "_dosk_get_dta_addr.h"
#include "_dos_findfirst.h"
#include "_dos_findnext.h"
#include "_dos_getfileattr.h"
#include "_dos_setfileattr.h"
#include "_dos_open.h"
#include "_dos_close.h"
#include "_dos_getftime.h"
#include "_dos_setftime.h"
#include "_dos_allocmem.h"
#include "_dos_setblock.h"
#include "_dos_freemem.h"
#include "_dos_getdate.h"
#include "_dos_setdate.h"
#include "_dos_gettime.h"
#include "_dos_settime.h"
#include "_dos_setvect.h"
#include "_dos_getvect.h"
#include "_dos_creat.h"
#include "_getch.h"
#include "_putch.h"
#include "_dosexterr.h"

void
_dosk_int21_main_dos_api
(cpu_t *cpu)
{
  assert (cpu);
  syscall_t syscall = NULL;
  switch (cpu->h.ah) /* AH */
    {
    case INT21_AH_FILE_METADATA: /* 0x43 */
      switch (cpu->l.al) /* AL */
	{
	case INT21_AL_FILE_METADATA_GETFILEATTR: /* 0x00 */
	  syscall = _dosk86_getfileattr;
	  break;
	case INT21_AL_FILE_METADATA_SETFILEATTR: /* 0x01 */
	  syscall = _dosk86_setfileattr;
	  break;
	}
      break;
    case INT21_AH_FILE_TIME: /* 0x57 */
      switch (cpu->l.al) /* AL */
	{
	case INT21_AL_FILE_TIME_GETFTIME: /* 0x00 */
	  syscall = _dosk86_getftime;
	  break;
	case INT21_AL_FILE_TIME_SETFTIME: /* 0x01 */
	  syscall = _dosk86_setftime;
	  break;
	}
      break;
    case INT21_AH_GETCHE: /* 0x01 */
      syscall = _dosk86_getche;
      break;
    case INT21_AH_PUTCH: /* 0x02 */
      syscall = _dosk86_putch;
      break;
    case INT21_AH_GETCH: /* 0x08 */
      syscall = _dosk86_getch;
      break;
    case INT21_AH_WRITE_STDOUT: /* 0x09 */
      syscall = _dosk86_write_stdout;
      break;
    case INT21_AH_SET_DTA_ADDR: /* 0x1a */
      syscall = _dosk86_set_dta_addr;
      break;
    case INT21_AH_SETVECT: /* 0x25 */
      syscall = _dosk86_setvect;
      break;
    case INT21_AH_GETDATE: /* 0x2a */
      syscall = _dosk86_getdate;
      break;
    case INT21_AH_SETDATE: /* 0x2b */
      syscall = _dosk86_setdate;
      break;
    case INT21_AH_GETTIME: /* 0x2c */
      syscall = _dosk86_gettime;
      break;
    case INT21_AH_SETTIME: /* 0x2d */
      syscall = _dosk86_settime;
      break;
    case INT21_AH_GET_DTA_ADDR: /* 0x2f */
      syscall = _dosk86_get_dta_addr;
      break;
    case INT21_AH_GETVECT: /* 0x35 */
      syscall = _dosk86_getvect;
      break;
    case INT21_AH_CREAT: /* 0x3c */
      syscall = _dosk86_creat;
      break;
    case INT21_AH_OPEN:	/* 0x3d */
      syscall = _dosk86_open;
      break;
    case INT21_AH_CLOSE: /* 0x3e */
      syscall = _dosk86_close;
      break;
    case INT21_AH_ALLOCMEM: /* 0x48 */
      syscall = _dosk86_allocmem;
      break;
    case INT21_AH_FREEMEM: /* 0x49 */
      syscall = _dosk86_freemem;
      break;
    case INT21_AH_SETBLOCK: /* 0x4a */
      syscall = _dosk86_setblock;
      break;
    case INT21_AH_FINDFIRST: /* 0x4e */
      syscall = _dosk86_findfirst;
      break;
    case INT21_AH_FINDNEXT: /* 0x4f */
      syscall = _dosk86_findnext;
      break;
    case INT21_AH_EXTERR: /* 0x59 */
      switch (cpu->h.bh) /* BH */
	{
	case INT21_BH_EXTERR: /* 0x00 */
	  switch (cpu->l.bl) /* BL */
	    {
	    case INT21_BL_EXTERR: /* 0x00 */
	      syscall = _dosk86exterr;
	      break;
	    }
	  break;
	}
      break;
    case INT21_AH_CREATNEW: /* 0x5b */
      syscall = _dosk86_creatnew;
      break;
    }
  call_syscall (INT21_MAIN_DOS_API,
		cpu,
		syscall);
}
