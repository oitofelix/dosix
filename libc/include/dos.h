/*
  dos.h -- DOS C library

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

#ifndef _INC_DOS
#define _INC_DOS

#include <stdint.h>
#include <sys/types.h>
#include <limits.h>
#include <glob.h>

#ifdef __cplusplus
extern "C" {
#endif

/*** 8086 CPU registers ***/
#ifndef _REGS_DEFINED
#define _REGS_DEFINED

typedef struct _WORDREGS
{
  uintmax_t ax, bx, cx, dx;
  uintmax_t si, di;
  uintmax_t cflag;
} _WORDREGS;

typedef struct _BYTEREGS
{
  uintmax_t al : 1 * 8;
  uintmax_t ah : (sizeof (uintmax_t) - 1) * 8;
  uintmax_t bl : 1 * 8;
  uintmax_t bh : (sizeof (uintmax_t) - 1) * 8;
  uintmax_t cl : 1 * 8;
  uintmax_t ch : (sizeof (uintmax_t) - 1) * 8;
  uintmax_t dl : 1 * 8;
  uintmax_t dh : (sizeof (uintmax_t) - 1) * 8;
} _BYTEREGS;

typedef union _REGS
{
  struct _WORDREGS r;
  struct _WORDREGS x;
  struct _BYTEREGS h;
  struct _BYTEREGS l;
} _REGS;

typedef struct _SREGS
{
  uintmax_t es;
  uintmax_t cs;
  uintmax_t ss;
  uintmax_t ds;
} _SREGS;

#define WORDREGS _WORDREGS
#define BYTEREGS _BYTEREGS
#define REGS _REGS
#define SREGS _SREGS

#endif

/*** Pointer macros  ***/

/* Make a far pointer */
#define _MK_FP(seg,offset) ((void *) ((uintptr_t) (offset)))
/* Return segment of far pointer */
#define _FP_SEG(address) ((uintptr_t) 0)
/* Return offset of far pointer */
#define _FP_OFF(address) ((uintptr_t) (address))

#define MK_FP _MK_FP
#define FP_SEG _FP_SEG
#define FP_OFF _FP_OFF

/*** DOS find functions ***/

#ifndef _FIND_T_DEFINED
#define _FIND_T_DEFINED

struct _find_t
  {
    /* Private */
    char *_filename;		/* Search template */
    unsigned _attrib;		/* Search attribute */
    glob_t _glob; 		/* Directory stream */
    size_t _gl_pathi;		/* Current index in _glob.gl_path */

    /* Public */
    unsigned attrib;		/* Attribute set for matched path */
    /*
      For the time format the listed bits have the given contents:

      - 00--04: Number of 2-second increments (0--29)
      - 05--10: Minutes (0--59)
      - 11--15: Hours (0--23)

      This element is in DOS format and is not usable by any other C
      run-time function.
    */
    unsigned wr_time;		/* Time of last write operation to
				   file */
    /*
      For the date format the listed bits have the given contents:

      - 00--04: Day of month (1--31)
      - 05--08: Month (1--12)
      - 09--15: Year (relative to 1980)

      This element is in DOS format and is not usable by any other C
      run-time function.
    */
    unsigned wr_date;		/* Date of last write operation to
				   file */
    off_t size;			/* Length of file in bytes */
    char name[NAME_MAX];		/* Null-terminated name of matched
					   file or directory, without the
					   path */
};

#define find_t _find_t
#endif

/* File attributes */
#define _A_NORMAL 0x00
#define _A_RDONLY 0x01
#define _A_HIDDEN 0x02
#define _A_SYSTEM 0x04
#define _A_VOLID 0x08
#define _A_EXEC 0x08		/* Novell NetWare */
#define _A_SUBDIR 0x10
#define _A_ARCH 0x20
#define _A_UNUSED 0x40
#define _A_DELETED 0x80		/* Novell DOS / OpenDOS */
#define _A_SHAREABLE 0x80	/* Novell NetWare */

unsigned
_dos_findfirst
(const char *filename,
 unsigned attrib,
 struct _find_t *fileinfo);

unsigned
_dos_findnext
(struct _find_t *fileinfo);

/*** dosexterr ***/
#ifndef _DOSERROR_DEFINED
#define _DOSERROR_DEFINED

