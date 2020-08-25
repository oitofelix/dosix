/* CPUTS.C: This program first displays a string to the console. */

#include <conio.h>

void main( void )
{
   /* String to print at console. Note the \r (return) character. */
   char *buffer = "Hello world (courtesy of _cputs)!\r\n";

   _cputs( buffer );
}
