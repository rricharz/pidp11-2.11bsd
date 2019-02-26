calc.c is a small calculator and base converter. Input can be decimal
(signed or unsigned, with or without decimal point), hex, octal or binary.
Operators are +,-,*,/,().&,|, <<,>>
Output is double, hex, octal and binary.

It is written in vintage Berkeley C on 2.11BSD Unix to demonstrate this historical
16-bit Unix system on a PiDP-11 using simh. The syntax of the C language differs
slightly from modern C. Function parameter declarations are probably the most
important difference.

Example output of calc:

rene@pdp11:calc$ calc
CALC for 2.11BSD Unix on PiDP11
Enter an expression, examples are:
32767     input decimal number
$fff      input hex number
o2750     input octal number
%11001   input binary number
-55.35    input negative number
2*(5+28)
<return>  exit
operators: +,-,*,/,(),&,|,<<,>>
double: 0.000000   long: $00000000 o00000000000   int: $0000 o000000 %0
? 32767
double: 32767.000000   long: $00007fff o00000077777   int: $7fff o077777 %111111111111111
? $ffff&27
double: 27.000000   long: $0000001b o00000000033   int: $001b o000033 %11011
? %1101|%10000
double: 29.000000   long: $0000001d o00000000035   int: $001d o000035 %11101
? o275+5
double: 194.000000   long: $000000c2 o00000000302   int: $00c2 o000302 %11000010
? 2*(5+23)
double: 56.000000   long: $00000038 o00000000070   int: $0038 o000070 %111000
? 
rene@pdp11:calc$ 


Compile this program with

  make

Install this program in your ~/bin folder:

  If you do not yet have a ~/bin folder, make it with

    mkdir ~/bin

  Then install octal

    make install

  Is should be accessible after the next login