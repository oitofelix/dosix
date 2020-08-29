/*
  dos.h -- DOS interface routines

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
#include <dosix/compiler.h>

#ifndef _DOSIX_LIBC_SRC
#define _dos_findfirst _dosix__dos_findfirst
#define _dos_findnext _dosix__dos_findnext
#define _dos_findclose _dosix__dos_findclose
#define _dos_getfileattr _dosix__dos_getfileattr
#define _dos_setfileattr _dosix__dos_setfileattr
#define _dos_open _dosix__dos_open
#define _dos_creat _dosix__dos_creat
#define _dos_creatnew _dosix__dos_creatnew
#define _dos_close _dosix__dos_close
#define _dos_getftime _dosix__dos_getftime
#define _dos_setftime _dosix__dos_setftime
#define _dos_allocmem _dosix__dos_allocmem
#define _dos_setblock _dosix__dos_setblock
#define _dos_freemem _dosix__dos_freemem
#define _dos_getdate _dosix__dos_getdate
#define _dos_gettime _dosix__dos_gettime
#define _dos_settime _dosix__dos_settime
#define _dos_setdate _dosix__dos_setdate
#define _intdosx _dosix__intdosx
#define _intdos _dosix__intdos
#define _int86x _dosix__int86x
#define _int86 _dosix__int86
#define _bdos _dosix__bdos

#ifndef __STRICT_ANSI__
#define dos_findfirst _dos_findfirst
#define dos_findnext _dos_findnext
#define dos_findclose _dos_findclose
#define dos_getfileattr _dos_getfileattr
#define dos_setfileattr _dos_setfileattr
#define dos_open _dos_open
#define dos_creat _dos_creat
#define dos_creatnew _dos_creatnew
#define dos_close _dos_close
#define dos_getftime _dos_getftime
#define dos_setftime _dos_setftime
#define dos_allocmem _dos_allocmem
#define dos_setblock _dos_setblock
#define dos_freemem _dos_freemem
#define dos_getdate _dos_getdate
#define dos_gettime _dos_gettime
#define dos_settime _dos_settime
#define dos_setdate _dos_setdate
#define intdosx _intdosx
#define intdos _intdos
#define int86x _int86x
#define int86 _int86
#define bdos _bdos

#define WORDREGS _WORDREGS
#define BYTEREGS _BYTEREGS
#define REGS _REGS
#define SREGS _SREGS

#define MK_FP _MK_FP
#define FP_SEG _FP_SEG
#define FP_OFF _FP_OFF

#define find_t _find_t
#define dosdate_t _dosdate_t
#define dostime_t _dostime_t
#define diskfree_t _diskfree_t
#endif	/* ! __STRICT_ANSI__ */

#endif	/* ! _DOS_LIBC_SRC */

#define _MK_FP(seg,offset) ((void *) ((((uintptr_t) (seg)) & 0) | (uintptr_t) (offset)))
#define _FP_SEG(address) ((uintptr_t) 0)
#define _FP_OFF(address) ((uintptr_t) (address))

/* File attributes */
#define _A_NORMAL 0x00
#define _A_RDONLY 0x01
#define _A_HIDDEN 0x02
#define _A_SYSTEM 0x04
#define _A_VOLID 0x08
#define _A_EXEC 0x08 /* Novell NetWare */
#define _A_SUBDIR 0x10
#define _A_ARCH 0x20
#define _A_UNUSED 0x40
#define _A_DELETED 0x80 /* Novell DOS / OpenDOS */
#define _A_SHAREABLE 0x80 /* Novell NetWare */

/* REMOVE-ME? */
#define intrpt(intnum,regs)			\
  intr (intnum, (union REGPACK *) regs)

/* Unofficial portability aid (supposedly) */

/* #define MAXDRIVE _MAX_DRIVE */
/* #define MAXPATH _MAX_PATH */
/* #define MAXDIR _MAX_DIR */
/* #define MAXFILE _MAX_FNAME */
/* #define MAXEXT _MAX_EXT */

/* #define WILDCARDS 0x01 */
/* #define EXTENSION 0x02 */
/* #define FILENAME  0x04 */
/* #define DIRECTORY 0x08 */
/* #define DRIVE     0x10 */

/*** CPU ***/
typedef struct cpu_word
{
  uintmax_t ax, bx, cx, dx;
  uintmax_t si, di, bp, sp;
  uintmax_t ip;
  uintmax_t cs, ds, es, ss;
  uintmax_t flags;
} cpu_word_t;

typedef struct cpu_byte
{
  uintmax_t al : 1 * 8;
  uintmax_t ah : (sizeof (uintmax_t) - 1) * 8;
  uintmax_t bl : 1 * 8;
  uintmax_t bh : (sizeof (uintmax_t) - 1) * 8;
  uintmax_t cl : 1 * 8;
  uintmax_t ch : (sizeof (uintmax_t) - 1) * 8;
  uintmax_t dl : 1 * 8;
  uintmax_t dh : (sizeof (uintmax_t) - 1) * 8;
} cpu_byte_t;

