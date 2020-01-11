{ starship - A game for the R65 computer using
  the tek4010 Tektronix 4010 emulator

  www.github.com/rricharz/Tek4010

  This is a revival of my original starship
  game with added high resolution graphics

  rewritten rricharz 2019 for the R65 system and       
  adapted 2020 for 2.11 BSD on the PiDP-11    }

program starship(input,output);

const
    maxships = 8;
    maxx = 1023; { Tektronix 4010 graphics }
    maxy = 780;

    solid        = 1;
    { dotted     = 2;
      dotdash    = 3;
      shortdash  = 4; }
    longdash     = 5;

    cpi = 0.017453278;

    xoffset = 341;   { offset of graphics }
    yoffset = 195;

var sinetable: array[0..90] of integer;
    dshiptable: array[0..maxships] of integer;
    ashiptable: array[0..maxships] of integer;
    shipdamage: array[0..maxships] of integer;
    energy,damage: array[0..4] of integer;
    radarsize,charging,score: integer;
    shield,quit: boolean;
    phaserrange,step: integer;
    xs,ys: integer;
    ch: char;

function irand(d: integer): integer;
begin
  irand := trunc(random(1) * d);
end;

procedure clearscreen;
begin
  write(chr(27),chr(12));
end;

procedure startdraw(x1,y1:integer);
var x,y: integer;
begin
  x:=2 * x1 div 3 + xoffset;
  y:=2 * y1 div 3 + yoffset;
  if x<0 then x:=0;
  if x>=maxx then x:=maxx-1;
  if y<0 then y:=0;
  if y>=maxy then y:=maxy;
  write(chr(29));
  write(chr((y div 32)+32));
  write(chr((y mod 32)+96));
  write(chr((x div 32)+32));
  write(chr((x mod 32)+64));
  xs:=x;
  ys:=y;
end;

procedure movetoline(l: integer);
var x, y: integer;
begin
  x := 0;
  y := 780 - 13 * l;
  if x<0 then x:=0;
  if x>=maxx then x:=maxx-1;
  if y<0 then y:=0;
  if y>=maxy then y:=maxy;
  write(chr(29));
  write(chr((y div 32)+32));
  write(chr((y mod 32)+96));
  write(chr((x div 32)+32));
  write(chr((x mod 32)+64));
  xs:=x;
  ys:=y;
  write(chr(31));
end;

procedure draw(x2,y2:integer);
var x,y: integer;
    hxchange,lychange:boolean;
begin
  x:=2 * x2 div 3 + xoffset;
  y:=2 * y2 div 3 + yoffset;
  if x<0 then x:=0;
  if x>=maxx then x:=maxx-1;
  if y<0 then y:=0;
  if y>=maxy then y:=maxy;
  if (y div 32) <> (ys div 5) then
    write(chr((y div 32)+32));
  hxchange:=(x div 32) <> (xs div 32);
  lychange:=(y mod 32) <> (ys mod 32);
  if hxchange or lychange then
    write(chr((y mod 32)+96));
  if hxchange then
    write(chr((x div 32)+32));
  write(chr((x mod 32)+64));
  xs:=x;
  ys:=y;
end;

procedure enddraw;
begin
  write(chr(31));
end;

procedure drawvector(x1,y1,x2,y2:integer);
begin
  startdraw(x1,y1);
  draw(x2,y2);
  enddraw;
end;

procedure drawrectange(x1,y1,x2,y2:integer);
begin
  startdraw(x1,y1);
  draw(x2,y1);
  draw(x2,y2);
  draw(x1,y2);
  draw(x1,y1);
  enddraw;
end;

procedure moveto(x1,y1: integer);
{ move in graphics coordinate space }
begin
  startdraw(x1,y1);
  enddraw;
end;

procedure setchsize(size:integer);
{ set character size }
begin
  if (size>=1)and(size <= 4) then begin
  write(chr(27));
  write(chr(ord('7') + size));
  end;
end;

procedure setlinemode(ltype:integer);
begin
  if (ltype>=solid) and (ltype<=longdash) then begin
    write(chr(27));
    write(chr(95+ltype));
  end;
end;

procedure startwritethrough;
begin
  write(chr(27),'p');
end;

procedure endwritethrough;
begin
  write(chr(27),'`');
