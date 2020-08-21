/* STRLWR.C: This program uses _strlwr and _strupr to create
 * uppercase and lowercase copies of a mixed-case string.
 */

#include <_string.h>
#include <stdio.h>
#include <dos.h>

void main( void )
{
   char string[100] = "The String to End All Strings!";
   char *copy1, *copy2;

   copy1 = _strlwr( _strdup( string ) );
   copy2 = _strupr( _strdup( string ) );
   printf( "Mixed: %s\n", string );
   printf( "Lower: %s\n", copy1 );
   printf( "Upper: %s\n", copy2 );
}
