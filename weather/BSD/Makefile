MYBIN="${HOME}/bin"

all: weather weather2 weather3

weather: weather.c
	cc -o weather weather.c

weather2: weather2.c
	cc -o weather2 weather2.c

weather3: weather3.c
	cc -o weather3 weather3.c -lcurses -ltermcap

install:
	cp weather ${MYBIN}; cp weather2 ${MYBIN}; cp weather3 ${MYBIN}
