/*  todos 2.1 BSD printer filter		*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  Converts lf to crlf				*/

#include <stdio.h>
#include <stdlib.h>

#define LEFTMARGIN	4

int main(argc, argv)
int argc;
char *argv[];
{
	int c,i;
	for (i=0; i<LEFTMARGIN; i++) putchar(' ');
	while ((c=getchar())!=EOF) {
		putchar(c);
		if (c=='\n') {
			putchar('\r');
			for (i=0; i<LEFTMARGIN; i++) putchar(' ');
		}
	}
	exit(0);
}
