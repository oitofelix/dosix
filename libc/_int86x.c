/*
  _int86x.c -- Execute 8086 interrupt passing all registers

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

#include <err.h>
#include "syscall_t.h"
#include "INT.h"
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
#include "_dosexterr.h"
#include "include/dos.h"
#include "include/_stdlib.h"

int
_int86x
(int intnum,			/* Interrupt number */
 union _REGS *inregs,		/* Register values on call */
 union _REGS *outregs,		/* Register values on return  */
 struct _SREGS *segregs)	/* Segment-register values on call  */
{
  syscall_t syscall = NULL;
  switch (intnum)
    {
    case INT21_MAIN_DOS_API: /* 0x21 */
      switch (inregs->x.ax)  /* AX */
	{
	case INT21_AX_GETFILEATTR: /* 0x4300 */
	  syscall = _dosk86_getfileattr;
	  break;
	case INT21_AX_GETFTIME: /* 0x5700 */
	  syscall = _dosk86_getftime;
	  break;
	case INT21_AX_SETFTIME: /* 0x5701 */
	  syscall = _dosk86_setftime;
	  break;
	case INT21_AX_SETFILEATTR: /* 0x4301 */
	  syscall = _dosk86_setfileattr;
	  break;
	}
      switch (inregs->h.ah)	/* AH */
	{
	case INT21_AH_WRITE_STDOUT: /* 0x09 */
	  syscall = _dosk86_write_stdout;
	  break;
	case INT21_AH_SET_DTA_ADDR: /* 0x1a */
	  syscall = _dosk86_set_dta_addr;
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
	case INT21_AH_OPEN:	/* 0x3d */
	  syscall = _dosk86_open;
	  break;
	case INT21_AH_CLOSE:	/* 0x3e */
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
	  switch (inregs->x.bx) {
	  case INT21_BX_EXTERR: /* 0x0000 */
	    syscall = _dosk86exterr;
	    break;
	  }
	  break;
	}
      break;
    case INT2F_MULTIPLEX:	/* 0x2f */
      switch (inregs->x.ax)
	{
	case INT2F_AX_EXTERR_SET: /* 0x1222 */
	  syscall = __dosk86exterr_set;
	  break;
	}
      break;
    }
  if (! syscall)
    errx (86,
	  "Not implemented: INT 0x%02x AX=0x%04x BX=0x%04x "
	  "CX=0x%04x DX=0x%04x",
	  intnum,
	  inregs->x.ax,
	  inregs->x.bx,
	  inregs->x.cx,
	  inregs->x.dx);
  unsigned ax = syscall (inregs,
			 outregs,
			 segregs);
  if (outregs->x.cflag)
    _doserrno = ax;
  return ax;
}
