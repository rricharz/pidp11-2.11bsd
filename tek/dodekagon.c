/*  dodekagon.c					*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tekio.h"

int main (argc, argv)
int argc; char *argv;
{
        int angle, angle1;
        double rad = 3.14159265 / 180;
        int xcenter = MAXX / 2;
        int ycenter = MAXY / 2;
        int radius = 9 * MAXY / 20;
 
        clearScreen();
        for (angle = 0; angle < 360; angle += 30) {
                for (angle1 = angle + 30; angle1 < 360; angle1 += 30) {
                        drawVector((int)(xcenter + radius * cos(rad * angle)),
                                        (int)(ycenter + radius * sin (rad * angle)),
                        		(int)(xcenter + radius * cos (rad * angle1)),
                                        (int)(ycenter + radius * sin (rad * angle1)));
                }
        }
	moveTo(0,0);
}
