/*  tek.c 2.11 BSD tektroxix graphics test	*/
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

#include <stdio.h>
#include <stdlib.h>

#define MAXX 1023
#define MAXY 780

void sendX(x)
int x;
{
    putchar((x >> 5) + 32);
    putchar((x & 31) + 64);
}

void sendY(y)
int y;
{
    putchar((y >> 5) + 32);
    putchar((y & 31) + 96);
}

void drawVector(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
    putchar(29);
    sendY(y1);
    sendX(x1);
    sendY(y2);
    sendX(x2);
    putchar(13);
}

void clearScreen()
{
    putchar(27);
    putchar(12);
}

void endScreen()
{
    getchar();
    clearScreen();
}

int main(argc, argv)
int argc;
char *argv[];
{
    int i,s,step;
    printf("This program requires a Tektronix terminal!\n");
    clearScreen();
    for (i=0; i<MAXY; i+=10)
      drawVector(0,i,MAXX,MAXY-i);
    step=28;
    for (i=0; i<MAXY/2; i+=step) {
      if (step--<1) step=1;
      s = (int)((double)i*(double)MAXX/(double)MAXY);
      drawVector(s,i,MAXX-s,i);
      drawVector(s,MAXY-i,MAXX-s,MAXY-i);
    }
    endScreen();
    exit(0);  
}
