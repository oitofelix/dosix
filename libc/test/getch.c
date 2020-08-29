/* GETCH.C: This program reads characters from the keyboard until it
 * receives a 'Y' or 'y'.
 */

#include <conio.h>
#include <dosix/ctype.h>

void main( void )
{
   int ch;

   _cputs( "Type 'Y' when finished typing keys: " );
   do
   {
      ch = _getch();
      ch = toupper( ch );
   } while( ch != 'Y' );

   _putch( ch );
   _putch( '\r' );    /* Carriage return */
   _putch( '\n' );    /* Linefeed       */
}