end;

function atan2(x,y:integer):real;
{ this is a special fast atan function
for the starship coordinate system }

  function atanx(z:real): real;
  const n1= 0.972394;
        n2=-0.191948;
        n3= 57.29578;
  begin
    atanx:=(n1+n2*z*z)*z*n3;
  end;

  function atanp(x,y:integer):real;
  begin
    if (y<>0) and (x<>0) then begin
      if x<y then
        atanp:=atanx(x/y)
      else
        atanp:=90.0-atanx(y/x)
    end else begin
      if y>0 then atanp:=0.0
      else if x>0 then atanp:=90.0
      else atanp:=0.0;
    end;
  end;

begin
  if (x>=0) and (y>=0) then
    atan2:=atanp(x,y)
  else if (x>=0) and (y<0) then
    atan2:=180.0-atanp(x,-y)
  else if (x<0) and (y<0) then
    atan2:=atanp(-x,-y)+180.0
  else
    atan2:=360.0-atanp(-x,y)
end;

procedure checkradar(angle,cx,cy,mode:integer);
var i,xi,yi,dist,rsize0,a:integer;
    x,y:real;
begin
  rsize0:=radarsize;
  if mode=0 then rsize0:=rsize0 div 10;
  for i:=0 to maxships-1 do
    if (ashiptable[i]>=angle) and
      (ashiptable[i]<angle+step) then begin
      if dshiptable[i]<=rsize0 then begin
        dist:=dshiptable[i];
        a:=ashiptable[i];
        if angle<90 then begin
          x:=sinetable[a]* dist / 32000.0;
          y:=sinetable[89-a] * dist / 32000.0
        end else if angle<180 then begin
          x:=sinetable[179-a] * dist / 32000.0;
          y:=-sinetable[a-90] * dist / 32000.0
        end else if angle<270 then begin
          x:=-sinetable[a-180] * dist / 32000.0;
          y:=-sinetable[269-a] * dist / 32000.0
        end else begin
          x:=-sinetable[359-a] * dist / 32000.0;
          y:=sinetable[a-270] * dist / 32000.0
        end;
        if mode=0 then begin
          x:=x * 10.0;
          y:=y * 10.0;
        end;
        xi:=cx+trunc(x);
        yi:=cy+trunc(y);
        startdraw(xi-3,yi-3);
        draw(xi+3,yi-3);
        draw(xi,yi+2);
        draw(xi-3,yi-3);
        enddraw;
        moveto(xi+6,yi-7);
        write(i+1:1);
        if shipdamage[i]<100 then
          write('d');
        if mode=1 then begin
          moveto(20,maxy-16*i-78);
          write(i+1:1,': ', angle:3, dist * 0.1:7:1,' pc');
          if shipdamage[i]<100 then
            write(' d');
        end;
      end;
    end;
end;

procedure scan(mode:integer);
var i,angle: integer;
    cx,cy: integer;
    x,y,csize,ticf1: real;

  procedure drawsegment;
  begin
    startwritethrough;
    drawvector(cx,cy,cx+trunc(x),cy+trunc(y));
    endwritethrough;
    drawvector(cx+trunc(x),
      cy+trunc(y),
      cx+trunc(x),
      cy+trunc(y));
    if mode=0 then begin
      drawvector(cx+trunc(ticf1*x),
      cy+trunc(ticf1*y),
      cx+trunc(ticf1*x),
      cy+trunc(ticf1*y));
    end;
    angle:=angle+step;
  end;

