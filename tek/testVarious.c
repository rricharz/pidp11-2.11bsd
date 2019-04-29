/*
  testVarious.c 
  
  on PiPD11 using 2.11BSD Unix

  includes the following tests for tek4010 emulator

  - Character Size

  2019  rricharz
*/

#include <stdio.h>
#include <stdlib.h>
#include "tekio.h"

void printCharacterSet(size)
int size;
{
	int i;
	setCharacterSize(size);
        printf("Character size %d: ", size);
	for (i = 33; i < 127; i++)
		putchar(i);
	printf("\n");
}

int main (argc,argv)
int argc;
char *argv[];
{
	int i;

	clearScreen();

	for (i = 1; i <= 4; i++)
		printCharacterSet(i);
	
	setCharacterSize(1);

	moveAlpha(8,1);
	printf("Line types:\n");
	for (i = 1; i <= 5; i++) {
		setLineMode(i);
		drawVector(0, 600 - 10 * i, MAXX, 600 - 10 * i);
	}

	setLineMode(SOLID);

	moveAlpha(15,1);
	printf("Intensity:\n");
	for (i = 64; i <= 119; i++) {	/* table F-6 page F-9 Tektronix 4014 user manual */
		putchar(27);
		putchar(28);
		putchar(i);
		putchar(13);
		drawVector(0, 600 - 4 * i, MAXX, 600 - 4 * i);
	}
	putchar(27);
	putchar(28);
	putchar('w');
	putchar(13);
	
        moveAlpha(35,1);
}
