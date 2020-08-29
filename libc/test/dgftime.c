/* DGFTIME.C: This program displays and modifies the date and time
 * fields of a file.
 */

#include <dosix/fcntl.h>
#include <dosix/stdio.h>
#include <dosix/stdlib.h>
#include <dos.h>

void main( void )
{
                                 /* FEDC BA98 7654 3210           */
   unsigned new_date = 0x278f;   /* 0010 0111 1000 1111  12/15/99 */
   unsigned new_time = 0x48e0;   /* 0100 1000 1110 0000  9:07 AM  */
   unsigned old_date, old_time;

   int fh;

   /* Open file with _dos_open function */

   if( _dos_open( "dgftime", _O_RDONLY, &fh ) != 0 )
      exit( 1 );

   /* Get file date and time */
   _dos_getftime( fh, &old_date, &old_time );
   printf( "Old date field: 0x%.4x\n", old_date );
   printf( "Old time field: 0x%.4x\n", old_time );
   system( "stat dgftime" );

   /* Modify file date and time */
   if( !_dos_setftime( fh, new_date, new_time ) )
   {
      _dos_getftime( fh, &new_date, &new_time );
      printf( "New date field: 0x%.4x\n", new_date );

      printf( "New time field: 0x%.4x\n", new_time );
      system( "stat dgftime" );

      /* Restore date and time */
      _dos_setftime( fh, old_date, old_time );
   }
   _dos_close( fh );
}
