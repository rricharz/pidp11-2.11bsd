/*
  testPlotlib.c 
  
  on PiPD11 using 2.11BSD Unix

  test of plotlib

  compile with
    cc -o testPlotlib testPlotlib.c -l4013 -lm

  2019  rricharz
*/

#include <stdio.h>
#include <stdlib.h>

int main (argc,argv)
int argc;
char *argv[];
{
      openpl();
      erase();
      move(170,700); label("plotlib test\n");
      circle(500,400,300);
      move(150,50);
      cont(850,50);
      cont(850,750);
      cont(150,750);
      cont(150,50);     
      move(0,25); /* set cursor */
      closepl();          
}
