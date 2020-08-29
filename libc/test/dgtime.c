/* DGTIME.C: This program gets and displays current date and time
 * values.
 */

#include <dosix/stdio.h>
#include <dos.h>

void main( void )
{
   struct _dosdate_t date;
   struct _dostime_t time;

   /* Get current date and time values */

   _dos_getdate( &date );
   _dos_gettime( &time );

   printf( "Today's date is %d-%d-%d\n", date.month,
                                         date.day, date.year );
   printf( "The time is %02d:%02d\n", time.hour, time.minute );

}
