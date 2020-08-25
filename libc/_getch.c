/*
  _getch.c -- Read a single character from console

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
#include <termios.h>
#include <unistd.h>
#include "_dosexterr.h"
#include "INT.h"

static
int
__getch
(int echo)
{
  struct _DOSERROR errorinfo = {0};
  struct termios termios_prev, termios_curr;
  int ttyp = isatty (STDIN_FILENO);
  /* disable terminal echo */
  if (ttyp)
    {
      if (tcgetattr (STDIN_FILENO, &termios_prev) < 0)
	_dosexterr (&errorinfo);	/* TODO? better error handling */
      termios_curr = termios_prev;
      termios_curr.c_lflag &= ~ICANON;
      termios_curr.c_lflag = echo
	? termios_curr.c_lflag | ECHO
	: termios_curr.c_lflag & ~ECHO;
      if (tcsetattr (STDIN_FILENO, TCSANOW, &termios_curr) < 0)
	_dosexterr (&errorinfo);	/* TODO? better error handling */
    }
  char c;
  if (read (STDIN_FILENO, &c, sizeof (c)) <= 0)
    _dosexterr (&errorinfo);	/* TODO? better error handling */
  /* restore terminal settings */
  if (ttyp && tcsetattr (STDIN_FILENO, TCSANOW, &termios_prev) < 0)
    _dosexterr (&errorinfo);	/* TODO? better error handling */
  return c;
}

int
_getch
(void)
{
  return __getch (0);
}

void
_dosk86_getch
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GETCH);
  cpu->l.al = _getch ();
}

int
_getche
(void)
{
  return __getch (1);
}

void
_dosk86_getche
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GETCHE);
  cpu->l.al = _getche ();
}
