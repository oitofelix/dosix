/* REALLOC.C: This program allocates a block of memory for buffer
 * and then uses _msize to display the size of that block. Next, it
 * uses realloc to expand the amount of memory used by buffer
 * and then calls _msize again to display the new amount of
 * memory allocated to buffer.
 */

#include <dosix_stdio.h>
#include <dosix_malloc.h>
#include <dosix_stdlib.h>
void main( void )
{
   long *buffer;
   size_t size;

   if( (buffer = (long *)malloc( 1000 * sizeof( long ) )) == NULL )

      exit( 1 );

   size = _msize( buffer );
   printf( "Size of block after malloc of 1000 longs: %u\n", size );

   /* Reallocate and show new size: */
   if( (buffer = realloc( buffer, size + (1000 * sizeof( long )) ))
        == NULL )
      exit( 1 );
   size = _msize( buffer );
   printf( "Size of block after realloc of 1000 more longs: %u\n",
            size );

   free( buffer );
   exit( 0 );
}
