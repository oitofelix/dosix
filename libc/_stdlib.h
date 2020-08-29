/*
  _stdlib.h -- Commonly used library features

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

#ifndef _INC__STDLIB
#define _INC__STDLIB

#include <dosix/stdlib.h>

#define EXIT_CLEANUP_STACK_SIZE 32

typedef struct exit_cleanup_entry
{
  union
  {
    void (__cdecl *_atexit) (void);
    void (__cdecl __far *_fatexit) (void);
    _onexit_t _onexit;
    _fonexit_t _fonexit;
  } func;
  enum
    {
     _ATEXIT_FUNC,
     _FATEXIT_FUNC,
     _ONEXIT_FUNC,
     _FONEXIT_FUNC
    } type;
} exit_cleanup_entry_t;

extern
exit_cleanup_entry_t
_dosix__exit_cleanup_stack[EXIT_CLEANUP_STACK_SIZE];

extern
size_t
_dosix__exit_cleanup_stack_top;

#endif
