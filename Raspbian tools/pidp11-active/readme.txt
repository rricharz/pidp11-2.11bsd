//
// pidp11-active
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
// Note: This program was moved from using wiringPi to using pigpio
//       because wiringPi is deprecated. It must be run with
//       superuser priviledges, e.g.
//       sudo ./pidp11-active