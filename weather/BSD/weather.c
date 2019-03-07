/*
  weather.c 
  a cool-retro weather station
  on PiPD11 using 2.11BSD Unix
  2019  rricharz
*/

#include <stdio.h>
#include <stdlib.h>

FILE *weatherData;
     
int main (argc,argv)
int argc;
char *argv[];
{
    double T;
	
    weatherData = popen(
        "rsh pizerow -l pi -n 'python ~/bin/bme280.py'", "r");
    if (weatherData != NULL) {
       fscanf(weatherData, "Temperature : %lf", &T);
       pclose (temperatureFile);

       printf("*****************************************\n");
       printf("* The current temperature is %0.1f deg C *\n",T);
       printf("*****************************************\n");
       exit(0);  
    }
    else {
       printf("Cannot obtain data from remote sensor\n");
       exit(1);
    }             
}
