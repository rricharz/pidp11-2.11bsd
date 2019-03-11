/*  todos 2.1 BSD printer filter		*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  Converts lf to crlf				*/
/*  Adds a left margin for puncher and folder	*/
/*  Converts tabs to spaces			*/

#include <stdio.h>
#include <stdlib.h>

#define LEFTMARGIN	6

int main(argc, argv)
int argc;
char *argv[];
{
	int c,i,pos;
	for (i=0; i<LEFTMARGIN; i++) putchar(' ');
	pos=0;	/* tab expansion after left margin */
	while ((c=getchar())!=EOF) {
		if (c=='\n') {
			putchar('\r');
			pos=0;
		}
		if (c=='\t')
			do putchar(' ');
			while (pos++ % 8);
		else {
			putchar(c); pos++;
		}
		if (c=='\n')
			for (i=0; i<LEFTMARGIN; i++) putchar(' ');
	}
	exit(0);
}
