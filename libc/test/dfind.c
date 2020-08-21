/* DFIND.C: This program finds and prints all files in the current
 * directory with the .c extension.
 */

#include <stdio.h>
#include <dos.h>

void main( void )
{
   struct _find_t  c_file;

   /* Find first .c file in current directory. */
   _dos_findfirst( "*.c", _A_NORMAL, &c_file );

   printf( "Listing of .c files\n\n" );
   printf( "File: %s is %ld bytes\n", c_file.name, c_file.size );

   /* Find the rest of the .c files. */
   while( _dos_findnext( &c_file ) == 0 )

      printf( "File: %s is %ld bytes\n", c_file.name, c_file.size );
}
