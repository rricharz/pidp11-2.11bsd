program Hilbert(input,output);
  {plot Hilbert curves of orders 1 to n}
  {adapted from N. Wirth,}
  { Algorithms+Data Structures=Programs,p132 76/11/18.  JFM.}
  {adapted to P8 using devput, 79/07/10  JFM }
  {adapted to P8RTS using TEK:, 81-12-06 JTE }
  {adapted to Berkeley Pascal, 2019/12/30 RR }

  {run this program with "pix hilbert.p      }

  const
    {place in center of tek 4015 - 1 scope}
    xoffset = 256 ;
    yoffset = 128;
    h0 = 512	{size factor - -  power of 2};
    {character codes for plotting}
    sp = 32	{ascii space};
    gs = 29	{ascii gs};
    bq = 96	{ascii back - quote};
    at = 64	{ascii at - sign};
    esc= 27	{ascii escape};
    ff = 12	{ascii form feed};
    cr = 13	{ascii carriage return};

  var
    i, h, x, y, x0, y0, n : integer;
    erase : boolean;

  procedure startplot {clear the screen};
  begin write(chr(esc),chr(ff)) end;

  procedure plot {plot a vector};
  begin
    write(chr(sp + (y + yoffset) div 32),
      chr(bq + (y + yoffset) mod 32),
      chr(sp + (x + xoffset) div 32),
      chr(at + (x + xoffset) mod 32))
  end {plot};

  procedure move;
  {move the pen to (x,y) - -  dark vector}
  begin write(chr(gs)); plot end{move};

  procedure endplot {leave graph mode};
  begin write(chr(cr)) end {endplot};

  procedure b(i : integer); forward;
  procedure c(i : integer); forward;
  procedure d(i : integer); forward;

  procedure a(i : integer);
  begin
    if i > 0 then
      begin
	d(i - 1); x := x - h; plot;
	a(i - 1); y := y - h; plot;
	a(i - 1); x := x + h; plot;
	b(i - 1)
      end
  end {a};

  procedure b;
  begin
    if i > 0 then
      begin
	c(i - 1); y := y + h; plot;
	b(i - 1); x := x + h; plot;
	b(i - 1); y := y - h; plot;
	a(i - 1)
      end
  end {b};

  procedure c;
  begin
    if i > 0 then
      begin
	b(i - 1); x := x + h; plot;
	c(i - 1); y := y + h; plot;
	c(i - 1); x := x - h; plot;
	d(i - 1)
      end
  end {c};

  procedure d;
  begin
    if i > 0 then
      begin
	a(i - 1); y := y - h; plot;
	d(i - 1); x := x - h; plot;
	d(i - 1); y := y + h; plot;
	c(i - 1)
      end
  end {d};

begin {Hilbert}
  write(output,'enter n of levels: ');
  read(input,n);
  erase := n<0;
  if erase then n :=  - n;
  startplot;
  i := 0;   h := h0;
  x0 :=  h div 2; y0 := x0;
  if erase then
    while i < n - 1 do
      begin
	i := i + 1;
	h := h div 2;
	x0 := x0 + (h div 2);
	y0 := y0 + (h div 2)
      end;
  repeat  
    {plot Hilbert curve of order i + 1}
    i := i + 1;
    h := h div 2;
    x0 := x0 + (h div 2);
    y0 := y0 + (h div 2);
    x := x0;   y := y0;   move;
    a(i)
  until i = n;
  endplot
end {Hilbert}.
