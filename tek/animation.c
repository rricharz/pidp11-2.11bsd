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
#include <strings.h>
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

drawJustifiedText(s, line, justify)
char *s; int line, justify;
/* justify 0=left, 1=center; 2=right */
{
	int length;
	length = strlen(s);
	switch (justify) {
		case 1: moveAlpha(line, 1); break;
                case 2: moveAlpha(line, (MAXCOLUMNS - length) / 2); break;
                case 3: moveAlpha(line, MAXCOLUMNS - length + 1); break;
	}
	printf(s);
}

int fix(r)
double r;
/* round double and convert to int */
{
 	if (r >= 0) return (int)(r + 0.5);
	else return (int)(r - 0.5);
}

int main(argc, argv)
int argc;
char *argv[];
{
    int i,j;
    long k;
    int r,rc;
    int px, py;
    double l, mx, my, mx2, my2;

    double rx,ry;
    double sP1, sP2;

    r = 10;         /* radius of ball */

    printf("This program requires a Tektronix 4014 terminal!\n");
    keepRunning = 1;
    signal(SIGINT,intHandler);  /* catch <ctrl>c */

    clearScreen();
    drawJustifiedText("A BOUNCING BALL on the PiDP-11", 10, 2);
    drawJustifiedText("using a Tektronix 4014 terminal", 11, 2);
    drawJustifiedText("demonstrates up to 4K graphics capabilities in the 1970's", 12, 2);


    drawJustifiedText("PiDP-11 by Oscar Vermeulen", 14, 2);
    drawJustifiedText("https://obsolescence.wixsite.com/obsolescence/pidp-11", 15, 2);

    drawJustifiedText("tek4010 terminal emulator by Rene Richarz", 17, 2);
    drawJustifiedText("https://github.com/rricharz/Tek4010", 18, 2);

    drawJustifiedText("animation on 2.11 BSD Unix by Rene Richarz", 20, 2);
    drawJustifiedText("https://github.com/rricharz/pidp11-2.11bsd", 21, 2);

    for (i = 0; i < 100; i++) printf("\007");  /* delay on the terminal emulator */

    /*************************************************************************/

    clearScreen();
    drawJustifiedText("A BOUNCING BALL on the PiDP-11", 1, 1);
    drawJustifiedText("Reflection off a circle", MAXLINES, 1);
    drawJustifiedText("Running on 2.11 BSD Unix", 1,3);
    drawJustifiedText("Using tek4014 emulator", MAXLINES,3);

    makePolygon(MAXVECTORS, (double)r);

    rc = 200;
    px = 30; py = 30; mx = 20.0; my = 10.0;

    drawRectangle(0,0,MAXX-1,MAXY-1);
    drawRectangle(1,1,MAXX-2,MAXY-2);

    drawCircle(MAXX / 2, MAXY / 2, rc);
    drawCircle(MAXX / 2, MAXY / 2, rc - 1);

    startWriteThrough();
    j = 0;
    do {
        startDraw(px + vTable[0].x, py + vTable[0].y);
    	for (i = 1; i <= MAXVECTORS; i++)
		draw(px + vTable[i].x, py + vTable[i].y);       	
        endDraw();
        fflush(stdout);
        px += fix(mx);
        py += fix(my);

	/* outer border */
	if (px < r) { px += r; mx = -mx; }
	if (px > (MAXX -r)) { px -= r; mx = -mx; }
	if (py < r) { py += r; my = -my; }
	if (py > (MAXY - r)) { py -= r; my = -my; }

	/* inner border */
	if (distanceSquared(px,py) < (double)(rc + r) * (double)(rc + r)) {

		/* ball touches circle */

		/* radius vector */
		rx = (double)(MAXX / 2 - px);
		ry = (double)(MAXY / 2 - py);

		/* normalize this vector -> lenght = 1 */
		/* cannot use rc here because we are not exactly on the border */
                l = sqrt(rx * rx + ry * ry);
		rx = rx / l;
		ry = ry / l;

		/* calculate dot product of radius and motion vectors */
		sP1 = rx * mx + ry * my;

		/* calculate dot product of tangent and motion vectors */
		sP2 = ry * mx - rx * my;

		/* project motion vector on radius and tangent */
		/* invert radial component */
		/* combine components again */
		mx2 = - rx * sP1 + ry * sP2;
		my2 = - ry * sP1 - rx * sP2;

		mx = mx2;
		my = my2;
		px = px + fix(mx);
		py = py + fix(my);
	}
        printf("\007"); /* delay the tek4010a bit to give the PiDP-11 time to keep up */
        j++;

    }
    while (keepRunning && (j < 500));
    endWriteThrough();

    if (!keepRunning) {
	moveAlpha(2,1);
	exit(0);
    }
    for (i = 0; i < 20; i++) printf("\007");  /* delay on the terminal emulator */

    /*************************************************************************/    /*************************************************************************/

    clearScreen();
    drawJustifiedText("A BOUNCING BALL on the PiDP-11", 1, 1);
    drawJustifiedText("Adding gravitation", MAXLINES, 1);
    drawJustifiedText("Running on 2.11 BSD Unix", 1, 3);
    drawJustifiedText("Using tek4014 emulator", MAXLINES,3);

    makePolygon(MAXVECTORS, (double)r);

    rc = 200;
    px = 30; py = 450; mx = 16.0; my = 24.0;

    drawRectangle(0,0,MAXX-1,MAXY-1);
    drawRectangle(1,1,MAXX-2,MAXY-2);

    drawCircle(MAXX / 2, MAXY / 2, rc);
    drawCircle(MAXX / 2, MAXY / 2, rc - 1);

    startWriteThrough();
    j = 0;
    do {
        startDraw(px + vTable[0].x, py + vTable[0].y);
    	for (i = 1; i <= MAXVECTORS; i++)
		draw(px + vTable[i].x, py + vTable[i].y);       	
        endDraw();
        fflush(stdout);
        px += fix(mx);
        py += fix(my);

	/* outer border */
	if (px < r) { px += r; mx = -mx; }
	if (px > (MAXX -r)) { px -= r; mx = -mx; }
	if (py < r) { py += r; my = -my; }
	if (py > (MAXY - r)) { py -= r; my = -my; }

	/* inner border */
	if (distanceSquared(px,py) < (double)(rc + r) * (double)(rc + r)) {

		/* ball touches circle */

		/* radius vector */
		rx = (double)(MAXX / 2 - px);
		ry = (double)(MAXY / 2 - py);

		/* normalize this vector -> lenght = 1 */
		/* cannot use rc here because we are not exactly on the border */
                l = sqrt(rx * rx + ry * ry);
		rx = rx / l;
		ry = ry / l;

		/* calculate dot product of radius and motion vectors */
		sP1 = rx * mx + ry * my;

		/* calculate dot product of tangent and motion vectors */
		sP2 = ry * mx - rx * my;

		/* project motion vector on radius and tangent */
		/* invert radial component */
		/* combine components again */
		mx2 = - rx * sP1 + ry * sP2;
		my2 = - ry * sP1 - rx * sP2 ;

		mx = mx2;
		my = my2;
		px = px + fix(mx);
		py = py + fix(my);
	}

	/* gravitation */
        my -= 0.5;

        printf("\007"); /* delay the tek4010a bit to give the PiDP-11 time to keep up */
        j++;

    }
    while (keepRunning && (j < 500));
    endWriteThrough();

    if (!keepRunning) {
	moveAlpha(2,1);
	exit(0);
    }
    for (i = 0; i < 20; i++) printf("\007");  /* delay on the terminal emulator */ 

    /*************************************************************************/

    clearScreen();
    drawJustifiedText("A BOUNCING BALL on the PiDP-11", 1, 1);
    drawJustifiedText("Reflection in a circle", MAXLINES, 1);
    drawJustifiedText("Running on 2.11 BSD Unix", 1,3);
    drawJustifiedText("Using tek4014 emulator", MAXLINES,3);

    makePolygon(MAXVECTORS, (double)r);

    rc = 320;
    px = 450; py = 100; mx = 20.0; my = 12.0;

    drawRectangle(0,0,MAXX-1,MAXY-1);
    drawRectangle(1,1,MAXX-2,MAXY-2);

    drawCircle(MAXX / 2, MAXY / 2, rc);
    drawCircle(MAXX / 2, MAXY / 2, rc - 1);

    startWriteThrough();
    j = 0;
    do {
        startDraw(px + vTable[0].x, py + vTable[0].y);
    	for (i = 1; i <= MAXVECTORS; i++)
		draw(px + vTable[i].x, py + vTable[i].y);       	
        endDraw();
        fflush(stdout);
        px += fix(mx);
        py += fix(my);

	/* outer border */
	if (px < r) { px += r; mx = -mx; }
	if (px > (MAXX -r)) { px -= r; mx = -mx; }
	if (py < r) { py += r; my = -my; }
	if (py > (MAXY - r)) { py -= r; my = -my; }

	/* inner border */
	if (distanceSquared(px,py) > (double)(rc - r) * (double)(rc - r)) {

		/* ball touches circle */

		/* radius vector */
		rx = (double)(MAXX / 2 - px);
		ry = (double)(MAXY / 2 - py);

		/* normalize this vector -> lenght = 1 */
		/* cannot use rc here because we are not exactly on the border */
                l = sqrt(rx * rx + ry * ry);
		rx = rx / l;
		ry = ry / l;

		/* calculate dot product of radius and motion vectors */
		sP1 = rx * mx + ry * my;

		/* calculate dot product of tangent and motion vectors */
		sP2 = ry * mx - rx * my;

		/* project motion vector on radius and tangent */
		/* invert radial component */
		/* combine components again */
		mx2 = - rx * sP1 + ry * sP2;
		my2 = - ry * sP1 - rx * sP2;

		mx = mx2;
		my = my2;
		px = px + fix(mx);
		py = py + fix(my);
	}
        printf("\007"); /* delay the tek4010a bit to give the PiDP-11 time to keep up */
        j++;

    }
    while (keepRunning && (j < 500));
    endWriteThrough();

    if (!keepRunning) {
	moveAlpha(2,1);
	exit(0);
    }
    for (i = 0; i < 20; i++) printf("\007");  /* delay on the terminal emulator */

    /*************************************************************************/

    clearScreen();
    drawJustifiedText("A BOUNCING BALL on the PiDP-11", 1, 1);
    drawJustifiedText("Adding gravitation", MAXLINES, 1);
    drawJustifiedText("Running on 2.11 BSD Unix", 1, 3);
    drawJustifiedText("Using tek4014 emulator", MAXLINES,3);

    makePolygon(MAXVECTORS, (double)r);

    rc = 320;
    px = 450; py = 100; mx = 10.0; my = 22.0;

    drawRectangle(0,0,MAXX-1,MAXY-1);
    drawRectangle(1,1,MAXX-2,MAXY-2);

    drawCircle(MAXX / 2, MAXY / 2, rc);
    drawCircle(MAXX / 2, MAXY / 2, rc - 1);

    startWriteThrough();
    j = 0;
    do {
        startDraw(px + vTable[0].x, py + vTable[0].y);
    	for (i = 1; i <= MAXVECTORS; i++)
		draw(px + vTable[i].x, py + vTable[i].y);       	
        endDraw();
        fflush(stdout);
        px += fix(mx);
        py += fix(my);

	/* outer border */
	if (px < r) { px += r; mx = -mx; }
	if (px > (MAXX -r)) { px -= r; mx = -mx; }
	if (py < r) { py += r; my = -my; }
	if (py > (MAXY - r)) { py -= r; my = -my; }

	/* inner border */
	if (distanceSquared(px,py) > (double)(rc - r) * (double)(rc - r)) {

		/* ball touches circle */

		/* radius vector */
		rx = (double)(MAXX / 2 - px);
		ry = (double)(MAXY / 2 - py);

		/* normalize this vector -> lenght = 1 */
		/* cannot use rc here because we are not exactly on the border */
                l = sqrt(rx * rx + ry * ry);
		rx = rx / l;
		ry = ry / l;

		/* calculate dot product of radius and motion vectors */
		sP1 = rx * mx + ry * my;

		/* calculate dot product of tangent and motion vectors */
		sP2 = ry * mx - rx * my;

		/* project motion vector on radius and tangent */
		/* invert radial component */
		/* combine components again */
		mx2 = - rx * sP1 + ry * sP2;
		my2 = - ry * sP1 - rx * sP2;

		mx = mx2;
		my = my2;
		px = px + fix(mx);
		py = py + fix(my);
	}

	/* gravitation */
        my -= 1.0;

        printf("\007"); /* delay the tek4010a bit to give the PiDP-11 time to keep up */
        j++;

    }
    while (keepRunning && (j < 500));
    endWriteThrough();

    if (!keepRunning) {
	moveAlpha(2,1);
	exit(0);
    }
    for (i = 0; i < 20; i++) printf("\007");  /* delay on the terminal emulator */

    /*************************************************************************/


    clearScreen();
    drawJustifiedText("A BOUNCING BALL on the PiDP-11", 1, 1);
    drawJustifiedText("Gravitation towards center of circle", MAXLINES, 1);
    drawJustifiedText("Running on 2.11 BSD Unix", 1,3);
    drawJustifiedText("Using tek4014 emulator", MAXLINES,3);

    makePolygon(MAXVECTORS, (double)r);

    rc = 200;
    px = 250; py = 190; mx = 5; my = 11;

    drawRectangle(0,0,MAXX-1,MAXY-1);
    drawRectangle(1,1,MAXX-2,MAXY-2);

    drawCircle(MAXX / 2, MAXY / 2, rc);
    drawCircle(MAXX / 2, MAXY / 2, rc - 1);

    startWriteThrough();
    j = 0;
    do {
        startDraw(px + vTable[0].x, py + vTable[0].y);
    	for (i = 1; i <= MAXVECTORS; i++)
		draw(px + vTable[i].x, py + vTable[i].y);       	
        endDraw();
        fflush(stdout);
        px += fix(mx);
        py += fix(my);

	/* outer border */
	if (px < r) { px += r; mx = -mx; }
	if (px > (MAXX -r)) { px -= r; mx = -mx; }
	if (py < r) { py += r; my = -my; }
	if (py > (MAXY - r)) { py -= r; my = -my; }

	/* inner border */
	if (distanceSquared(px,py) < (double)(rc + r) * (double)(rc + r)) {

		/* radius vector */
		rx = (double)(MAXX / 2 - px);
		ry = (double)(MAXY / 2 - py);

		/* normalize this vector -> lenght = 1 */
		/* cannot use rc here because we are not exactly on the border */
                l = sqrt(rx * rx + ry * ry);
		rx = rx / l;
		ry = ry / l;

		/* ball touches circle */

		/* calculate dot product of radius and motion vectors */
		sP1 = rx * mx + ry * my;

		/* calculate dot product of tangent and motion vectors */
		sP2 = ry * mx - rx * my;

		/* project motion vector on radius and tangent */
		/* invert radial component */
		/* combine components again */
		mx2 = - rx * sP1 + ry * sP2;
		my2 = - ry * sP1 - rx * sP2;

		mx = mx2;
		my = my2;
		px = px + fix(mx);
		py = py + fix(my);
	}
	
	/* gravity */
	/* calculate radius vector */
	rx = (double)(MAXX / 2 - px);
	ry = (double)(MAXY / 2 - py);
	/* normalize this vector -> lenght = 1 */
        l = sqrt(rx * rx + ry * ry);
	rx = rx / l;
	ry = ry / l;
	mx = mx + 2.0 * rx;
        my = my + 2.0 * ry;

        printf("\007"); /* delay the tek4010a bit to give the PiDP-11 time to keep up */
        j++;

    }
    while (keepRunning && (j < 500));
    endWriteThrough();

    moveAlpha(2,1);
    exit(0);  
}
