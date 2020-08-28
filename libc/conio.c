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


/* headers */
#include <assert.h>
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <conio.h>
#include <termios.h>
#include <unistd.h>


/* _cputs */
int
_cputs
(const char *string)
{
  assert (string);
  for (size_t i = 0; string[i]; i++)
    if (_putch (string[i]) == EOF)
      return -1; /* return non-zero value on error */
  return 0;
}


/* _getch functions */
static
int
__getch
(int echo)
{
  struct termios termios_prev, termios_curr;
  int ttyp = isatty (STDIN_FILENO);
  /* disable terminal echo */
  if (ttyp)
    {
      if (tcgetattr (STDIN_FILENO, &termios_prev) < 0)
	assert (true);	/* ignore error */
      termios_curr = termios_prev;
      termios_curr.c_lflag &= ~ICANON;
      termios_curr.c_lflag = echo
	? termios_curr.c_lflag | ECHO
	: termios_curr.c_lflag & ~ECHO;
      if (tcsetattr (STDIN_FILENO, TCSANOW, &termios_curr) < 0)
	assert (true);	/* ignore error */
    }
  char c;
  if (read (STDIN_FILENO, &c, sizeof (c)) <= 0)
    assert (true); /* ignore error */
  /* restore terminal settings */
  if (ttyp && tcsetattr (STDIN_FILENO, TCSANOW, &termios_prev) < 0)
    assert (true); /* ignore error */
  return c;
}

int
_getch
(void)
{
  return __getch (0);
}

int
_getche
(void)
{
  return __getch (1);
}


/* _putch */

int
_putch
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
