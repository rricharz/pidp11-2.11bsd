Octal is a small base converter. Input can be decimal (signed or unsigned),
octal or binary. Output is 16-bit signed word,16-bit unsigned word, up to
22 bit (for the PDP-11/70 address space) octal and up to 22 bit binary.

Example input:

  octal 555
  octal -31768
  octal o3777
  octal b101101a

Example output:
  rene@pdp11:octal$ octal 31767
  16-bit integer:
    signed decimal:   31767
    unsigned decimal: 31767
  octal:   0   0   0   7   6   0   2   7
  binary:  0 000 000 111 110 000 010 111
  rene@pdp11:octal$

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