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


#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define ROW  24		   // These are the PARITY HIGH and PARITY low leds.
#define COL1 7
#define COL2 6

int serverWasRunning = 1;   // makes sure that cpio pins are reset for first usage

int blinkForOneSecond(int usage)
// blink for one second, rate based on usage
{
    int n = (usage / 10) + 1;
    int d = 500 / n;
    for (int i = 0; i < n; i++) {
        digitalWrite(COL1, 1);
        digitalWrite(COL2, 0);
        delay(d);
        digitalWrite(COL1, 0);
        digitalWrite(COL2, 1);
        delay(d);
    }
}

void resetPiDP11Gpios()
// set all gpio pins used by PiDP11 to input
{
    for (int i = 4; i<=27; i++)
	if (i != 19) pinMode(i, INPUT);   // gpio 19 is not used
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
    double usage;
        
    fd = open("/proc/stat", O_RDONLY);

    wiringPiSetupGpio();    // initialize, use gpio numbering scheme

    do {
	if (system("pidof -x pidp11.sh >/dev/null") != 0) {

	    // server is currently not running

	    if (serverWasRunning) {
		resetPiDP11Gpios();
                pinMode(ROW, OUTPUT);
                pinMode(COL1, OUTPUT);
                pinMode(COL2, OUTPUT);
                digitalWrite(ROW,1);
		serverWasRunning = 0;
	    }

	    size = read(fd, buf, sizeof(buf));
	    if(size > 0) {

		nums = parser_result(buf, size);

		idle=nums[3];

		for(i=0, total=0; i<10; i++) total += nums[i];

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
	    delay(1000);
	}
    }
    while (1); // this is a daemon, loop until killed
}
