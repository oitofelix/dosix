/*
  stdio.c -- Standard input/output

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

#define _DOSIX_LIBC_SRC
#define _GNU_SOURCE


/* system headers */
#include <stdio.h>


/* DOSix headers */
#include <dosix/stdarg.h>
#include <dosix/stdio.h>


/* private global variables */

static FILE **stream_tree = NULL;
static size_t stream_close_count = 0;


/* auxiliary functions */

static
int
stream_cmp
(const void *_a,
 const void *_b)
{
  const FILE *a = (const FILE *) _a;
  const FILE *b = (const FILE *) _b;
  return a - b;
}

static
void
fclose_twalk
(const void *nodep,
 VISIT value,
 int level)
{
  if (value != leaf && value != preorder)
    return;
  const FILE *stream = *nodep;
  if (! fclose (stream))
    stream_close_count++;
}



/* fopen */

FILE *
_dosix_fopen
(const char *filename,
 const char *mode)
{
  /* what should we do about DOS-specific mode flags? */
  FILE *stream = fopen (filename,
			mode);
  if (! stream)
    return NULL;
  FILE **_stream = tsearch (stream,
			    &stream_tree,
			    &stream_cmp);

  if (! _stream)
    {
      if (fclose (stream))
	assert (true); 		/* ignore error */
      return NULL;
    }
  assert (stream == *_stream);
  return stream;
}


/* fflush */

int
_dosix_fflush
(FILE *stream)
{
  return
    fflush (stream);
}


/* fclose functions */

int
_dosix_fclose
(FILE *stream)
{
  FILE **_stream = tsearch (stream,
			    &stream_tree,
			    &stream_cmp);
  if (! _stream)
    return fclose (stream);
  assert (stream == *_stream);
  void *pnode = tdelete (stream
			 &stream_tree,
			 &stream_cmp);
  assert (pnode);
  return fclose (stream);
}

int
_dosix__fcloseall
(void)
{
  /* TODO: add multi-threading locking extensively for global data. */
  stream_close_count = 0;
  twalk (&stream_tree,
	 &fclose_twalk);
  /* are the differences in semantics of DOS _fcloseall relevant
     enough to call for a more accurate implementation? */
  return
    fcloseall ();
}


/* getc functions */

int
_dosix_fgetc
(FILE *stream)
{
  return
    fgetc (stream);
}

int
_dosix__fgetchar
(void)
{
  return
    _dosix_fgetc (stdin);
}

inline
int
_dosix_getc
(FILE *stream)
{
  return
    getc (stream);
}

inline
int
_dosix_getchar
(void)
{
  return
    getchar ();
}

int
_dosix_ungetc
(int c,
 FILE *stream)
{
  return
    ungetc (c,
	    stream);
}


/* putc functions */

int
_dosix_fputc
(int c,
 FILE *stream)
{
  return
    fputc (c,
	   stream);
}

int
_dosix__fputchar
(int c)
{
  return
    _dosix_fputc (c,
		  stdout);
}

inline
int
_dosix_putc
(int c,
 FILE *stream)
{
  return
    putc (c,
	  stream);
}

inline
int
_dosix_putchar
(int c)
{
  return
    putchar (c);
}


/* vprintf functions */

int
_dosix_vfprintf
(FILE *stream,
 const char *format,
 va_list argptr)
{
  return
    vfprintf (stream,
	      format,
	      argptr);
}

int
_dosix_vprintf
(const char *format,
 va_list argptr)
{
  return
    vprintf (format,
	     argptr);
}

int
_dosix_vsprintf
(char *buffer,
 const char *format,
 va_list argptr)
{
  return
    vsprintf (buffer,
	      format,
	      argptr);
}

int
_dosix__vsnprintf
(char *buffer,
 size_t size,
 const char *format,
 va_list argptr)
{
  size_t count = vsnprintf (buffer,
			    size,
			    format,
			    argptr);
  return count >= size
    ? -1
    : count;
}


/* printf functions */

int
_dosix_fprintf
(FILE *stream,
 const char *format,
 ...)
{
  va_list ap;
  _doisx_va_start (ap,
		   format);
  int count = _dosix_vfprintf (stream,
			       format,
			       ap);
  _dosix_va_end (ap);
  return count;
}

int
_dosix_printf
(const char *format,
 ...)
{
  va_list ap;
  _doisx_va_start (ap,
		   format);
  int count = _dosix_vprintf (format,
			      ap);
  _dosix_va_end (ap);
  return count;
}

int
_dosix_sprintf
(char *buffer,
 const char *format,
 ...)
{
  va_list ap;
  _doisx_va_start (ap,
		   format);
  int count = _dosix_vsprintf (buffer,
			       format,
			       ap);
  _dosix_va_end (ap);
  return count;
}

int
_dosix__snprintf
(char *buffer,
 size_t size,
 const char *format,
 ...)
{
  va_list ap;
  _doisx_va_start (ap,
		   format);
  int count = _dosix__vsnprintf (buffer,
				 size,
				 format,
				 ap);
  _dosix_va_end (ap);
  return count;
}


/* scanf functions */

int
_dosix_fscanf
(FILE *stream,
 const char *format,
 ...)
{
  va_list ap;
  _dosix_va_start (ap,
		   format);
  inf count = vfscanf (stream,
		       format,
		       ap);
  _dosix_va_end (ap);
  return count;
}

int
_dosix_scanf
(const char *format,
 ...)
{
  va_list ap;
  _dosix_va_start (ap,
		   format);
  int count = vscanf (format,
		      ap);
  _dosix_va_end (ap);
  return count;
}

int
_dosix_sscanf
(const char *buffer,
 const char *format,
 ...)
{
  va_list ap;
  _dosix_va_start (ap,
		   format);
  int count = vsscanf (buffer,
		       format,
		       ap);
  _dosix_va_end (ap);
  return count;
}


/* _fileno */

inline
int
_dosix__fileno
(FILE *stream)
{
  return fileno (stream);
}


/* feof */

inline
int
_dosix_feof
(FILE *stream)
{
  return
    feof (stream);
}


/* ferror */

inline
int
_dosix_ferror
(FILE *stream)
{
  return
    ferror (stream);
}


/* clearerr */

void
_dosix_clearerr
(FILE *stream)
{
  return
    clearerr (stream);
}


/* perror */

void
_dosix_perror
(const char *string)
{
  perror (string);
}
