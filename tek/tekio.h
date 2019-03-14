/*  tecio.h 2.11 BSD Tektronix graphics 	*/
/*  minimal io functions for Tektronix 4010     */
/*  rricharz 2019 for 2.11BSD Unix on PiDP-11	*/
/*  Uses Berkley C syntax!			*/
/*  If you want to use this, you need to	*/
/*  login from a Tektronix terminal or emulator	*/

#define MAXX 1023	/* Tektronix graphics screen size */
#define MAXY  780
#define MAXCH  74       /* Tektronix alpha screen size */

extern void moveTo();
extern void drawVector();
extern void clearScreen();
extern void endScreen();
