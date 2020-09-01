/*
  stdio.h -- Standard input/output

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

#ifndef _INC_DOSIX_STDIO
#define _INC_DOSIX_STDIO

#include <stdio.h>
#include <dosix/compiler.h>

#ifndef _DOSIX_LIBC_SRC
/* getc functions */
#  define fgetc _dosix_fgetc
#  define _fgetchar _dosix__fgetchar
#  define getc _dosix_getc
#  define getchar _dosix_getchar
#  define ungetc _dosix_ungetc
/* putc functions */
#  define fputc _dosix_fputc
#  define _fputchar _dosix__fputchar
#  define putc _dosix_putc
#  define putchar _dosix_putchar
/* vprintf functions */
#  define vfprintf _dosix_vfprintf
#  define vprintf _dosix_vprintf
#  define vsprintf _dosix_vsprintf
#  define _vsnprintf _dosix__vsnprintf
/* printf functions */
#  define fprintf _dosix_fprintf
#  define printf _dosix_printf
#  define sprintf _dosix_sprintf
#  define snprintf _dosix_snprintf
/* scanf functions */
#  define fscanf _dosix_fscanf
#  define scanf _dosix_scanf
#  define sscanf _dosix_sscanf
/* perror */
#  define perror _dosix_perror
/* _fileno */
#  define _fileno _dosix__fileno

#  ifndef __STRICT_ANSI__
#    define fcloseall _fcloseall
#    define fdopen _fdopen
#    define fgetchar _fgetchar
#    define fileno _fileno
#    define flushall _flushall
#    define fputchar _fputchar
#    define getw _getw
#    define putw _putw
#    define rmtmp _rmtmp
#    define tempnam _tempnam
#    define unlink _unlink
#    define stdaux _stdaux
#    define stdprn _stdprn
#    define P_tmpdir _P_tmpdir
#    define SYS_OPEN _SYS_OPEN
#  endif /* ! __STRICT_ANSI__ */

#endif /* ! _DOSIX_LIBC_SRC */

#ifdef __cplusplus
extern "C" {
#endif
  /* getc functions */
  int __cdecl _dosix_fgetc (FILE *);
  int __cdecl _dosix__fgetchar (void);
  inline int __cdecl _dosix_getc (FILE *);
  inline int __cdecl _dosix_getchar (void);
  int __cdecl _dosix_ungetc (int, FILE *);
  /* putc functions */
  int __cdecl _dosix_fputc (int, FILE *);
  int __cdecl _dosix_fputchar (int);
  inline int __cdecl _dosix_putc (int, FILE *);
  inline int __cdecl _dosix_putchar (int);
  /* vprintf functions */
  int __cdecl _dosix_vfprintf (FILE *, const char *, va_list);
  int __cdecl _dosix_vprintf (const char *, va_list);
  int __cdecl _dosix_vsprintf (char *, const char *, va_list);
  int __cdecl _dosix__vsnprintf (char *, size_t, const char *, va_list);
  /* printf functions */
  int __cdecl _dosix_fprintf (FILE *, const char *, ...);
  int __cdecl _dosix_printf (const char *, ...);
  int __cdecl _dosix_sprintf (char *, const char *, ...);
  int __cdecl _dosix__snprintf (char *, size_t, const char *, ...);
  /* scanf functions */
  int __cdecl _dosix_fscanf (FILE *, const char *, ...);
  int __cdecl _dosix_scanf (const char *, ...);
  int __cdecl _dosix_sscanf (const char *, const char *, ...);
  /* fileno */
  inline int __cdecl _dosix__fileno (FILE *);
  /* perror */
  void __cdecl _dosix_perror (const char *);
#ifdef __cplusplus
}
#endif

#endif	/* ! _INC_DOSIX_STDIO */
