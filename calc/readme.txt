calc.c is a small calculator and base converter. Input can be decimal
(signed or unsigned, with or without decimal point), hex, octal or binary.
Operators are +,-,*,/,().&,|, <<,>>
Output is double, hex, octal and binary.

It is written in vintage Berkeley C on 2.11BSD Unix to demonstrate this historical
16-bit Unix system on a PiDP-11 using simh. The syntax of the C language differs
slightly from modern C. Function parameter declarations are probably the most
important difference.

Compile this program with

  make

Install this program in your ~/bin folder:

  If you do not yet have a ~/bin folder, make it with

    mkdir ~/bin

  Then install octal

    make install

  Is should be accessible after the next login