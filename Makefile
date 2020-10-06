.POSIX:
.SUFFIXES:
CC		= gcc
CFLAGS	= -Wall -Wextra -O0 -g
LDLIBS	= -F /Library/Frameworks `sdl2-config --cflags --libs` -lSDL2_ttf

all: kenken
kenken: obj/GUI-kenken.o obj/kenken-solver.o obj/kenken-basic.o
	$(CC) $(CFLAGS) -o kenken obj/kenken-basic.o obj/kenken-solver.o obj/GUI-kenken.o $(LDLIBS)
obj/kenken-basic.o: src/kenken-basic.c inc/kenken-basic.h
	$(CC) -c $(CFLAGS) src/kenken-basic.c -o obj/kenken-basic.o
obj/kenken-solver.o: src/kenken-solver.c inc/kenken-solver.h inc/kenken-basic.h
	$(CC) -c $(CFLAGS) src/kenken-solver.c -o obj/kenken-solver.o
obj/GUI-kenken.o: src/GUI-kenken.c inc/GUI-kenken.h inc/kenken-solver.h inc/kenken-basic.h
	$(CC) -c $(CFLAGS) src/GUI-kenken.c $(LDLIBS) -o obj/GUI-kenken.o
clean:
	rm -f kenken obj/kenken-basic.o obj/kenken-solver.o obj/GUI-kenken.o
