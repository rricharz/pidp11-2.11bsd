/*  graph.c 2.11 BSD tektronix graphics test	*/
/*  display weather graphics                    */
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

#include <stdio.h>
#include <stdlib.h>

#define MAXX 1023
#define MAXY 780
#define MAXCH 74

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
    sendX(0);
    putchar(13);
    pos=(int)((double)x*(double)MAXCH/(double)MAXX);
    for (i=0; i<pos;i++) putchar(' ');
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

int main(argc, argv)
int argc;
char *argv[];
{
    char *fname = "../weather/data/20190312";
    FILE *f;
    double maxTemp = -273.0;
    double minTemp =  200.0;
    int maxTime = 0;
    int minTime = 24*60;
    int i,time,t1,t2,timevalue,value,first,lasttimevalue,lastvalue,lasttime;
    double T,P,H;
    int minGX=60;
    int maxGX=MAXX-40;
    int minGY=40;
    int maxGY=MAXY-50;

    printf("This program requires a Tektronix terminal!\n");
    if (argc!=2) {
        printf("Usage: graph filename\n");
        exit(1);
    }
    fname=argv[1];

    /* clear screen and draw a box */
    clearScreen();
    
    printf("             Temperature data in %s\n",fname);
    drawVector(minGX,minGY,maxGX,minGY);
    drawVector(maxGX,minGY,maxGX,maxGY);
    drawVector(maxGX,maxGY,minGX,maxGY);
    drawVector(minGX,maxGY,minGX,minGY);

    if (f=fopen(fname,"r")) {
       /* first read maxima and minima for limits of graph */
       while (fscanf(f,"%d:%d %f %f %f\n", &t1, &t2, &T, &P, &H)!=EOF) {
           time=60*t1+t2;
           if (time>maxTime) maxTime=time;
           if (time<minTime) minTime=time;
           if (T>maxTemp) maxTemp=T;
           if (T<minTemp) minTemp=T;
       }
       moveTo(0,maxGY-9); printf("%0.1f",maxTemp);
       moveTo(0,minGY-9); printf("%0.1f",minTemp);
       moveTo(minGX-25,2); printf("%2d:%02d",minTime/60,minTime%60);
       moveTo(maxGX-30,2); printf("%2d:%02d",maxTime/60,maxTime%60);
       /* rewind, then read and display data */
       rewind(f);
       first=0;
       while (fscanf(f,"%d:%d %f %f %f\n", &t1, &t2, &T, &P, &H)!=EOF) {
           /* scale values */
           time=60*t1+t2;
           timevalue=(int)((double)(60*t1+t2-minTime)*(double)(maxGX-minGX)
                                           /(double)(maxTime-minTime));
           value=(int)((double)(T-minTemp)*(double)(maxGY-minGY)
                                           /(double)(maxTemp-minTemp));
           if (first) {
               first=0;
               lasttime=time;
               lasttimevalue=timevalue;
               lastvalue=value;
           }
           if (time-lasttime<=2)
               drawVector(minGX+lasttimevalue,minGY+lastvalue,
                                         minGX+timevalue,minGY+value);
           else /* no measurements done between last and current time */
               drawVector(minGX+timevalue,minGY+value,
                                         minGX+timevalue,minGY+value);
           lasttime=time;
           lasttimevalue=timevalue;
           lastvalue=value;
       }
       fclose(f);
    }

    else {
      printf("Cannot open data file %s\n",fname);
      exit(1);
    }
  
    endScreen();
    exit(0);  
}
