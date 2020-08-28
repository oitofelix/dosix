/* INTDOS.C: This program uses _intdos to invoke DOS system call 2AH
 * (gets the current date).
 */

#include <dos.h>
#include <dos/stdio.h>

void main( void )
{
   union _REGS inregs, outregs;

   inregs.h.ah = 0x2a;           /* DOS Get Date function: */
   _intdos( &inregs, &outregs );
   printf( "Date: %d/%d/%d\n", outregs.h.dh,
                               outregs.h.dl, outregs.x.cx );
}
