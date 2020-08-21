/* INTDOSX.C Sends a $-terminated string to the standard output
 * device.
 */

#include <dos.h>
#include <stdio.h>

char far *buffer = "Dollar-sign terminated string\n\r\n\r$";

void main( void )
{
   union  _REGS inregs, outregs;
   struct _SREGS segregs;

   /* Print a $-terminated string on the screen using DOS function
    * 0x09.
    */
   inregs.h.ah = 0x9;
   inregs.x.dx = _FP_OFF( buffer );
   segregs.ds  = _FP_SEG( buffer );

   _intdosx( &inregs, &outregs, &segregs );
}
