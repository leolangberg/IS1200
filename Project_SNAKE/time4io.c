#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */


int getsw( void )
{
   return (PORTD >> 8) & 0x0f;
}
int getbtns(void)
{
	return (PORTD >> 5) & 0x07;   	
}
int btn1 ( void ){
    return (PORTF & 0x2);
}



