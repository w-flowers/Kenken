.POSIX:
.SUFFIXES:
CC		= gcc
CFLAGS	= -Wall -Wextra -Wpedantic
LDLIBS	= -lSDL2_ttf -lSDL2  

all: kenken
kenken: obj/GUI-kenken.o obj/kenken-solver.o obj/kenken-basic.o
	$(CC) $(LDFLAGS) -o kenken obj/kenken-basic.o obj/kenken-solver.o obj/GUI-kenken.o $(LDLIBS)
obj/kenken-basic.o: src/kenken-basic.c inc/kenken-basic.h
	$(CC) -c $(CFLAGS) src/kenken-basic.c
obj/kenken-solver.o: src/kenken-solver.c inc/kenken-solver.h inc/kenken-basic.h
	$(CC) -c $(CFLAGS) src/kenken-solver.c
obj/GUI-kenken.o: src/GUI-kenken.c inc/GUI-kenken.h inc/kenken-solver.h inc/kenken-basic.h
	$(CC) -c $(CFLAGS) src/GUI-kenken.c $(LDLIBS)
clean:
	rm -f kenken obj/kenken-basic.o obj/kenken-solver.o obj/GUI-kenken.o
