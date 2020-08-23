/*
  cpu.h -- 8086 CPU registers

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

#ifndef _INC_CPU
#define _INC_CPU

#include <stdint.h>

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

#endif
