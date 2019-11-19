/*  mathdemo2.c 2.11 BSD tektroxix graphics test	*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11		*/
/*  Uses Berkley C syntax!				*/
/*  If you want to use this, you need to		*/
/*  login from a Tektronix terminal or emulator		*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tekiohres.h"

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

void drawmasked(x1, y1, x2, y2, visible)
int x1, y1,x2, y2, visible;
{
	int i, doDraw, start, vstart;
	double step, v;

        if (x2 == x1)
		step = (double)(y2-y1);
	else {
		step = (double)(y2 - y1)/(double)(x2 - x1);
		if ((x2 - x1) < 0) step = - step;
	}

	doDraw = 0;
	v = (double)y1;
	start = x1;
	vstart = y1;

        for (i = x1; i < x2; i++) {		/* check mask for each point */
		if (fix(v) <= mask[i]) {	/* hidden point */
			if (doDraw) {		/* draw up to here */
				if ((i - x1 > 1) && visible)
					drawVector(start, vstart, i, fix(v));
			}
			doDraw = 0;
		}
		else {				/* visible point */
			mask[i] = fix(v);	/* update mask */
			if (doDraw == 0) {	/* start draw from here */
				start = i;
				vstart = fix(v);
			}
			doDraw = 1;
		}
		v = v + step;
	}

	for (i = x1; i > x2; i--) {		/* check mask for each point */
		if (fix(v) <= mask[i]) {	/* hidden point */
			if (doDraw) {		/* draw up to here */
				if (x1 - i > 1)
					drawVector(start, vstart, i, fix(v));
			}
			doDraw = 0;
		}
		else {				/* visible point */
			mask[i] = fix(v);	/* update mask */
			if (doDraw == 0) {	/* start draw from here */
				start = i;
				vstart = fix(v);
			}
			doDraw = 1;
		}
		v = v + step;
	}

	if (doDraw && visible)
		drawVector(start, vstart, x2, y2);
	if (y2 < mask[x2])
		mask[x2] = y2;
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
	scale = 200.0;

	dx = (double)MAXX / (2.2 * max);
	dy = dx /2.0;

	for (i = 0; i < MAXX; i++)
		mask[i] = 0;

	/* this is required to generate a first mask*/
	x = 0.0; y = 0.0;
	x1 = fix(dx * x - dx * y);
	y1 = fix(dy * x + dy * y + f1(x, y) * scale);
        for (x = 0; x < max; x += step) {
		x2 = fix(dx * x - dx * y);
		y2 = fix(dy * x + dy * y + f1(x, y) * scale);
		drawmasked(fix(x1 + sx), fix(y1 + sy) - 1, fix(x2 + sx), fix(y2 + sy) - 1, 0);
		x1 = x2;
		y1 = y2;
	}
    
	for (x = 0; x < max; x += step) {
		y = 0.0;
		x1 = fix(dx * x - dx * y);
		y1 = fix(dy * x + dy * y + f1(x, y) * scale);
                for (y = 0; y < max; y += step) {
			x2 = fix(dx * x - dx * y);
			y2 = fix(dy * x + dy * y + f1(x, y) * scale);
			drawmasked(fix(x1 + sx), fix(y1 + sy), fix(x2 + sx), fix(y2 + sy), 1);
			x1 = x2;
			y1 = y2;
		}
	}

	for (i = 0; i < MAXX; i++)
		mask[i] = 0;

	/* this is required to generate a first mask*/
	x = 0.0; y = 0.0;
	x1 = fix (dx * x - dx * y);
	y1 = fix(dy * x + dy * y + f1(x, y) * scale);
        for (y = 0; y < max; y += step) {
		x2 = fix(dx * x - dx * y);
		y2 = fix(dy * x + dy * y + f1(x, y) * scale);
		drawmasked(fix(x1 + sx), fix(y1 + sy) - 1, fix(x2 + sx), fix(y2 + sy) - 1, 0);
		x1 = x2;
		y1 = y2;
	}

	for (y = 0; y < max; y += step) {
		x = 0.0;
		x1 = fix(dx * x - dx * y);
		y1 = fix(dy * x + dy * y + f1(x, y) * scale);
                for (x = 0; x < max; x += step) {
			x2 = fix(dx * x - dx * y);
			y2 = fix(dy * x + dy * y + f1(x, y) * scale);
			drawmasked(fix(x1 + sx), fix(y1 + sy), fix(x2 + sx), fix(y2 + sy), 1);
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
	scale = 800.0;

	dx = (double)MAXX / (2.2 * max);
	dy = dx /2.0;

	for (i = 0; i < MAXX; i++)
		mask[i] = 0;

	/* this is required to generate a first mask*/
	x = 0.0; y = 0.0;
	x1 = fix(dx * x - dx * y);
	y1 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
        for (x = 0; x < max; x += step) {
		x2 = fix(dx * x - dx * y);
		y2 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
		drawmasked(fix(x1 + sx), fix(y1 + sy) - 1, fix(x2 + sx), fix(y2 + sy) - 1, 0);
		x1 = x2;
		y1 = y2;
	}
    
	for (x = 0; x < max; x += step) {
		y = 0.0;
		x1 = fix(dx * x - dx * y);
		y1 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
                for (y = 0; y < max; y += step) {
			x2 = fix(dx * x - dx * y);
			y2 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
			drawmasked(fix(x1 + sx), fix(y1 + sy), fix(x2 + sx), fix(y2 + sy), 1);
			x1 = x2;
			y1 = y2;
		}
	}

	for (i = 0; i < MAXX; i++)
		mask[i] = 0;

	/* this is required to generate a first mask*/
	x = 0.0; y = 0.0;
	x1 = fix (dx * x - dx * y);
	y1 = fix(dy * x + dy * y + f2(x -max/2, y - max/2) * scale);
        for (y = 0; y < max; y += step) {
		x2 = fix(dx * x - dx * y);
		y2 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
		drawmasked(fix(x1 + sx), fix(y1 + sy) - 1, fix(x2 + sx), fix(y2 + sy) - 1, 0);
		x1 = x2;
		y1 = y2;
	}

	for (y = 0; y < max; y += step) {
		x = 0.0;
		x1 = fix(dx * x - dx * y);
		y1 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
                for (x = 0; x < max; x += step) {
			x2 = fix(dx * x - dx * y);
			y2 = fix(dy * x + dy * y + f2(x - max/2, y - max/2) * scale);
			drawmasked(fix(x1 + sx), fix(y1 + sy), fix(x2 + sx), fix(y2 + sy), 1);
			x1 = x2;
			y1 = y2;
		}
	}

	moveAlpha(35,0);

/********************************************************************************/
    	exit(0);  
}
