/*
  weather.c 
  a cool-retro weather station
  on PiPD11 using 2.11BSD Unix
  2019  rricharz
*/

#include <stdio.h>
#include <stdlib.h>

#define MAXL 80  /* maximum line length */

FILE *weatherData;
     
int main (argc,argv)
int argc;
char *argv[];
{
    char line[MAXL+1], w1[MAXL+1], w2[MAXL+1];
    char ch;
    int i;
    double v,T,P,H;

    T=-273.0;
    P=-1.0;
    H=-1.0;

    printf("Contacting remote sensor...\n");
    weatherData = popen(
        "rsh pizerow -l pi -n 'python ~/bin/bme280.py'", "r");
    if (weatherData != NULL) {
       printf("Data received, analyzing ...\n");
       i=0;
       while (((ch=fgetc(weatherData))!=EOF) && (i<MAXL)){
           line[i++]=ch;
           if (ch=='\n') {
              line[i-1]=0;
              sscanf(line,"%s %s %f",w1,w2,&v);
              /* printf("%s %f\n",w1,v); */
              if (strcmp(w1,"Temperature")==0)
                 T=v;
              if (strcmp(w1,"Pressure")==0)
                 P=v;
              if (strcmp(w1,"Humidity")==0)
                 H=v;
              i=0;
           }
       }
       pclose (weatherData);

       printf("**************************************\n");
       printf("The sensor reports the following data:\n");
       if (T>-273.0)
           printf(" Temperature  %0.1f C\n",T);
       if (P>=0.0)
           printf(" Pressure  %0.1f hPa\n",P);
       if (H>=00)
           printf(" Humidity     %0.1f %%\n",H);
       printf("**************************************\n");
       exit(0);  
    }
    else {
       printf("Cannot obtain data from remote sensor\n");
       exit(1);
    }             
}
