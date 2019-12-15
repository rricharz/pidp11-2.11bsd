/*  tsync.c 2.11 BSD synchronize t4010		*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

#include <stdio.h>
#include <stdlib.h>

int main(argc, argv)
int argc;
char *argv[];
{
    int ch;
        system("stty -echo");
    putchar(27);
    putchar(5);
    do {
	ch = getchar();
        if (ch == 3) { /* ctrl-c */
            system("stty echo");
            exit(1);
        }
    }
    while (ch != 10);
    system("stty echo");
    exit(0);
}
