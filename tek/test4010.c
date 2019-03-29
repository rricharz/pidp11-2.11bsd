/*
  test4010.c - test plotlib for the 4010
  
  on PiPD11 using 2.11BSD Unix

  compile with
    cc -o test4010 test4010.c -l4013 -lm

  2019  rricharz
*/

#include <stdio.h>
#include <stdlib.h>

#define XMAX 1024		/* screen size as per Tektronix 4010 manual */
#define YMAX  768

int main (argc,argv)
int argc;
char *argv[];
{
      openpl();
      erase();
      move(XMAX/2,YMAX-40); label("Plotlib test using 4010 plotlib\n");
      circle(XMAX/2,YMAX/2,YMAX/3);
      move(150,100);
      cont(XMAX-150,100);
      cont(XMAX-150,YMAX-100);
      cont(150,YMAX-100);
      cont(150,100);
      move(0,YMAX-40);
      closepl();          
}
