#include <stdio.h>
#include <math.h>

#include "tekio.h"

int main(argc, argv)
int argc;
char *argv[];
{
	clearScreen();

	drawCircle(512, 340, 200);
        drawCircle(612, 340, 100);
        drawCircle(412, 340, 100);
        drawCircle(512, 240, 100);
	drawCircle(512, 440, 100);

	startDraw(212,340); draw(812,340); endDraw();
	startDraw(512,040); draw(512,640); endDraw();

	moveAlpha(1,1); /* move the cursor out of the way */
}
