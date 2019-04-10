/*  mathdemo2.c 2.11 BSD tektroxix graphics test	*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11		*/
/*  Uses Berkley C syntax!				*/
/*  If you want to use this, you need to		*/
/*  login from a Tektronix terminal or emulator		*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tekio.h"

int mask[MAXX];

double f1(x,y)
double x,y;
{	
	return sin(2 * x + y);
}

double f2(x,y)
double x,y;
{	
	double r = sqrt(x * x + y * y);
	if (r == 0) return 1;
	else return sin(r) / r;
}


/* The masking algorithm is crude. Hidden vectors should not be drawn	*/
/* and partially hidden vectors require interpolated end points		*/

void drawmasked(x1, y1, x2, y2)
int x1, y1,x2, y2;
{
	int i, xa, xb, w;
	int y1masked, y2masked;
        double step, first;

	if (y1 >= mask[x1])
		y1masked = y1;
	else
		y1masked = mask[x1];

	if (y2 >= mask[x2])
		y2masked = y2;
	else
		y2masked = mask[x2];

	drawVector(x1, y1masked, x2, y2masked);

        if (x2 >= x1) {
		if ((x2 - x1) == 0) step = (double)(y2 - y1);
		else step = (double)(y2 - y1) / (double)(x2 - x1);
		first = (double)y1;
		xa = x1;
		xb = x2;
	}
	else {
		if ((x2 - x1) == 0) step = (double)(y2 - y1);
		else step = (double)(y1 - y2) / (double)(x1 - x2);
		first = (double)y2;
		xa = x2;
		xb = x1;
	}

	for (i = xa; i <= xb; i++) {
			if (mask[i] < fix(first)) mask[i] = fix(first);
			first += step;
	}
}

int main(argc, argv)
int argc;
char *argv[];
{
	int i,x1,y1,x2,y2;
    	double x, y, step, max, scale;
	double sx = MAXX/2;   	/* starting point */
	double sy = MAXY/8;
        double dx = 6;		/* offset to next point */
        double dy = 3;
    	printf("This program requires a Tektronix terminal!\n");

/********************************************************************************/

    	clearScreen();
        drawJustifiedText("2.11 BSD on PiDP-11", 1, 0);
	drawJustifiedText("sin(2x + y)", 1, 3);

    	step = 0.1;
	max = PI2;
	scale = 50.0;

	dx = (double)MAXX / (2.2 * max);
	dy = dx /2.0;

	for (i = 0; i < MAXX; i++)
		mask[i] = 0;
    
	for (x = 0; x < max; x += step) {
		y = 0.0;
		x1 = fix(dx * x - dx * y);
		y1 = fix(dy * x + dy * y + f1(x, y) * scale);
                for (y = 0; y < max; y += step) {
			x2 = fix(dx * x - dx * y);
			y2 = fix(dy * x + dy * y + f1(x, y) * scale);
			drawmasked(fix(x1 + sx), fix(y1 + sy), fix(x2 + sx), fix(y2 + sy));
			x1 = x2;
			y1 = y2;
		}
	}

	for (i = 0; i < MAXX; i++)
		mask[i] = 0;

	for (y = 0; y < max; y += step) {
		x = 0.0;
		x1 = fix(dx * x - dx * y);
		y1 = fix(dy * x + dy * y + f1(x, y) * scale);
                for (x = 0; x < max; x += step) {
			x2 = fix(dx * x - dx * y);
			y2 = fix(dy * x + dy * y + f1(x, y) * scale);
			drawmasked(fix(x1 + sx), fix(y1 + sy), fix(x2 + sx), fix(y2 + sy));
			x1 = x2;
			y1 = y2;
		}
	}

	moveAlpha(35,0);
	
/********************************************************************************/

    	clearScreen();
        drawJustifiedText("2.11 BSD on PiDP-11", 1, 0);
	drawJustifiedText("sin(r) / r", 1, 3);

    	step = 0.4;
	max = 8 * PI;
	scale = 200.0;

	dx = (double)MAXX / (2.2 * max);
	dy = dx /2.0;

	for (i = 0; i < MAXX; i++)
		mask[i] = 0;
    
	for (x = 0; x < max; x += step) {
		y = 0.0;
		x1 = fix(dx * x - dx * y);
		y1 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
                for (y = 0; y < max; y += step) {
			x2 = fix(dx * x - dx * y);
			y2 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
			drawmasked(fix(x1 + sx), fix(y1 + sy), fix(x2 + sx), fix(y2 + sy));
			x1 = x2;
			y1 = y2;
		}
	}

	for (i = 0; i < MAXX; i++)
		mask[i] = 0;

	for (y = 0; y < max; y += step) {
		x = 0.0;
		x1 = fix(dx * x - dx * y);
		y1 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
                for (x = 0; x < max; x += step) {
			x2 = fix(dx * x - dx * y);
			y2 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
			drawmasked(fix(x1 + sx), fix(y1 + sy), fix(x2 + sx), fix(y2 + sy));
			x1 = x2;
			y1 = y2;
		}
	}

	moveAlpha(35,0);

/********************************************************************************/
    	exit(0);  
}
