/* MALLOC.C: This program allocates memory with malloc, then frees
 * the memory with free.
 */

#include <dos/stdlib.h>         /* Definition of _MAX_PATH */
#include <dos/stdio.h>
#include <dos/malloc.h>

void main( void )
{
   char *string;

   /* Allocate space for a path */
   string = malloc( _MAX_PATH );
   if( string == NULL )
      printf( "Insufficient memory available\n" );
   else
      printf( "Memory space allocated for path\n" );
   free( string );
   printf( "Memory freed\n" );

}