begin
  if mode=0 then begin
    step:=6;
    cx:=maxx-130;
    cy:=150;
    moveto((maxx div 2)-72,maxy-25);
    radarsize:=100;
    ticf1:=phaserrange / 10.0;
    csize:= radarsize / 32000.0;
    moveto(cx-130,cy-radarsize-28);
    write('Phaser range');
    write(phaserrange div 10:4);
    write(' pc');
  end else begin
    step:=3;
    cx:=maxx div 2;
    cy:=maxy div 2;
    moveto((maxx div 2)-72,maxy-25);
    write('R65 Starship');
    radarsize:=(310*damage[0]) div 100;
    csize:=radarsize/32000.0;
    moveto(cx-150,cy-radarsize-28);
    write('Long range scan');
    write(radarsize * 0.1:4:1);
    write(' pc');
  end;
  angle:=0;
  setlinemode(2);
  drawvector(cx-radarsize,cy,cx+radarsize,cy);
  drawvector(cx,cy-radarsize,cx,cy+radarsize);
  setlinemode(1);
  if (mode=1) then begin
    drawrectange(10,maxy-200,240,maxy-10);
    moveto(20,maxy-30);
    write('SCANNER');
    moveto(20,maxy-46);
    write('Objects located:');
    moveto(20,maxy-60);
    write('   angle distance');
  end;
  angle:=0;
  i:=0;
  repeat
    checkradar(angle,cx,cy,mode);
    x:=csize*sinetable[i];
    y:=csize*sinetable[89-i];
    drawsegment;
    i:=i+step;
  until i>=90;
  i:=0;
  repeat
    checkradar(angle,cx,cy,mode);
    x:=csize*sinetable[89-i];
    y:=-csize*sinetable[i];
    drawsegment;
    i:=i+step;
  until i>=90;
  i:=0;
  repeat
    checkradar(angle,cx,cy,mode);
    x:=-csize*sinetable[i];
    y:=-csize*sinetable[89-i];
    drawsegment;
    i:=i+step;
  until i>=90;
  i:=0;
  repeat
    checkradar(angle,cx,cy,mode);
    x:=-csize*sinetable[89-i];
    y:=csize*sinetable[i];
    drawsegment;
   i:=i+step;
  until i>=90;
end;

procedure showEnergy;
var i: integer;
begin
  setlinemode(1);
  drawrectange(10,10,240,116);
  moveto(20,90);
  write('ENERGY');
  for i:= 0 to 3 do begin
    moveto(20,74-16*i);
    case i of
      0: write('Scanner:    ');
      1: write('Shield:     ');
      2: write('Phaser:     ');
      3: write('Warp Engine:')
    end;
    write(energy[i]:4);
    write('%');
    if charging=i then
      write('^');
  end;
end;

procedure showStatus;
var i: integer;
begin
  setlinemode(1);
  drawrectange(maxx-240,maxy-116,
    maxx-10,maxy-10);
  moveto(maxx-230,maxy-36);
  write('STATUS/DAMAGE');
  for i:= 0 to 3 do begin
    moveto(maxx-230,maxy-52-16*i);
    case i of
      0: write('Scanner:    ');
      1: if shield then
           write('Shield: Up  ')
         else
           write('Shield: Down');
      2: write('Phaser:     ');
      3: write('Warp Engine:')
    end;
    write(damage[i]:4);
    write('%');
  end;
end;

procedure setcharging;
var select: integer;
begin
  writeln('Select station to charge:');
  writeln('1: Scanner');
  writeln('2: Shield');
  writeln('3: Phaser');
  writeln('4: Warp Engine');
  write('Which station (1..4)? ');
  readln(select);
  clearscreen;
  setchsize(3);
  if (select>=1) and (select<=4) then
    charging:=select-1
  else
    writeln('ILLEGAL SELECTION');
  write('Charging ');
  case charging of
    0: writeln('Scanner');
    1: writeln('Shield');
    2: writeln('Phaser');
    3: writeln('Warp Engine')
  end;
end;

procedure doscan;
begin
  if damage[0]<5 then
    writeln('SCANNER DEFECTIVE')
  else if energy[0]<8 then
    writeln('NOT ENOUGH ENERGY FOR SCAN')
  else begin
    energy[0]:=energy[0]-8;
    if energy[0]<0 then energy[0]:=0;
    scan(1);
    scan(0);
  end;
end;

procedure repair;
var select: integer;
begin
  writeln('Select station to repair:');
  writeln('1: Scanner');
  writeln('2: Shield');
  writeln('3: Phaser');
  writeln('4: Warp Engine');
  write('Which station (1..4)? ');
  readln(select);
  clearscreen;
  setchsize(3);
  if (select>=1) and (select<=4) then begin
    damage[select-1]:=damage[select-1]+20;
    if damage[select-1]>100 then
      damage[select-1]:=100;
    write('Repairing ');
    case select-1 of
      0: write('Scanner');
      1: write('Shield');
      2: write('Phaser');
      3: write('Warp Engine')
    end
  end else
    write('ILLEGAL SELECTION');
  writeln;
