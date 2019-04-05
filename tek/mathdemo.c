/*  tek.c 2.11 BSD tektroxix graphics test	*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tekio.h"

double pi = 3.14159265;

int main(argc, argv)
int argc;
char *argv[];
{
    	double step;
	double mx = MAXX/2;
	double my = MAXY/2;
	double r = MAXY / 2.2;
	double deg, x1, y1, x2, y2;
    	printf("This program requires a Tektronix terminal!\n");
    	clearScreen();
    	step = pi / 100.0;
    
	x1 = 0; y1 = 0;
	for (deg = step; deg < 2*pi+step; deg += step) {
		x2 = r * sin(2*deg);
		y2 = r * sin(3*deg);
		drawVector((int)(x1+mx), (int)(y1+my), (int)(x2+mx), (int)(y2+my));
		x1 = x2;
		y1 = y2;
	}

    	moveAlpha(35,0);
    	exit(0);  
}
