/*
  weather3.c 
  a cool-retro weather station
  on PiPD11 using 2.11BSD Unix
  2019  rricharz

  This version uses curses
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include <signal.h>

#define MAXL    80  /* maximum line length of input file */

#define	COLUMNS	80
#define	ROWS	23

static int keepRunning;

void intHandler(dummy)    /* catch <ctrl>c */
{
   keepRunning=0;
}

void stampChar(ch,column,row)
char ch;
int column,row;
/* put a char into the matrix */
{
   int i,j;
   char *stamp[7];
   switch (ch) {
           case '0': stamp[0]=" ___ ";
                     stamp[1]="/   \\";
                     stamp[2]="|   |";
                     stamp[3]="|   |";
                     stamp[4]="|   |";
                     stamp[5]="|   |";
                     stamp[6]="\\___/"; break;
           case '1': stamp[0]="     ";
                     stamp[1]="  |  ";
                     stamp[2]="  |  ";
                     stamp[3]="  |  ";
                     stamp[4]="  |  ";
                     stamp[5]="  |  ";
                     stamp[6]="  |  "; break;
           case '2': stamp[0]=" ___ ";
                     stamp[1]="/   \\";
                     stamp[2]="    / ";
                     stamp[3]="   / ";
                     stamp[4]="  /  ";
                     stamp[5]=" /   ";
                     stamp[6]="/____"; break;
           case '3': stamp[0]=" ___ ";
                     stamp[1]="/   \\";
                     stamp[2]="    |";
                     stamp[3]=" ___|";
                     stamp[4]="    |";
                     stamp[5]="    |";
                     stamp[6]="\\___/"; break;
           case '4': stamp[0]="      ";
                     stamp[1]="|   |";
                     stamp[2]="|   |";
                     stamp[3]="|___|";
                     stamp[4]="    |";
                     stamp[5]="    |";
                     stamp[6]="    |"; break;
           case '5': stamp[0]="_____";
                     stamp[1]="|    ";
                     stamp[2]="|    ";
                     stamp[3]="|___ ";
                     stamp[4]="    \\";
                     stamp[5]="    |";
                     stamp[6]="\\___/"; break;
           case '6': stamp[0]=" ____";
                     stamp[1]="/    ";
                     stamp[2]="|    ";
                     stamp[3]="|___ ";
                     stamp[4]="|   \\";
                     stamp[5]="|   |";
                     stamp[6]="\\___/"; break;
           case '7': stamp[0]="_____";
                     stamp[1]="    |";
                     stamp[2]="    /";
                     stamp[3]="   / ";
                     stamp[4]="  /  ";
                     stamp[5]=" /   ";
                     stamp[6]="/    "; break;
           case '8': stamp[0]=" ___ ";
                     stamp[1]="/   \\";
                     stamp[2]="|   |";
                     stamp[3]="\\___/";
                     stamp[4]="/   \\";
                     stamp[5]="|   |";
                     stamp[6]="\\___/"; break;
           case '9': stamp[0]=" ___ ";
                     stamp[1]="/   \\";
                     stamp[2]="|   |";
                     stamp[3]="\\___|";
                     stamp[4]="    |";
                     stamp[5]="    |";
                     stamp[6]="\\___/"; break;
           case '.': stamp[0]="     ";
                     stamp[1]="     ";
                     stamp[2]="     ";
                     stamp[3]="     ";
                     stamp[4]="     ";
                     stamp[5]="  _  ";
                     stamp[6]=" |_| "; break;
           case '-': stamp[0]="     ";
                     stamp[1]="     ";
                     stamp[2]="     ";
                     stamp[3]=" ___ ";
                     stamp[4]="     ";
                     stamp[5]="     ";
                     stamp[6]="     ";; break;
           case '%': stamp[0]=" _   ";
                     stamp[1]="|_| /";
                     stamp[2]="   / ";
                     stamp[3]="  /  ";
                     stamp[4]=" /   ";
                     stamp[5]="/  _ ";
                     stamp[6]="  |_|";; break;
           case '@': stamp[0]="   _ ";
                     stamp[1]="  |_|";
                     stamp[2]="     ";
                     stamp[3]="     ";
                     stamp[4]="     ";
                     stamp[5]="     ";
                     stamp[6]="     "; break;
           case ':': stamp[0]="     ";
                     stamp[1]="     ";
                     stamp[2]="  _  ";
                     stamp[3]=" |_| ";
		     stamp[4]="     ";
                     stamp[5]="  _  ";
                     stamp[6]=" |_| "; break;
           case 'C': stamp[0]=" ___ ";
                     stamp[1]="/   \\";
                     stamp[2]="|    ";
                     stamp[3]="|    ";
                     stamp[4]="|    ";
                     stamp[5]="|    ";
                     stamp[6]="\\___/"; break;
           case 'P': stamp[0]="____ ";
                     stamp[1]="|   \\";
                     stamp[2]="|   |";
                     stamp[3]="|__/ ";
                     stamp[4]="|    ";
                     stamp[5]="|    ";
                     stamp[6]="|    "; break;
           case 'a': stamp[0]="     ";
                     stamp[1]="     ";
                     stamp[2]=" ___ ";
                     stamp[3]="/   \\";
                     stamp[4]=" ___|";
                     stamp[5]="|   |";
                     stamp[6]="\\___|"; break;
           case 'h': stamp[0]="     ";
                     stamp[1]="|    ";
                     stamp[2]="| __ ";
                     stamp[3]="|/  \\";
                     stamp[4]="|   |";
                     stamp[5]="|   |";
                     stamp[6]="|   |"; break;
           default:  stamp[0]="     ";
                     stamp[1]="     ";
                     stamp[2]="     ";
                     stamp[3]="     ";
                     stamp[4]="     ";
                     stamp[5]="     ";
                     stamp[6]="     "; break; 
       }

       for (i=0; i<7; i++) {
           move(row+i,column);
           addstr(stamp[i]);
       }
}

