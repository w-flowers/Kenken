# Mathdoku:

A simple GUI program that allows the user to play a 6x6 mathdoku (A.K.A. Kenken) game.

# To install:

Ensure that SDL2 and SDL_ttf are installed on your system. See https://www.libsdl.org/ and https://www.libsdl.org/projects/SDL_ttf/ for more info.

Clone this repository, and run "make all". This will create an executable "mathdoku".

Context: This game was made entirely for my own development as a programmer, and is definitely still a work in progress. It began as an excuse to implement linked lists in C, as I figured that, since each constraint could be a different size, a linked list was an appropriate data structure. However, as I progressed, it began to take on its own life. 

As it developed, I had the chance to develop my skills developing a mid-sized c program, learnt about many particularities of pointers, memory allocation, arrays and functions that have massively improved my programming abilities. I also got to work with doubly-linked lists, 3rd party libraries, events, file I/O, and a huge variety of C data structures. 

This program also tested my ability to design a large program with a variety of interlocking components and interfaces. I made plenty of mistakes early in the process which required me to go back and redesign or reimplement components, but through this I developed strong skills in designing data structures, interfaces and architectures, and then implementing them using simple, understandable and reusable code.

Features:

## Mathdoku Generator:

- Generates a random grid of numbers, with no repeats in any row or column
- Randomly generates a constraint, which uses a random walk algorithm ensure that the constraint is connected
- Contains programs to check the validity of a mathdoku or a single constraint
- After all constraints are generated, the mathdoku is checked with the solver to have a unique solution

## Mathdoku Solver:

- A simple solver that creates an array of possible solutions for each square of the mathdoku, and then trims this as much as possible using the initial conditions of the mathdoku (checking against rows, columns and individual constraints), before brute forcing any remaining unsolved sections of the mathdoku
- This component also stores the first solution it finds, and checks the mathdoku to determine if there are any other solutions
- The brute force algorithm uses a recursive algorithm which fills in a square with an available number, then, if the puzzle is solved, returns success, else it calls the algorithm on the next square. If no solution can be found, it returns a failure, which is passed to the preceeding square, which eliminates its previous selection from its list of available choices. NOTE: This is an approximate description, there is further complexity in the real implementation.

## GUI:

- A simple main loop GUI, which stores the user game data and draws it the screen, giving the user a solvable puzzle
- The user can enter numbers into this puzzle, can check their progress, delete entries, see the solution, save their progress, or generate a new mathdoku
- Implemented with SDL2 and SDL_ttf

## Save/Load:

- A simple savegame functionality which can take users mathdoku and save it as a text file
- A load function has been written which parses the text file, and if it is a valid mathdoku, populates a mathdoku pointer with the data from said file
- A menu which allows a user to load a mathdoku from saved mathdokus, listed by date

## Screenshots:

![solved game](res/img10.png)

![new game](res/img9.png)

![partially completed game](res/img8.png)

![start menu](res/img5.png)

![load menu](res/img6.png)

