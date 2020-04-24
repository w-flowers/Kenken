.POSIX:
.SUFFIXES:
CC		= gcc
CFLAGS	= -Wall
LDLIBS	= -lSDL2 -lSDL2_ttf

all: kenken
kenken: GUI-kenken.o kenken-solver.o kenken-basic.o
	$(CC) $(LDFLAGS) -o kenken kenken-basic.o kenken-solver.o GUI-kenken.o $(LDLIBS)
kenken-basic.o: kenken-basic.c kenken-basic.h
	gcc -c $(CFLAGS) kenken-basic.c
kenken-solver.o: kenken-solver.c kenken-solver.h kenken-basic.h
	gcc -c $(CFLAGS) kenken-solver.c
GUI-kenken.o: GUI-kenken.c GUI-kenken.h kenken-solver.h kenken-basic.h
	gcc -c $(CFLAGS) GUI-kenken.c $(LDLIBS)
clean:
	rm -f kenken kenken-basic.o kenken-solver.o GUI-kenken.o