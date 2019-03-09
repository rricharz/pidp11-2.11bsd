/*
  weather2.c 
  a cool-retro weather station
  on PiPD11 using 2.11BSD Unix
  2019  rricharz

  This version displays new data every 30 seconds
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>

#define MAXL    80  /* maximum line length of input file */

#define	COLUMNS	80
#define	ROWS	10

char matrix[ROWS][COLUMNS];    /* holds the characters to be displayed */

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
                     stamp[5]="     ";
                     stamp[6]="  *  ";; break;
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
           case 'C': stamp[0]=" ___ ";
                     stamp[1]="/   \\";
                     stamp[2]="|    ";
                     stamp[3]="|    ";
                     stamp[4]="|    ";
                     stamp[5]="|    ";
                     stamp[6]="\\___/"; break;
           case 'd': stamp[0]="   _ ";
                     stamp[1]="  |_|";
                     stamp[2]="     ";
                     stamp[3]="     ";
                     stamp[4]="     ";
                     stamp[5]="     ";
                     stamp[6]="     "; break;
           default:  stamp[0]="     ";
                     stamp[1]="     ";
                     stamp[2]="     ";
                     stamp[3]="     ";
                     stamp[4]="     ";
                     stamp[5]="     ";
                     stamp[6]="     "; break; 
   }

       for (i=0; i<7; i++)
           for (j=0; j<5; j++)
               matrix[row+i][column+j]=stamp[i][j];
}

void stampString(s)
char *s;
/* put a string into the matrix */
{
   int xstart;
   int i=0;
   if (strlen(s)<=12) {
     xstart=COLUMNS/2-strlen(s)*3;
     while (s[i]!=0)
       stampChar(s[i++],xstart+6*i,1);
   }
}

void showStamps()
{
    int i;
    for (i=0; i<ROWS; i++) {
      matrix[i][COLUMNS-1]=0;  /* terminate each string */
      printf("%s\n",matrix[i]);
    }   
}

void initStamps(s)
char *s;
{
    int i,pos;
    memset(matrix,' ',sizeof(matrix));
    for (i=0; i<COLUMNS-1; i++) {
        matrix[0][i]='=';
        matrix[ROWS-1][i]='=';
    }
    pos=(COLUMNS-strlen(s))/2;
    for (i=0; i<strlen(s); i++)
        matrix[0][pos+i]=s[i];
    for (i=1; i<ROWS-1; i++) {
        matrix[i][0]='|';
        matrix[i][COLUMNS-2]='|';
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

    do {
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

           time(&rawtime);
           timeinfo=localtime(&rawtime);
           strftime(ctime,MAXL,"%H:%M",timeinfo);
           strftime(cdate,MAXL,"%Y%m%d",timeinfo);

           printf("The sensor reports the following data at %s:\n",ctime);
           if (T>-273.0)
               printf(" Temperature  %0.1f C\n",T);
           if (P>=0.0)
               printf(" Pressure     %0.1f hPa\n",P);
           if (H>=00)
               printf(" Humidity     %0.0f %%\n",H);

           if ((T>-273.0) && (H>=0.0)) {

               strcat(strcpy(w1,getenv("HOME")),"/weather/data/");
               strcat(w1,cdate);
               printf("Storing data in %s\n",w1);
               storage=fopen(w1,"a");
               if (storage<0)
                   printf("Cannot open data file\n");
               else {
                   fprintf(storage,"%s %0.1f %0.1f %0.1f\n",ctime,T,P,H);
                   fclose(storage);
               }

               initStamps("cool-retro-weatherstation");
               sprintf(w1,"%0.1fdC %d%%",T,(int)H);
               stampString(w1);
               showStamps();

           }
           else
               printf("Data received does not contain weather info\n"); 
        }
        else {
           printf("Cannot obtain data from remote sensor\n");
        }
        printf("Sleeping 60 seconds, type <ctrl>c to abort\n");
        for (i=0; i<60; i++) {
            putchar('.');
            fflush(stdout);
            sleep(1);
        }
        printf("\n");

    }
    while (1);           
}
