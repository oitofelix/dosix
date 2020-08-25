/*
  INT.h -- DOS interrupt services enumeration

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

#ifndef _INC_INT
#define _INC_INT

/* Interrupts */

#define INT21_MAIN_DOS_API 0x21
#define INT2F_MULTIPLEX 0x2f

/* Sub-functions */

#define INT21_AH_GETCHE 0x01
#define INT21_AH_PUTCH 0x02
#define INT21_AH_GETCH 0x08
#define INT21_AH_WRITE_STDOUT 0x09
#define INT21_AH_SET_DTA_ADDR 0x1a
#define INT21_AH_SETVECT 0x25
#define INT21_AH_GETDATE 0x2a
#define INT21_AH_SETDATE 0x2b
#define INT21_AH_GETTIME 0x2c
#define INT21_AH_SETTIME 0x2d
#define INT21_AH_GET_DTA_ADDR 0x2f
#define INT21_AH_GETVECT 0x35
#define INT21_AH_OPEN 0x3d
#define INT21_AH_CLOSE 0x3e
#define INT21_AH_ALLOCMEM 0x48
#define INT21_AH_FREEMEM 0x49
#define INT21_AH_SETBLOCK 0x4a
#define INT21_AH_FINDFIRST 0x4e
#define INT21_AH_FINDNEXT 0x4f
#define INT21_AH_EXTERR 0x59
#define INT21_BH_EXTERR 0x00
#define INT21_BL_EXTERR 0x00
#define INT21_AH_FILE_METADATA 0x43
#define INT21_AL_FILE_METADATA_GETFILEATTR 0x00
#define INT21_AL_FILE_METADATA_SETFILEATTR 0x01
#define INT21_AH_FILE_TIME 0x57
#define INT21_AL_FILE_TIME_GETFTIME 0x00
#define INT21_AL_FILE_TIME_SETFTIME 0x01
#define INT2F_AH_DOS_INTERNAL 0x12
#define INT2F_AL_DOS_INTERNAL_EXTERR_SET 0x22

#endif
