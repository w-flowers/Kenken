.POSIX:
.SUFFIXES:
CC		= gcc
CFLAGS	= -Wall -Wextra -O0 -g
LDLIBS	= -F /Library/Frameworks/ `sdl2-config --cflags --libs` -lSDL2_ttf

all: mathdoku
mathdoku: obj/GUI-kenken.o obj/kenken-solver.o obj/kenken-basic.o obj/menu.o obj/save-kenken.o
	$(CC) $(CFLAGS) -o mathdoku obj/kenken-basic.o obj/kenken-solver.o obj/menu.o obj/save-kenken.o obj/GUI-kenken.o $(LDLIBS)
obj/kenken-basic.o: src/kenken-basic.c inc/kenken-basic.h
	$(CC) -c $(CFLAGS) src/kenken-basic.c -o obj/kenken-basic.o
obj/kenken-solver.o: src/kenken-solver.c inc/kenken-solver.h inc/kenken-basic.h
	$(CC) -c $(CFLAGS) src/kenken-solver.c -o obj/kenken-solver.o
obj/save-kenken.o: src/save-kenken.c inc/save-kenken.h inc/kenken-basic.h
	$(CC) -c $(CFLAGS) src/save-kenken.c -o obj/save-kenken.o -DFILEPATH=\"$(CURDIR)\"
obj/menu.o: src/menu.c inc/menu.h inc/GUI-kenken.h
	$(CC) -c $(CFLAGS) src/menu.c $(LDLIBS) -o obj/menu.o -DFILEPATH=\"$(CURDIR)\"
obj/GUI-kenken.o: src/GUI-kenken.c inc/GUI-kenken.h inc/kenken-solver.h inc/kenken-basic.h inc/menu.h inc/save-kenken.h
	$(CC) -c $(CFLAGS) src/GUI-kenken.c $(LDLIBS) -o obj/GUI-kenken.o -DFILEPATH=\"$(CURDIR)\"
clean:
	rm -f mathdoku obj/kenken-basic.o obj/kenken-solver.o obj/GUI-kenken.o obj/menu.o obj/save-kenken.o
