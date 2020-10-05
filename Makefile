.POSIX:
.SUFFIXES:
CC		= gcc
CFLAGS	= -Wall -Wextra -Wpedantic
LDLIBS	= -lSDL2_ttf -lSDL2  `sdl2-config --cflags --libs`

all: kenken
kenken: GUI-kenken.o kenken-solver.o kenken-basic.o
	$(CC) $(LDFLAGS) -o kenken kenken-basic.o kenken-solver.o GUI-kenken.o $(LDLIBS)
kenken-basic.o: kenken-basic.c kenken-basic.h
	$(CC) -c $(CFLAGS) kenken-basic.c
kenken-solver.o: kenken-solver.c kenken-solver.h kenken-basic.h
	$(CC) -c $(CFLAGS) kenken-solver.c
GUI-kenken.o: GUI-kenken.c GUI-kenken.h kenken-solver.h kenken-basic.h
	$(CC) -c $(CFLAGS) GUI-kenken.c $(LDLIBS)
clean:
	rm -f kenken kenken-basic.o kenken-solver.o GUI-kenken.o
