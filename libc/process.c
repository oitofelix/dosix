/*
  process.c -- Process control

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
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include "_stdlib.h"


/* exit functions */

void
_dosix_exit
(int status)
{
  exit (status);
}

void
_dosix__exit
(int status)
{
  _Exit (status);
}


/* _cexit functions */

void
_cexit
(void)
{
  while (_dosix__exit_cleanup_stack_top-- > 0)
    {
      struct exit_cleanup_entry *ce =
	&_dosix__exit_cleanup_stack[_dosix__exit_cleanup_stack_top];
      switch (ce->type)
	{
	case _ATEXIT_FUNC:
	  ce->func._atexit ();
	  break;
	case _FATEXIT_FUNC:
	  ce->func._fatexit ();
	  break;
	case _ONEXIT_FUNC:
	  ce->func._onexit ();
	  break;
	case _FONEXIT_FUNC:
	  ce->func._fonexit ();
	  break;
	default:
	  assert (false);	/* should never get here! */
	}
    }
  if (fflush (NULL))
    assert (true);		/* ignore errors */
  _c_exit ();
}

void
_c_exit
(void)
{
  if (fcloseall ())
    assert (true);		/* ignore errors */
}
