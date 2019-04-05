/*  tek.c 2.11 BSD tektroxix graphics test	*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

#include <stdio.h>
#include <stdlib.h>
#include "tekio.h"

int main(argc, argv)
int argc;
char *argv[];
{
    int i,s,step;
    printf("This program requires a Tektronix terminal!\n");
    clearScreen();
    for (i=0; i<=MAXY; i+=10)
      drawVector(0,i,MAXX,MAXY-i);
    step=28;
    for (i=0; i<=MAXY/2; i+=step) {
      if (step--<1) step=1;
      s = (int)((double)i*(double)MAXX/(double)MAXY);
      drawVector(s,i,MAXX-s,i);
      drawVector(s,MAXY-i,MAXX-s,MAXY-i);
    }
    moveAlpha(35,0);
    exit(0);  
}
