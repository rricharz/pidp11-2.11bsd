/*
  rtest.c 
  test remote execution of commands with rsh
  on PiPD11 using 2.11BSD Unix
  2019  rricharz
*/

#include <stdio.h>
#include <stdlib.h>

FILE *temperatureFile;
     
int main (argc,argv)
int argc;
char *argv[];
{
    double T;
	
    temperatureFile = popen(
        "rsh pizerow -l pi -n 'cat /sys/class/thermal/thermal_zone0/temp'", "r");
    if (temperatureFile != NULL) {
       fscanf(temperatureFile, "%lf", &T);
       if (T < 0.0) T = 0.0;
       if (T > 99999.0) T = 99999.0;
       T = T / 1000.0;
       pclose (temperatureFile);
    }
    else T = 0.0;
    
    printf("****************************************************\n");
    printf("* The CPU temperature on the pizerow is %0.1f deg C *\n",T);
    printf("****************************************************\n");                   
}
