MYBIN="${HOME}/bin"

all: tek thome tgraph test4010 test4014 dodekagon mathdemo mathdemo2 animation circles testGIN testVarious

testGIN: testGIN.c tekio.c tekio.h
	cc -o testGIN testGIN.c tekio.c -lm

tek: tek.c tekio.c tekio.h
	cc -o tek tek.c tekio.c -lm

thome: thome.c
	cc -o thome thome.c

tgraph: tgraph.c tekio.c tekio.h
	cc -o tgraph tgraph.c tekio.c -lm

dodekagon: tekio.h tekio.c dodekagon.c
	cc -o dodekagon dodekagon.c tekio.c -lm

mathdemo: mathdemo.c tekio.h tekio.c
	cc -o mathdemo mathdemo.c tekio.c -lm

mathdemo2: mathdemo2.c tekio.h tekio.c
	cc -o mathdemo2 mathdemo2.c tekio.c -lm

test4010: test4010.c
	cc -o test4010 test4010.c -l4013 -lm

test4014: test4014.c
	cc -o test4014 test4014.c -l4014 -lm

animation: animation.c tekio.h tekio.c
	cc -o animation animation.c tekio.c -lm

circles: circles.c tekio.h tekio.c
	cc -o circles circles.c tekio.c -lm

testVarious: testVarious.c tekio.h tekio.c
	cc -o testVarious testVarious.c tekio.c -lm

install:
	cp tek ${MYBIN}; cp thome ${MYBIN}; cp tgraph ${MYBIN}; cp dodekagon ${MYBIN}

