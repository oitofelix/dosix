/*
  timeconv.c -- Conversion between DOS and Unix time formats

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
#include <errno.h>
#include <time.h>
#include "_dosexterr.h"

unsigned
__unixtime_struct
(const struct _dosdate_t *date,
 const struct _dostime_t *time,
 struct timeval *tv)
{
  assert (date), assert (time), assert (tv);
  struct _DOSERROR errorinfo = {0};
  struct tm tm =
    {
     .tm_sec = time->second,
     .tm_min = time->minute,
     .tm_hour = time->hour,
     .tm_mday = date->day,
     .tm_mon = date->month - 1,
     .tm_year = date->year - 1900,
     .tm_wday = date->dayofweek,
    };
  time_t utime = mktime (&tm);
  if (utime == (time_t) -1)
    {
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      return __doskexterr_set (&errorinfo, EINVAL);
    }
  tv->tv_sec = utime;
  tv->tv_usec = time->hsecond * 10000;
  return 0;
}

unsigned
__dostime_struct
(const struct timeval *tv,
 struct _dosdate_t *date,
 struct _dostime_t *time)
{
  assert (tv);
  struct _DOSERROR errorinfo = {0};
  struct tm *tm = localtime (&tv->tv_sec);
  if (! tm)
    {
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      return __doskexterr_set (&errorinfo, 0);
    }
  if (date)
    *date = (struct _dosdate_t)
      {
       .day = tm->tm_mday,
       .month = tm->tm_mon + 1,
       .year = 1900 + tm->tm_year,
       .dayofweek = tm->tm_wday
      };
  if (time)
    *time = (struct _dostime_t)
      {
       .hour = tm->tm_hour,
       .minute = tm->tm_min,
       .second = tm->tm_sec,
       .hsecond = tv->tv_usec / 10000
      };
  return 0;
}

/* Convert DOS date-time encoded integers to Unix time */
unsigned
__unixtime_int
(unsigned date,
 unsigned time,
 time_t *utime)
{
  struct tm tm =
    {
     .tm_sec = (time & ((1 << 5) - 1)) * 2,
     .tm_min = time >> 5 & ((1 << 6) - 1),
     .tm_hour = time >> 11,
     .tm_mday = date & ((1 << 5) - 1),
     .tm_mon = (date >> 5 & ((1 << 4) - 1)) - 1,
     .tm_year = (date >> 9) + (1980 - 1900),
     .tm_isdst = -1
    };
  time_t _utime = mktime (&tm);
  struct _DOSERROR errorinfo = {0};
  if (_utime == (time_t) (-1))
    {
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      return __doskexterr_set (&errorinfo, 0);
    }
  if (utime)
    *utime = _utime;
  return 0;
}

/* Convert Unix time to DOS date-time encoded integers */
unsigned
__dostime_int
(const time_t *utime,
 unsigned *date,
 unsigned *time)
{
  assert (utime);
  struct _DOSERROR errorinfo = {0};
  struct tm *tm = localtime (utime);
  if (! tm)
    {
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      return __doskexterr_set (&errorinfo, 0);
    }
  if (date)
    *date = tm->tm_year - (1980 - 1900) << 9
      | tm->tm_mon + 1 << 5
      | tm->tm_mday;
  if (time)
    *time = tm->tm_hour << 11
      | tm->tm_min << 5
      | tm->tm_sec / 2;
  return 0;
}
