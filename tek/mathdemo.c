/*  mathdemo.c 2.11 BSD tektroxix graphics test	*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tekio.h"

int main(argc, argv)
int argc;
char *argv[];
{
    	double step;
	double mx = MAXX/2;   /* center of screen */
	double my = MAXY/2;
	double r = MAXY / 2.2;
	double deg, x1, y1, x2, y2;
    	printf("This program requires a Tektronix terminal!\n");

/********************************************************************************/

    	clearScreen();
        drawJustifiedText("2.11 BSD on PiDP-11", 1, 0);
	drawJustifiedText("x = sin(2t), y = sin(3t)", 1, 2);
	drawJustifiedText("0 <= t <= 2*PI", 1, 3);

    	step = PI / 100.0;
    
	x1 = 0; y1 = 0;
	for (deg = step; deg < 2*PI+step; deg += step) {
		x2 = r * sin(2*deg);
		y2 = r * sin(3*deg);
		drawVector((int)(x1+mx), (int)(y1+my), (int)(x2+mx), (int)(y2+my));
		x1 = x2;
		y1 = y2;
	}
	moveAlpha(35,0);
        sleep(10);

/********************************************************************************/

    	clearScreen();
        drawJustifiedText("2.11 BSD on PiDP-11", 1, 0);
	drawJustifiedText("x = sin(4t), y = sin(5t)", 1, 2);
	drawJustifiedText("0 <= t <= 2*PI", 1, 3);

    	step = PI / 200.0;
    
	x1 = 0; y1 = 0;
	for (deg = step; deg < 2*PI+step; deg += step) {
		x2 = r * sin(4*deg);
		y2 = r * sin(5*deg);
		drawVector((int)(x1+mx), (int)(y1+my), (int)(x2+mx), (int)(y2+my));
		x1 = x2;
		y1 = y2;
	}
	moveAlpha(35,0);
	sleep(10);

/********************************************************************************/


    	clearScreen();
        drawJustifiedText("2.11 BSD on PiDP-11", 1, 0);
	drawJustifiedText("x = sin(9t), y = sin(10t)", 1, 2);
	drawJustifiedText("0 <= t <= 2*PI", 1, 3);

    	step = PI / 400.0;
    
	x1 = 0; y1 = 0;
	for (deg = step; deg < 2*PI+step; deg += step) {
		x2 = r * sin(9*deg);
		y2 = r * sin(10*deg);
		drawVector((int)(x1+mx), (int)(y1+my), (int)(x2+mx), (int)(y2+my));
		x1 = x2;
		y1 = y2;
	}
	moveAlpha(35,0);

/********************************************************************************/

    	exit(0);  
}