struct _DOSERROR
{
  int exterror;
  char errclass;
  char action;
  char locus;
};

#if ! defined (__cplusplus)
struct DOSERROR
{
  int exterror;
  char class;
  char action;
  char locus;
};

int
dosexterr
(struct DOSERROR *);
#endif

int
_dosexterr
(struct _DOSERROR *);

/*** date and time ***/

#ifndef _DATETIME_T_DEFINED

#define _DATETIME_T_DEFINED

struct _dosdate_t
{
  unsigned char day;		/* 01--31 */
  unsigned char month;		/* 01--12 */
  unsigned int year;		/* 1980--2099 */
  unsigned char dayofweek;	/* 0--6, 0=Sunday */
};

struct _dostime_t
{
  unsigned char hour;		/* 0--23 */
  unsigned char minute;		/* 0--59 */
  unsigned char second;		/* 0--59 */
  unsigned char hsecond;	/* 0--99 */
};

#define dosdate_t _dosdate_t
#define dostime_t _dostime_t

#endif

int
_int86x
(int intnum,
 union _REGS *inregs,
 union _REGS *outregs,
 struct _SREGS *segregs);

#define int86x _int86x

int
_int86
(int intnum,
 union _REGS *inregs,
 union _REGS *outregs);

#define int86 _int86

int
_intdosx
(union _REGS *inregs,
 union _REGS *outregs,
 struct _SREGS *segregs);

#define intdosx _intdosx

int
_intdos
(union _REGS *inregs,
 union _REGS *outregs);

#define intdos _intdos

int
_bdos
(int dosfunc,
 unsigned int dosdx,
 unsigned int dosal);

#define bdos _bdos

unsigned
_dos_getfileattr
(const char *path,
 unsigned *attrib);

#define dos_getfileattr _dos_getfileattr

unsigned
_dos_setfileattr
(const char *path,
 unsigned attrib);

#define dos_setfileattr _dos_setfileattr

unsigned
_dos_open
(const char *path,
 unsigned mode,
 int *handle);

#define dos_open _dos_open

unsigned
_dos_close
(int handle);

#define dos_close _dos_close

unsigned
_dos_getftime
(int handle,
 unsigned *date,
 unsigned *time);

#define dos_getftime _dos_getftime

unsigned
_dos_setftime
(int handle,
 unsigned date,
 unsigned time);

#define dos_setftime _dos_setftime

unsigned
_dos_allocmem
(size_t size,
 uintptr_t *seg);

#define dos_allocmem _dos_allocmem

unsigned
_dos_setblock
(size_t size,
 uintptr_t seg,
 size_t *maxsize);

#define dos_setblock _dos_setblock

unsigned
_dos_freemem
(uintptr_t seg);

#define dos_freemem _dos_freemem

void
_dos_getdate
(struct _dosdate_t *date);

#define dos_getdate _dos_getdate

void
_dos_gettime
(struct _dostime_t *time);

#define dos_gettime _dos_gettime

unsigned
_dos_settime
(struct _dostime_t *time);

#define dos_settime _dos_settime

unsigned
_dos_setdate
(struct _dosdate_t *date);

#define dos_setdate _dos_setdate

#endif

/* Fixes */

#define __far
#define _far __far
#define far _far
#define __near
#define _near __near
#define near _near
#define __cdecl
#define _cdecl __cdecl
#define cdecl _cdecl

/* Unofficial portability aid (supposedly) */

#define _MAX_DRIVE 3		  /* max. length of drive component */
#define _MAX_PATH (PATH_MAX + 1)  /* max. length of full pathname */
#define _MAX_DIR (PATH_MAX + 1)   /* max. length of path component */
#define _MAX_FNAME (NAME_MAX + 1) /* max. length of file name component */
#define _MAX_EXT (NAME_MAX + 1)	  /* max. length of extension component */

#define MAXDRIVE _MAX_DRIVE
#define MAXPATH _MAX_PATH
#define MAXDIR _MAX_DIR
#define MAXFILE _MAX_FNAME
#define MAXEXT _MAX_EXT

#define WILDCARDS 0x01
#define EXTENSION 0x02
#define FILENAME  0x04
#define DIRECTORY 0x08
#define DRIVE     0x10

typedef struct fblock
{
  struct _find_t ff;
} FBLOCK;

#ifdef __cplusplus
}
#endif
#endif
