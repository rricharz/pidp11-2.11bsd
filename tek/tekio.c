/*  tecio.c 2.11 BSD Tektronix graphics 	*/
/*  io functions for Tektronix 4010             */
/*  For 2.11BSD Unix on PiDP-11	                */
/*  cc 2019 rricharz                            */
/*  Uses Berkley C syntax			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/
/*  See https://github.com/rricharz/Tek4010     */

#include "tekio.h"

static int xs,ys;

void startDraw(x1,y1)
int x1,y1;
{
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
}

void moveTo(x1,y1)
int x1,y1;
{
    if (x1 < 0) x1 = 0;
    if (x1 >= MAXX) x1 = MAXX -1;
    if (y1 < 0) y1 = 0;
    if (y1 >= MAXY) y1 = MAXY - 1;
    startDraw(x1,y1);
    endDraw();
}

void moveAlpha(line, column)
int line,column;
{
    int vDotsPerChar, hDotsPerChar;
    if (line < 1) line = 1;
    if (line > MAXLINES) line = MAXLINES;
    if (column < 1) column = 1;
    if (column > MAXCOLUMNS) column = MAXCOLUMNS;
    hDotsPerChar  = MAXX / MAXCOLUMNS;
    vDotsPerChar  = MAXY / MAXLINES;
    moveTo((column - 1) * hDotsPerChar, MAXY - line * vDotsPerChar);
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
}

extern int endWriteThrough()
{
    putchar(27);
    putchar('`');
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

