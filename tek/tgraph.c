/*  tgraph.c 2.11 BSD Tektronix graphics 	*/
/*  display weather graphics                    */
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "tekio.h"

static int keepRunning;

void intHandler(dummy)    /* catch <ctrl>c */
{
   keepRunning=0;
}

int minGX;		/* Coordinates of graphic box */
int maxGX;
int minGY;
int maxGY;

void yAxis(min,max)
double min,max;
{
    double first,step,val,start;
    int ypos;
    step=1.0;
    while (((max-min)/step)>8) step=2.0*step;
    while (((max-min)/step)<4) {
        step=step/2.0;
        if ((step<0.3) && (step>0.2)) step=0.2;
        }
    start=step*(double)((int)(min/step+1.001));
    for (val=start; val<max; val=val+step) {
        ypos=(int)((double)(val-min)*(double)(maxGY-minGY)
                             /(double)(max-min)+(double)minGY);
        drawVector(minGX-20,ypos,minGX-1,ypos);
        moveTo(0, ypos-8);
        if (step > 0.1)
          printf("%0.1f",val);
        else
          printf("%0.2f",val);
    }
}

void xAxis(min,max)
int min,max;
{
    int first,step,val,start;
    int xpos;
    step=1;
    while (((max-min)/step)>8) {
      step=2*step;
      if (step==4) step=5;
      if (step==40) step=60;
    }
    start=step*((min/step)+1);
    for (val=start; val<max; val=val+step) {
        xpos=(int)((double)(val-min)*(double)(maxGX-minGX)
                             /(double)(max-min) + (double)minGX);
        drawVector(xpos,minGY-20,xpos,minGY-1);
        moveTo(xpos-32,minGY-40);
        printf("%2d:%02d",val/60,val%60);
    }
}

int main(argc, argv)
int argc;
char *argv[];
{
    FILE *f;
    char *fname;

    double maxTemp;
    double minTemp;
    int maxTime;
    int minTime;

    int i,time,t1,t2,timevalue,value,first,lasttimevalue,lastvalue,lasttime;
    double T,P,H;

    minGX=80;
    maxGX=MAXX-40;
    minGY=70;
    maxGY=MAXY-50;

    printf("This program requires a Tektronix terminal!\n");

    keepRunning = 1;
    signal(SIGINT,intHandler);  /* catch <ctrl>c */

    if (argc!=2) {
        printf("Usage: graph [-l] filename\n");
        exit(1);
    }
    fname=argv[argc-1];

    do {

        /* clear screen and draw a box */
        clearScreen();

        maxTemp = -273.0;
        minTemp =  200.0;
        maxTime = 0;
        minTime = 24*60;
    
        printf("             Temperature data from %s\n",fname);
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
           if (maxTemp<=minTemp) maxTemp=minTemp+0.2;
           if (maxTime<=minTime) maxTime=minTime+10;

           /* display axes */
           yAxis(minTemp,maxTemp);
           xAxis(minTime,maxTime);

           /* rewind, then read and display data */
           rewind(f);
           first=1;
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
               else if (time-lasttime<=2)
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
           moveTo(0,0);
           printf("Type <ctrl>c to clear screen and exit");
           fflush(stdout);
        }

        else {
          printf("Cannot open data file %s\n",fname);
          keepRunning=0;
          exit(1);
        }
        sleep(120);
    }
    while (keepRunning);
  
    clearScreen();
    exit(0);  
}
