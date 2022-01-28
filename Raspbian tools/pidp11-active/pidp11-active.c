//
// pidp11-active.c
// Version for PiDP-11
//
// rricharz 2019
//
// This daemon toggles the PiDP PARITY HIGH and PARITY LOW LEDs
// if pidp11.sh is not running
//
// This happens early during the bootup process
// and if the user uses "exit" to stop simh
//
// The speed of the LED blinking shows current CPU usage
//
// The program also controls a FAN connected with a transistor to Pin 35.
// If the CPU temperature is above HIGH_TEMP, the fan is turned on
// If the CPU temperature is below LOW_TEMP, the fan is turned off
// Set CONTROL_FAN if you do not want to control an attached fan
//
// One word of caution: Nothing guarantees that this will still work with
// new releases of the PiDP-11 software! Use at your own risk!
//
// cpu usage code from Matheus (https://github.com/hc0d3r)
//
// To install:
//     chmod +x install
//     ./install
//
// To uninstall
//     chmod +x uninstall
//     ./uninstall
//


#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define CONTROL_FAN 1		// set to 0 if no fan control is required
#define HIGH_TEMP 60.0		// turn fan on above this temperature (in °C)
#define LOW_TEMP  50.0		// turn fan off below this temperature (in °C)

#define ROW  24			// These are the PARITY HIGH and PARITY low leds.
#define COL1 7
#define COL2 6
#define FAN  19			// spare pin on PiDP11, used for fan control, if CONTROL_FAN set

int serverWasRunning = 1;	// makes sure that gpio pins are reset for first usage

int blinkForOneSecond(int usage)
// blink for one second, rate based on usage
{
    int n = (usage / 10) + 1;
    int d = 500 / n;
    for (int i = 0; i < n; i++) {
        gpioWrite(COL1, 1);
        gpioWrite(COL2, 0);
        usleep(d*1000);
        gpioWrite(COL1, 0);
        gpioWrite(COL2, 1);
        usleep(d*1000);
    }
}

void resetPiDP11Gpios()
// set all gpio pins used by PiDP11 to input
{
    for (int i = 4; i<=27; i++)
	if (i != FAN) gpioSetMode(i, PI_INPUT);   // gpio 19 is not used in PiDP11
}

int *parser_result(const char *buf, int size) {
// parse /proc/stat and return cpu usage
    static int ret[10];
    int i, j = 0, start = 0;

    for(i=0; i<size; i++) {
	char c = buf[i];
	if(c >= '0' && c <= '9') {
	    if(!start){
		start = 1;
		ret[j] = c-'0';
	    }
	    else {
		ret[j] *= 10;
		ret[j] += c-'0';
	    }
	}
	else if (c == '\n') {
	    break;
	}
	    else
	{
	    if(start) {
		j++;
		start = 0;
	    }
	}
    }
    return ret;
}


int main (int argc, char **argv)
{
    char buf[256];
    int size, fd, *nums;
    int prev_idle = 0;
    int prev_total = 0;
    int idle, total, i;
    double T, usage;
    int fanIsOn;
        
    fd = open("/proc/stat", O_RDONLY);

    gpioInitialise();
    
    if (CONTROL_FAN) {
	gpioSetMode(FAN, PI_OUTPUT);
	gpioWrite(FAN, 0);		// fan initially turned off
	fanIsOn = 0;
    }	

    do {
	if (system("pidof -x pidp11.sh >/dev/null") != 0) {

	    // server is currently not running

	    if (serverWasRunning) {
		resetPiDP11Gpios();
                gpioSetMode(ROW, PI_OUTPUT);
                gpioSetMode(COL1, PI_OUTPUT);
                gpioSetMode(COL2, PI_OUTPUT);
                gpioWrite(ROW,1);
		serverWasRunning = 0;
	    }

	    size = read(fd, buf, sizeof(buf));
	    if(size > 0) {

		nums = parser_result(buf, size);

		idle=nums[3];

		total = 0;
		for(i=0; i<10; i++) total += nums[i];

		int diff_idle = idle-prev_idle;
		int diff_total = total-prev_total;
		usage = (double)(((double)(1000*(diff_total-diff_idle))/(double)diff_total+5)/(double)10);

	    }
	    else {
		usage = 0.0;
	    }
                    
	    if (usage < 1.0) usage = 1.0; // avoid too long delays or underflow
	    if (usage > 99.0) usage = 99.0;

	    prev_total = total;
	    prev_idle = idle;

	    lseek(fd, 0, SEEK_SET);
                    
	    blinkForOneSecond((int)usage);               
	}
	else {
	    if (!serverWasRunning) resetPiDP11Gpios();
	    serverWasRunning = 1;
	    sleep(1);
	}
    
	if (CONTROL_FAN) {
	    FILE *temperatureFile = fopen(
                        "/sys/class/thermal/thermal_zone0/temp", "r");
	    if (temperatureFile != NULL) {
		fscanf(temperatureFile, "%lf", &T);
		if (T < 0.0) T = 0.0;
		if (T > 99999.0) T = 99999.0;
		T = T / 1000.0;
		fclose (temperatureFile);
		}
	    else T = 0.0;
	    if ((T > HIGH_TEMP) && (fanIsOn == 0)) {
		// printf("Fan on, T=%f\n", T);
		gpioWrite(FAN,1);
		fanIsOn = 1;
	    }
	    else if ((T < LOW_TEMP) && (fanIsOn != 0)) {
		// printf("Fan off, T=%f\n", T);
		gpioWrite(FAN,0);
		fanIsOn = 0;
	    }
	    
	}
	
    }
    while (1); // this is a daemon, loop until killed
}
