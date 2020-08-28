/*
  stdlib.c -- Commonly used library features

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
#include <stdlib.h>
#include <stdbool.h>
#include <dos/stdlib.h>
#include "_stdlib.h"


/* exported variables */

exit_cleanup_entry_t
_dosix__exit_cleanup_stack[EXIT_CLEANUP_STACK_SIZE];

size_t
_dosix__exit_cleanup_stack_top = 0;


/* auxiliary functions */

static int
register_cleanup
(const exit_cleanup_entry_t *entry)
{
  assert (entry);
  assert (_dosix__exit_cleanup_stack_top <= EXIT_CLEANUP_STACK_SIZE);
  if (_dosix__exit_cleanup_stack_top == EXIT_CLEANUP_STACK_SIZE)
    return 1;
  int ret = 0;
  switch (entry->type)
    {
    case _ATEXIT_FUNC:
      ret = atexit ((void (*) (void)) entry->func._atexit);
      break;
    case _FATEXIT_FUNC:
      ret = atexit ((void (*) (void)) entry->func._fatexit);
      break;
    case _ONEXIT_FUNC:
      ret = atexit ((void (*) (void)) entry->func._onexit);
      break;
    case _FONEXIT_FUNC:
      ret = atexit ((void (*) (void)) entry->func._fonexit);
      break;
    default:
      assert (false);		/* should never get here! */
    }
  if (ret)
    return ret;
  _dosix__exit_cleanup_stack[_dosix__exit_cleanup_stack_top++] = *entry;
  return 0;
}


/* atexit functions */

int
_dosix_atexit
(void (__cdecl *func) (void))
{
  exit_cleanup_entry_t ce =
    {
     .func._atexit = func,
     .type = _ATEXIT_FUNC
    };
  return register_cleanup (&ce);
}

int
__far
_fatexit
(void (__cdecl __far *func) (void))
{
  exit_cleanup_entry_t ce =
    {
     .func._fatexit = func,
     .type = _FATEXIT_FUNC
    };
  return register_cleanup (&ce);
}


/* _onexit functions */

_onexit_t
_onexit
(_onexit_t func)
{
  exit_cleanup_entry_t ce =
    {
     .func._onexit = func,
     .type = _ONEXIT_FUNC
    };
  return ! register_cleanup (&ce)
    ? func
    : NULL;
}

_fonexit_t
__far
_fonexit
(_fonexit_t func)
{
  exit_cleanup_entry_t ce =
    {
     .func._fonexit = func,
     .type = _FONEXIT_FUNC
    };
  return ! register_cleanup (&ce)
    ? func
    : NULL;
}
