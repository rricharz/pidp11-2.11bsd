/*  tecio.c 2.11 BSD Tektronix graphics 	*/
/*  minimal io functions for Tektronix 4010     */
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

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

void moveTo(x,y)
int x,y;
{
    int i,pos;
    putchar(29);
    sendY(y);
    sendX(x);
    putchar(31);
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
    moveTo(0,0);
    getchar();
    clearScreen();
}
