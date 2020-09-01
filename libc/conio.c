/*
  conio.c -- Console and port I/O

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
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>


/* DOSix headers */
#include <dosix/assert.h>
#include <dosix/stdarg.h>
#include <io.h>
#include <conio.h>


/* private global variables */

static bool ungetch_full = false;
static char ungetch_char = '\0';


/* _cputs */
int
_dosix__cputs
(const char *string)
{
  _dosix_assert (string);
  for (size_t i = 0; string[i]; i++)
    if (_dosix__putch (string[i]) == EOF)
      return -1; /* return non-zero value on error */
  return 0;
}


/* _getch functions */
static
int
getch
(int echo)
{
  if (ungetch_full)
    {
      ungetch_full = false;
      return ungetch_char;
    }
  struct termios termios_prev, termios_curr;
  int ttyp = _dosix__isatty (STDIN_FILENO);
  /* setup terminal input mode */
  if (ttyp)
    {
      if (tcgetattr (STDIN_FILENO, &termios_prev) < 0)
	_dosix_assert (true); /* ignore error */
      termios_curr = termios_prev;
      termios_curr.c_lflag &= ~ICANON;
      termios_curr.c_lflag = echo
	? termios_curr.c_lflag | ECHO
	: termios_curr.c_lflag & ~ECHO;
      if (tcsetattr (STDIN_FILENO, TCSANOW, &termios_curr) < 0)
	_dosix_assert (true); /* ignore error */
    }
  char c;
  int ret = read (STDIN_FILENO, &c, sizeof (c)) <= 0
    ? EOF			/* not required by documentation, but
				   might be useful while compatible */
    : c;
  /* restore terminal settings */
  if (ttyp && tcsetattr (STDIN_FILENO, TCSANOW, &termios_prev) < 0)
    _dosix_assert (true); /* ignore error */
  return ret;
}

int
_dosix__getch
(void)
{
  return getch (0);
}

int
_dosix__getche
(void)
{
  return getch (1);
}


/* _ungetch */

int
_ungetch
(int c)
{
  if (ungetch_full)
    return EOF;
  ungetch_full = true;
  ungetch_char = c;
  return c;
}



/* _putch */

int
_dosix__putch
(int c)
{
  char ch = c;
  ssize_t count = 0;
  while (count >= 0 && count < sizeof (ch))
    count = TEMP_FAILURE_RETRY (write (STDOUT_FILENO,
				       &ch,
				       sizeof (ch)));
  return count < 0
    ? EOF
    : c;
}


/* _cprintf */

int
_dosix__cprintf
(const char *format,
 ...)
{
  char *string = NULL;
  va_list ap;
  _dosix_va_start (ap, format);
  if (vasprintf (&string,
		 format,
		 ap) < 0)
    {
      free (string); /* harmless; just to make sure */
      _dosix_va_end (ap);
      return 0;
    }
  _dosix_assert (string);
  size_t count;
  for (count = 0; string[count]; count++)
    if (_dosix__putch (string[count]) == EOF)
      break;
  free (string);
  _dosix_va_end (ap);
  return count;
}


/* _cscanf */

int
_dosix_cscanf
(const char *format,
 ...)
{
  /* Couldn’t find a way to implement this in terms of _getche and the
     underlying C library’s scanf facilities.  A re-implementation
     might not be worth it. */
  va_list ap;
  _dosix_va_start (ap,
		   format);
  /* _ungetch hack */
  if (getch_full && _dosix_ungetc (ungetch_char, stdin) != EOF)
    ungetch_full = false;
  int count = vscanf (format,
		      ap);
  _dosix_va_end (ap);
  return count;
}
