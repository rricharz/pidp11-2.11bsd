/*  tektest.c 2.11 BSD tektroxix graphics test	*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/
/*  test some tek4010 features                  */

#include <stdio.h>
#include <stdlib.h>
#include "tekio.h"

int main(argc, argv)
int argc;
char *argv[];
{
    int i,p;
    printf("This program requires a Tektronix 4014 terminal!\n");
    clearScreen();

    for (p = 20; p <= 600; p += 20 )  {
        startWriteThrough(); /* each time in loop to synchrosize screen refresh */
        startDraw(p, p);
        draw(p + 50, p);
        draw(p + 50, p + 50);
        draw(p, p + 50);
        draw(p, p);
        endDraw();
    }

    endWriteThrough();

    moveAlpha(1,1);
    exit(0);  
}
