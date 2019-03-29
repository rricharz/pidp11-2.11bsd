/*
  test4014.c - test plotlib for the 4014
  
  on PiPD11 using 2.11BSD Unix

  compile with
    cc -o test4014 test4014.c -l4014 -lm

  2019  rricharz
*/

#include <stdio.h>
#include <stdlib.h>

#define XMAX 4096	/* screen size as per Tektronix 4014 manual */
#define YMAX 3072

int main (argc,argv)
int argc;
char *argv[];
{
      openpl();
      erase();
      move(XMAX/2,YMAX-40); label("Plotlib test using 4014 plotlib\n");
      circle(XMAX/2,YMAX/2,YMAX/3);
      move(150,100);
      cont(XMAX-150,100);
      cont(XMAX-150,YMAX-100);
      cont(150,YMAX-100);
      cont(150,100);
      move(0,YMAX-40);
      closepl();          
}