void stampString(s,y)
char *s;
int y;
/* put a string into the matrix */
{
   int i=0;
   int pos=(COLUMNS-(strlen(s))*6)/2;
   if (strlen(s)<=12) {
     while (s[i]!=0)
       stampChar(s[i++],pos+6*i,y);
   }
}

void myBox(s)
char *s;
{
    int i,pos;
    for (i=0; i<COLUMNS; i++) {
        move(0,i); addch('-');
        move(ROWS-1,i); addch('-');
    }
    pos=(COLUMNS-strlen(s))/2;
    move(0,pos); addstr(s);
    for (i=1; i<ROWS-1; i++) {
        move(i,0); addch('|');
        move(i,COLUMNS-1); addch('|');
    }
}
     
int main (argc,argv)
int argc;
char *argv[];
{
    FILE *weatherData,*storage;
    char line[MAXL+1], w1[MAXL+1], w2[MAXL+1];
    char cdate[MAXL+1], ctime[MAXL+1];
    char ch;
    int i;
    double v,T,P,H;
    time_t rawtime;
    struct tm *timeinfo;

    keepRunning = 1;
    signal(SIGINT,intHandler);  /* catch <ctrl>c */

    initscr();

    do {
        T=-273.0;
        P=-1.0;
        H=-1.0;

        clear();
        myBox("cool-retro-weatherstation");

        weatherData = popen(
            "rsh pizerow -l pi -n 'python ~/bin/bme280.py'", "r");
        if (weatherData != NULL) {
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

           time(&rawtime);
           timeinfo=localtime(&rawtime);
           strftime(ctime,MAXL,"%H:%M",timeinfo);
           strftime(cdate,MAXL,"%Y%m%d",timeinfo);

           if ((T>-273.0) && (H>=0.0) && (H>=0.0)) {

               strcat(strcpy(w1,getenv("HOME")),"/weather/data/");
               strcat(w1,cdate);
               storage=fopen(w1,"a");
               if (storage) {
                   fprintf(storage,"%s %0.1f %0.1f %0.1f\n",ctime,T,P,H);
                   fclose(storage);
               }
               else {
                   move(ROWS-2,2); addstr("Cannot store data!");
               }
               sprintf(w1,"%0.1f@C %0.0f%%",T,H);
               stampString(w1,2);
	       sprintf(w1,"%0.0fhPa %s%",P,ctime);
               stampString(w1,12);
               move(ROWS,0);
           }
           else {
             move(ROWS-2,2); addstr("No weather info in data received!");
	   }
        }
	else {
             move(ROWS-2,2); addstr("No data obtained from remote sensor!");
	}
        move(ROWS,0); addstr("Type <ctrl>c to exit ");
        refresh();
        sleep(60);
    }
    while (keepRunning);
    endwin();
    printf("\n");         
}
