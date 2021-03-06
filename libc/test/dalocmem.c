/* DALOCMEM.C: This program allocates 20 paragraphs of memory, increases
 * the allocation to 40 paragraphs, and then frees the memory space.
 */

#include <dos.h>
#include <dosix/stdio.h>

void main( void )
{
   uintptr_t segment;
   size_t maxsize;

   /* Allocate 20 paragraphs */
   if( _dos_allocmem( 20, &segment ) != 0 )
      printf( "allocation failed\n" );
   else
      printf( "allocation successful\n" );

   /* Increase allocation to 40 paragraphs */

   if( _dos_setblock( 40, segment, &maxsize ) != 0 )
      printf( "allocation increase failed\n" );
   else
      printf( "allocation increase successful\n" );

   /* free memory */
   if( _dos_freemem( segment ) != 0 )
      printf( "free memory failed\n" );
   else
      printf( "free memory successful\n" );
}
