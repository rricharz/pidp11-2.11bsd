MYBIN="${HOME}/bin"

all: tek thome tgraph testPlotlib dodekagon

tek: tek.c tekio.c tekio.h
	cc -o tek tek.c tekio.c

thome: thome.c
	cc -o thome thome.c

tgraph: tgraph.c tekio.c tekio.h
	cc -o tgraph tgraph.c tekio.c

dodekagon: tekio.h tekio.c dodekagon.c
	cc -o dodekagon dodekagon.c tekio.c -lm

testPlotlib: testPlotlib.c
	cc -o testPlotlib testPlotlib.c -l4013 -lm

install:
	cp tek ${MYBIN}; cp thome ${MYBIN}; cp tgraph ${MYBIN}

