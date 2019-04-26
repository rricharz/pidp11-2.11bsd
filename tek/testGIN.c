/*  testGIN.c 2.11 BSD tektroxix graphics test	*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

/*  test for ENQUIRE				*/         

#include <stdio.h>
#include <stdlib.h>
#include "tekio.h"

int getCharacter()
{
    int c;
    c = getchar();
    /* printf("%d\n",c); */
    return (c & 127);
}

void enquire(x, y)
int *x, *y;
{
    int status;
    putchar(27);
    putchar(5);
    system("stty -echo");
    status = getCharacter() & 127;
    *x  = (getCharacter() & 31) << 5;
    *x += (getCharacter() & 31);
    *y  = (getCharacter() & 31) << 5;
    *y += (getCharacter() & 31);
    getCharacter();		/* ignore lf */
    system("stty echo");
}

void ginMode(c, x, y)
int *c, *x, *y;
{
    putchar(27);
    putchar(26);
    system("stty -echo");
    *c = getCharacter() & 127;
    *x  = (getCharacter() & 31) << 5;
    *x += (getCharacter() & 31);
    *y  = (getCharacter() & 31) << 5;
    *y += (getCharacter() & 31);
    getCharacter();		/* ignore lf */
    system("stty echo");
    
}

int main(argc, argv)
int argc;
char *argv[];
{
    int i,s,step,c,x,y;
    printf("This program requires a Tektronix terminal!\n");
    clearScreen();

    moveAlpha(1,1);
    printf("Test Enquire mode\n");
    
    drawVector(MAXX / 2, MAXY / 2, 1 + MAXX / 2, MAXY / 2); 
    enquire(&x, &y);

    moveAlpha(2,1);
    printf("Enquire returns (%d,%d)\n", x , y);
    printf("Enquire complete\n");

    printf("Test GIN mode, use mouse clicks to move crosshair cursor, then\n");
    printf("type any character to send character and coordinates to host\n");

    drawVector(MAXX / 2, MAXY / 2, 1 + MAXX / 2, MAXY / 2);
    ginMode(&c,&x,&y);

    moveAlpha(6,1);
    printf("GIN returns c=%d, x=%d, y=%d\n", c, x, y);
    
    exit(0);  
}
