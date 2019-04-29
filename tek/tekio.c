/*  tecio.c 2.11 BSD Tektronix graphics 	*/
/*  io functions for Tektronix 4010             */
/*  For 2.11BSD Unix on PiDP-11	                */
/*  cc 2019 rricharz                            */
/*  Uses Berkley C syntax			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/
/*  See https://github.com/rricharz/Tek4010     */

#include <stdio.h>
#include <math.h>
#include "tekio.h"

static int xs,ys;

void startDraw(x1,y1)
int x1,y1;
{
    if (x1 < 0) x1 = 0;
    if (x1 >= MAXX) x1 = MAXX -1;
    if (y1 < 0) y1 = 0;
    if (y1 >= MAXY) y1 = MAXY - 1;
    putchar(29);
    putchar((y1 >> 5) + 32);
    putchar((y1 & 31) + 96);
    putchar((x1 >> 5) + 32);
    putchar((x1 & 31) + 64);
    xs = x1;
    ys = y1;
}

extern void draw(x2,y2)
int x2,y2;
{
    int hxchange, lychange;

    if (x2 < 0) x2 = 0;
    if (x2 >= MAXX) x2 = MAXX -1;
    if (y2 < 0) y2 = 0;
    if (y2 >= MAXY) y2 = MAXY - 1;

    if ((y2 >> 5) != (ys >> 5))        /* if high y has changed */
	putchar((y2 >> 5) + 32);
    hxchange = (x2 >> 5) != (xs >> 5);
    lychange = (y2 & 31) != (ys & 31);
    if (hxchange || lychange) putchar((y2 & 31) + 96);
    if (hxchange)                                     /* if high order x has changed */
    	putchar((x2 >> 5) + 32);
    putchar((x2 & 31) + 64);
    xs = x2;
    ys = y2;
}

extern void endDraw()
{
    putchar(31);
    fflush(stdout);
}

void moveTo(x1,y1)
int x1,y1;
{
    startDraw(x1,y1);
    endDraw();
}

void moveAlpha(line, column)
int line,column;
{
    int vDotsPerChar;
    double hDotsPerChar;
    if (line < 1) line = 1;
    if (line > MAXLINES) line = MAXLINES;
    if (column < 1) column = 1;
    if (column > MAXCOLUMNS) column = MAXCOLUMNS;
    vDotsPerChar = MAXY / MAXLINES;
    hDotsPerChar = (double)(MAXX)/(double)(MAXCOLUMNS);
    moveTo((int)((double)(column - 1) * hDotsPerChar), MAXY - line * vDotsPerChar);
}

void drawVector(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
    startDraw(x1,y1);
    if (x2 < 0) x2 = 0;
    if (x2 >= MAXX) x2 = MAXX -1;
    if (y2 < 0) y2 = 0;
    if (y2 >= MAXY) y2 = MAXY - 1;
    draw(x2,y2);
    endDraw();
}

void clearScreen()
{
    putchar(27);
    putchar(12);
}


extern int startWriteThrough()
{
    putchar(27);
    putchar('p');
    fflush(stdout);	
}

extern int endWriteThrough()
{
    putchar(27);
    putchar('`');
    fflush(stdout);
}

int drawRectangle(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
    startDraw(x1,y1);
    draw(x2,y1);
    draw(x2,y2);
    draw(x1,y2);
    draw(x1,y1);
    endDraw();
}

drawCircle(x,y,r)
int x,y,r;
{
	int i;
	double arg;
    	startDraw(x + r, y);
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

void setCharacterSize(size)
int size;
{
	if ((size >= 1) && (size <= 4)) {
		putchar(27);
		putchar('7' + size);
	}
	else
		printf("setCharacterSize: Illegal size %d\n", size);
}

void setLineMode(type)
int type;
{
	if ((type >= SOLID) && (type <= LONGDASH)) {
		putchar(27);
		putchar(95 + type);
	}
	else
		printf("SetLineMode: illegal type %d\n", type);
}