end;

procedure warp;
var i,angle,required:integer;
    distance,x,y,warpx,warpy:real;
    done:boolean;
begin
  if damage[3]<10 then
    writeln('WARP ENGINE DAMAGED')
  else begin
    done:=false;
    write('Warp direction? ');
    readln(angle);
    if angle<0 then angle:=360-angle;
    if angle>360 then angle:=360;
    repeat
      write('Warp distance in pc? ');
      readln(distance);
      required:=trunc(2.0*distance);
      writeln('Energy required: ',required);
      if energy[3]<required then
        writeln('NOT ENOUGH ENERGY (',required,'% required)')
      else begin
        clearscreen;
        setchsize(3);
        done:=true;
        energy[3]:=energy[3]-required;
        write('Warping...');
        warpx:=sin(angle*cpi)*distance*10.0;
        warpy:=cos(angle*cpi)*distance*10.0;
        for i:=0 to 7 do begin
          x:=sin(ashiptable[i]*cpi)*
            dshiptable[i]-warpx;
          y:=cos(ashiptable[i]*cpi)*
            dshiptable[i]-warpy;
          ashiptable[i]:=
            trunc(atan2(trunc(x),trunc(y)));
          dshiptable[i]:=trunc(sqrt(x*x+y*y));
        end;
        writeln('done');
      end
    until done;
  end;
end;

procedure shoot;
var i,angle:integer;
begin
  if shield then begin
    writeln('LOWERING SHIELD');
    shield:=false;
  end;
  if damage[2]<10 then
    writeln('PHASER DAMAGED')
  else if energy[2]<20 then
    writeln('NOT ENOUGH ENERGY FOR PHASER')
  else begin
    energy[2]:=energy[2]-20;
    write('Phaser direction? ');
    readln(angle);
    clearscreen;
    setchsize(3);
    if angle<0 then angle:=360-angle;
    if angle>360 then angle:=360;
    for i:=0 to 7 do begin
      if (abs(angle-ashiptable[i])<4) and
        (dshiptable[i]<phaserrange) then begin
        shipdamage[i]:=shipdamage[i]-50;
        if shipdamage[i]>0 then begin
          writeln('SHIP ',i+1:1,' DAMAGED');
        score:=score+1;
        end else begin
          writeln('SHIP ',i+1:1,' DESTROYED');
          score:=score+10;
          { Create new ship far away }
          dshiptable[i]:=20000;
          ashiptable[i]:=irand(360);
          shipdamage[i]:=100;
        end;
      end;
    end;
  end;
end;

procedure hit(impact0:integer);
var impact,station: integer;
begin
  impact:=impact0;
  if impact<5 then impact:=5;
  write('STARSHIP HIT, ');
  if shield then begin
    if energy[1]>=impact then begin
      energy[1]:=energy[1]-impact;
      writeln('shield at ',energy[1]:3,'%');
      impact:=0
    end else begin
      impact:=impact-energy[1];
      energy[1]:=0;
      writeln('shield at ',energy[1]:3,'%');
    end
  end;
  if impact>0 then begin
    station:=irand(4);
    case station of
      0: write('Scanner');
      1: write('Shield generator');
      2: write('Phaser');
      3: write('Warp engine')
    end;
    damage[station]:=damage[station]-impact;
    if damage[station]<0 then
      damage[station]:=0;
    if energy[station]>damage[station] then
      energy[station]:=damage[station];
    writeln(' damaged (',
      damage[station]:3,'%)');
  end;
end;

procedure enemy;
var i,dangle:integer;
begin
  for i:=0 to 7 do begin
    if dshiptable[i]>10 then begin
      { warp }
      if irand(256)<128 then begin
        dshiptable[i]:=dshiptable[i]-irand(128);
        if dshiptable[i]<3 then
          dshiptable[i]:=3;
        dangle:=(irand(256)-128) div dshiptable[i];
        ashiptable[i]:=ashiptable[i]+dangle;
        if ashiptable[i]<0 then
          ashiptable[i]:=ashiptable[i]+360
        else if ashiptable[i]>=360 then
          ashiptable[i]:=ashiptable[i]-360;
      end
    end;
    if dshiptable[i]<10 then begin
      if irand(256)<128 then
        hit(irand(40));
    end;
  end;
