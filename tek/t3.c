/*  t3.c rricharz 2019 for 2.11BSD on PiDP-11	*/
/*  switch to character size 3 (small)          */
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

#include <stdio.h>
#include <stdlib.h>

void t3()
{
    putchar(27);
    putchar(':');
}

int main(argc, argv)
int argc;
char *argv[];
{
    t3();
    exit(0);
}