typedef union cpu
{
  cpu_word_t r;
  cpu_byte_t l;
  cpu_byte_t h;
} cpu_t;

/*** syscall_t ***/
typedef
void
(*syscall_t)
(cpu_t *);

/*** 8086 CPU registers ***/
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

typedef struct
{
  uintmax_t r_ax, r_bx, r_cx, r_dx;
  uintmax_t r_bp, r_si, r_di;
  uintmax_t r_ds, r_es;
  uintmax_t r_flags;
} IREGS;

union REGPACK
{
  struct
  {
    uintmax_t al : 1 * 8;
    uintmax_t ah : (sizeof (uintmax_t) - 1) * 8;
    uintmax_t bl : 1 * 8;
    uintmax_t bh : (sizeof (uintmax_t) - 1) * 8;
    uintmax_t cl : 1 * 8;
    uintmax_t ch : (sizeof (uintmax_t) - 1) * 8;
    uintmax_t dl : 1 * 8;
    uintmax_t dh : (sizeof (uintmax_t) - 1) * 8;
  } h;
  struct
  {
    uintmax_t ax, bx, cx, dx;
    uintmax_t bp, si, di;
    uintmax_t ds, es;
    uintmax_t flags;
  } x;
};

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

/* REMOVE-ME? */
typedef struct fblock
{
  struct _find_t ff;
} FBLOCK;

struct _DOSERROR
{
  int exterror;
  char errclass;
  char action;
  char locus;
};

#if ! defined (__STRICT_ANSI__) && ! defined (__cplusplus)
struct DOSERROR
{
  int exterror;
  char class;
  char action;
  char locus;
};
#endif

struct _dosdate_t
{
  unsigned char day; /* 01--31 */
  unsigned char month; /* 01--12 */
  unsigned int year; /* 1980--2099 */
  unsigned char dayofweek; /* 0--6, 0=Sunday */
};

struct _dostime_t
{
  unsigned char hour; /* 0--23 */
  unsigned char minute; /* 0--59 */
  unsigned char second; /* 0--59 */
  unsigned char hsecond; /* 0--99 */
};

#ifdef __cplusplus
extern "C" {
#endif
  /* find functions */
  unsigned __cdecl _dosix__dos_findfirst (const char *, unsigned, struct _find_t *);
  unsigned __cdecl _dosix__dos_findnext (struct _find_t *);
  void __cdecl _dosix__dos_findclose (struct _find_t *);
  int __cdecl _dosix_dosexterr (struct DOSERROR *);
  int __cdecl _dosix__dosexterr (struct _DOSERROR *);
  int __cdecl _dosix__int86x (int, union _REGS *, union _REGS *, struct _SREGS *);
  int __cdecl _dosix__int86 (int, union _REGS *, union _REGS *);
  int __cdecl _dosix__intdosx (union _REGS *, union _REGS *, struct _SREGS *);
  int __cdecl _dosix__intdos (union _REGS *, union _REGS *);
  void __cdecl _dosix_intr (int, union REGPACK *);
  int __cdecl _dosix__bdos (int, unsigned int, unsigned int);
  unsigned __cdecl _dosix__dos_getfileattr (const char *, unsigned *);
  unsigned __cdecl _dosix__dos_setfileattr (const char *, unsigned );
  unsigned __cdecl _dosix__dos_open (const char *, unsigned, int *);
  unsigned __cdecl _dosix__dos_creat (const char *, unsigned, int *);
  unsigned __cdecl _dosix__dos_creatnew (const char *, unsigned, int *);
  unsigned __cdecl _dosix__dos_close (int);
  unsigned __cdecl _dosix__dos_getftime (int, unsigned *, unsigned *);
  unsigned __cdecl _dosix__dos_setftime (int, unsigned, unsigned);
  unsigned __cdecl _dosix__dos_allocmem (size_t, uintptr_t *);
  unsigned __cdecl _dosix__dos_setblock (size_t, uintptr_t, size_t *);
  unsigned __cdecl _dosix__dos_freemem (uintptr_t);
  void __cdecl _dosix__dos_getdate (struct _dosdate_t *);
  void __cdecl _dosix__dos_gettime (struct _dostime_t *);
  unsigned __cdecl _dosix__dos_settime (struct _dostime_t *);
  unsigned __cdecl _dosix__dos_setdate (struct _dosdate_t *);
  void __cdecl _dosix__dos_setvect (unsigned, syscall_t);
  syscall_t __cdecl _dosix__dos_getvect (unsigned);
#ifdef __cplusplus
}
#endif

#endif	/* ! _INC_DOS */