end;

procedure action;
var i,s:integer;
    sshield:boolean;
    select:char;
begin
  sshield:=shield;
  writeln('CURRENT SCORE ',score:3);
  if not shield then
    writeln('SHIELD IS DOWN');
  writeln('Select action:');
  writeln('R: Repair stations');
  if shield then
    writeln('S: Lower shield')
  else
    writeln('S: Rase shield');
  write('P: Phaser (range ');
  write(phaserrange/10.0:3:1);
  writeln(' pc)');
  writeln('W: Warp');
  writeln('C: Choose station for fast charge');
  writeln('Q: Quit game');
  write('Which action (R,S,P,W,C,Q)? ');
  readln(select);
  if (ord(select) >= ord('a')) and (ord(select) <= ord('z')) then
    select := chr(ord(select) - 32);
  if select='R' then repair
  else if select='S' then begin
    shield:= not shield;
    clearscreen;
    setchsize(3);
    end
  else if select='P' then shoot
  else if select='W' then warp
  else if select='C' then setcharging
  else if select='Q' then quit:=true
  else begin
    clearscreen;
    setchsize(3);
    writeln('NO ACTION TAKEN (',select,')');
  end;
  if not quit then begin
    energy[charging]:=energy[charging]+10;
    if energy[charging]>damage[charging] then
      energy[charging]:=damage[charging];
    for i:=0 to 3 do begin
      energy[i]:=energy[i]+5;
      if energy[i]>damage[i] then
        energy[i]:=damage[i];
    end;
    if shield then begin
      if energy[1]<10 then begin
        writeln('NOT ENOUGH ENERGY, SHIELD DOWN');
        shield:=false;
      end else begin
        energy[1]:=energy[1]-10;
        if energy[1]>damage[1] then
          energy[1]:=damage[1];
      end;
    end;
    enemy;
    if (select='P') and (shield<>sshield)
    then begin
      writeln('RAISING SHIELD');
      shield:=sshield;
    end;
    phaserrange:=10*damage[2] div 100;
    showEnergy;
    showStatus;
    doscan;
    movetoline(15);
    s:=0;
    for i:=1 to 3 do
      s:=s+damage[i];
    if s<30 then begin
      writeln('Starship heavily damaged,',
        ' game over!');
      quit:=true;
    end;
    s:=20000;
    for i:=0 to 7 do begin
      if dshiptable[i]<s then
        s:=dshiptable[i];
    end;
    if s>1000 then begin
      writeln('No nearby ships anymore,',
        ' game over!');
      quit:=true;
    end;
  end;
end;

procedure initialize;
var i,j,s: integer;
    v:real;
begin
  write('Initializing...');
  quit:=false;
  score:=0;
  shield:=true;
  radarsize:=310;
  phaserrange:=10;
  for i:=0 to 89 do begin
    v:=32000.0*sin(i*cpi);
    sinetable[i]:=trunc(v);
  end;
  for i:=0 to 3 do begin
    energy[i]:=80;
    damage[i]:=100;
  end;
  charging:=1;
  for i:=0 to maxships-1 do begin
    ashiptable[i]:=irand(360);
    dshiptable[i]:=irand(128) + radarsize-128;
    shipdamage[i]:=100;
  end;
  for i:=0 to maxships-2 do
    for j:=0 to maxships-2-i do
      if ashiptable[j]>ashiptable[j+1]
      then begin
        s:=ashiptable[j];
        ashiptable[j]:=ashiptable[j+1];
        ashiptable[j+1]:=s;
        s:=dshiptable[j];
        dshiptable[j]:=dshiptable[j+1];
        dshiptable[j+1]:=s;
      end;
  writeln(' done.');
end;

begin {main}
  clearscreen;
  setchsize(3);
  writeln('STARSHIP by rricharz, ',
    'revived 2019');
  writeln('Requires a tek4010 terminal');
  write('Ready (y/n)? ');
  readln(ch);
  if ch = 'y' then begin
    initialize;
    clearscreen;
    setchsize(3);
    showEnergy;
    showStatus;
    scan(1);
    scan(0);
    movetoline(1);
    repeat
      action;
    until quit;
    movetoline(52);
    writeln('Final score: ',score:3);
  end;
end.
 