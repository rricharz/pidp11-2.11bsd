/*  home.c 2.11 BSD clear tektronix screen	*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

#include <stdio.h>
#include <stdlib.h>

void clearScreen()
{
    putchar(27);
    putchar(12);
}

int main(argc, argv)
int argc;
char *argv[];
{
    clearScreen();
    exit(0);
}
