/*
  share.h -- File sharing modes

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

#ifndef _INC_SHARE
#define _INC_SHARE

#define _SH_COMPAT 0x00 /* compatibility mode */
#define _SH_CMPAT _SH_CMPAT /* documented alias (typo?) */
#define _SH_DENYRW 0x10 /* deny read/write mode */
#define _SH_DENYWR 0x20 /* deny write mode */
#define _SH_DENYRD 0x30 /* deny read mode */
#define _SH_DENYNO 0x40 /* deny none mode */

#define SH_COMPAT _SH_COMPAT
#define SH_CMPAT _SH_CMPAT
#define SH_DENYRW _SH_DENYRW
#define SH_DENYWR _SH_DENYWR
#define SH_DENYRD _SH_DENYRD
#define SH_DENYNO _SH_DENYNO

#endif
