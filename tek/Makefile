MYBIN="${HOME}/bin"

all: tek thome tgraph

tek: tek.c tekio.c tekio.h
	cc -o tek tek.c tekio.c

thome: thome.c
	cc -o thome thome.c

tgraph: tgraph.c tekio.c tekio.h
	cc -o tgraph tgraph.c tekio.c
install:
	cp tek ${MYBIN}; cp thome ${MYBIN}; cp tgraph ${MYBIN}

