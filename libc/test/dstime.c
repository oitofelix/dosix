/* DSTIME.C: This program changes the time and date values and displays
 * the new date and time values.
 */

#include <dos.h>
#include <conio.h>
#include <dosix/stdio.h>
#include <dosix/time.h>

void main( void )
{
   struct _dosdate_t olddate, newdate = { 4, 7, 1999 };
   struct _dostime_t oldtime, newtime = { 3, 45, 30,
                                          0 };
   char   datebuf[40], timebuf[40];

   /* Get current date and time values */

   _dos_getdate( &olddate );
   _dos_gettime( &oldtime );
   printf( "%s    %s\n" , _strdate( datebuf ), _strtime( timebuf ) );

   /* Modify date and time structures */
   _dos_setdate( &newdate );
   _dos_settime( &newtime );
   printf( "%s    %s\n" , _strdate( datebuf ), _strtime( timebuf ) );

   /* Restore old date and time */
   _dos_setdate( &olddate );
   _dos_settime( &oldtime );
}
