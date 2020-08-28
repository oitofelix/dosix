/* STRTIME.C */

#include <dos/time.h>
#include <dos/stdio.h>

void main( void )
{
   char dbuffer [9];
   char tbuffer [9];

   _strdate( dbuffer );
   printf( "The current date is %s \n", dbuffer );
   _strtime( tbuffer );
   printf( "The current time is %s \n", tbuffer );
}
