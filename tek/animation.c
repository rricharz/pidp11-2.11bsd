/*  animation.c 2.11 BSD tektroxix graphics test*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or the	*/
/*  teh4010 emulator. Does not work with xterm  */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include "tekio.h"

#define MAXVECTORS 12		/* number of animated vectors */
#define PI2 6.283185307		/* value of 2 pi */

static int keepRunning;

void intHandler(dummy)    /* catch <ctrl>c */
{
   keepRunning=0;
}

struct vectorTable {
	int x;
	int y;
} vTable[MAXVECTORS + 1];

void makePolygon(n, r)
int n; double(r);
{
	int i;
        double arg;
	vTable[0].x = r; vTable[0].y = 0;
	for (i = 1; i <= n; i++) {
		arg = (double)(i) * PI2 / (double)(n);
		vTable[i].x = (int)(r * cos(arg));
                vTable[i].y = (int)(r * sin(arg));
	}
}

double distanceSquared(x,y)
{
	return (double)(x - (MAXX / 2)) * (double)(x - (MAXX / 2)) +
     		(double)(y - (MAXY / 2)) * (double)(y - (MAXY / 2));
}

drawCircle(x,y,r)
int x,y,r;
{
	int i;
	double arg;
    	startDraw((MAXX / 2) + r, (MAXY /2));
    	for (i = 0; i <= r; i++) {
        	arg = (double)(i) * PI2 / (double) r;
		draw(x + (int)((double)r * cos(arg)), y + (int)((double)r * sin(arg)));
    	}
    	endDraw;

}

int main(argc, argv)
int argc;
char *argv[];
{
    int i,j;
    long k;
    int r,rc;
    int px,py,mx,my,mx2,my2;

    double rx,ry;
    double sP1, sP2;

    r = 10;         /* radius of ball */
    rc = 200;       /* radius of circle */

    printf("This program requires a Tektronix 4014 terminal!\n");
    keepRunning = 1;
    signal(SIGINT,intHandler);  /* catch <ctrl>c */
    clearScreen();
    makePolygon(MAXVECTORS, (double)r);

    px = 30; py = 30; mx = 20; my = 11;

    drawRectangle(0,0,MAXX-1,MAXY-1);
    drawRectangle(1,1,MAXX-2,MAXY-2);

    drawCircle(MAXX / 2, MAXY / 2, rc);

    startWriteThrough();
    j = 0;
    do {
        startDraw(px + vTable[0].x, py + vTable[0].y);
    	for (i = 1; i <= MAXVECTORS; i++)
		draw(px + vTable[i].x, py + vTable[i].y);       	
        endDraw();
        fflush(stdout);
        px += mx;
        py += my;

	/* outer border */
	if (px < r) { px += r; mx = -mx; }
	if (px > (MAXX -r)) { px -= r; mx = -mx; }
	if (py < r) { py += r; my = -my; }
	if (py > (MAXY - r)) { py -= r; my = -my; }

	/* inner border */
	if (distanceSquared(px,py) < (double)(rc + r) * (double)(rc + r)) {

		/* ball touches circle */

		/* vector to center of circle */
		rx = (double)((MAXX / 2) - px);
		ry = (double)((MAXY / 2) - py);

		/* normalize this vector -> lenght = 1 */
		rx = rx / (double)(rc);
		ry = ry / (double)(rc);

		/* calculate the dot product of this vector, and of the perpendicular vector */
		sP1 = rx * (double) mx + ry * (double) my;
		sP2 = ry * (double) mx - rx * (double) my;

		/* make projection and add components */
		mx2 = (int)(- rx * sP1 + ry * sP2);
		my2 = (int)(- ry * sP1 - rx * sP2 );

		mx = mx2;
		my = my2;
		px = px + mx;
		py = py + my;
	}
        j++;

    }
    while (keepRunning && (j < 1000));
    endWriteThrough();

    moveAlpha(1,1);
    exit(0);  
}
