#ifndef KENKENBASE
#define KENKENBASE

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define ADDOP 0
#define MULTOP 1
#define SUBOP 2
#define DIVOP 3
#define NOOP 4


// A program to take a 6x6 array of numbers and determine if the result is a valid Kenken **May have grown beyond its original scope to include generation**//

//Linked list element for logical constraints 
struct node_square {
	int pos[2];				//position in kenken, valid values are 0-5, pos[0] must come from grid[*][], pos[1] from grid[][*]
	int entry;				//number in said position, valid values are 1-6
	struct node_square *next_node;	//next element
};

//A single constraint on the puzzle
struct constraint{
	int result;				//the result of the operation
	int op;					//the operation: defined by preprocessor
	struct node_square *numbers;	//the list of squares in the element
};

//Linked list of constraints
struct node_ctr{
	struct constraint constraint;
	struct node_ctr *next_node;
};

//Usually created in Main, and then populated using r_fill_grid and generate_constraints
struct kenken{
	struct node_ctr *ctrs;
	int grid[6][6];
};

int valid_grid( int[6][6] );
//returns 1 if it is valid, 0 if not **TESTED -- WORKS

int valid_constraint( struct constraint * );
//returns 1 if valid, 0 if not **TESTED -- WORKS

int kenken_invalid( struct kenken * );
//returns 0 if valid, non-zero if not **TESTED IN MAIN USING r_fill_grid -- works in that case

int list_length( struct node_square * );
//gives the length of a linked list for node_square

void r_fill_grid( int [6][6] );
//randomly generates an underlying grid for a kenken, given a pointer to a blank array **TESTED -- WORKS

int random_available( int[6] );
//takes an array of available integers padded with 0's and returns one of those integers **TESTED -- WORKS

int remove_available( int[6], int );
//removes a given number ( 1-6 ) from the list of available numbers for a square **TESTED -- WORKS
//returns 1 if an element is removed, 0 otherwise

int fill_square( int, int, int [6][6] );
//fills squares of grid for generate function **TESTED -- WORKS

int generate_kenken( struct kenken * );
//generates the kenken structure using the generate_constraints and r_fill_grid functions
//returns 0 in the event of a malloc failure ****TESTED-Works

int destroy_kenken( struct kenken * );
//Deallocates allocated memory in a of a generated kenken. **TESTED - WORKS

int generate_constraints( struct kenken * );
//generates the list of constraints for a puzzle, and maintains a list of unused squares
//marks a square as unused by setting it to 6 in local array **TESTED-Works

//NOTE: this function may require tweaking to generate good Kenkens to solve, however, the initial design goal is to create a working function

struct node_square *random_square_walk( int, int [36][2], int [6][6] );
//takes an integer length and a list of available cells, and uses this to randomly return a list
//of available squares of this length, and then removes the given cells from the availability array
//( by setting as 0 ) **TESTED-Works

int create_constraint( int, struct node_square*, struct constraint * );
//takes the operation and a pointer to a list of squares and populates the constraint,
//returns 0 if a failure occurs, else 1 **TESTED-Works

int r_assign_op( int, int, int );
//Assigns an operation given the length of the list of nubmers of a constraint

int ( * random_available_sqr( int [36][2], int[2], int[4][2] ) )[2];
//as above but for squares, and uses 6 as a dead value instead of 0
//if second argument is [6][6], returns any random square, else returns one adjacent to argument if possible
//returns 0 otherwise **TESTED-Works

int remove_available_sqr( int [36][2], int[2] );
//removes a given square i 0-5, j 0-5, from the list of available squares **TESTED-Works
//returns 1 if an element is removed, 0 otherwise

int valid_partial_kenken( struct kenken kenken );
//Return 1 if a kenken is correct so far, or 0 if there are no solutions for the given state. 
//**TESTED --Works

int valid_partial_grid( int arr[6][6] );
//Return 1 if the grid has no invalid partial rows or columns **UNTESTED
//helper for valid_partial_kenken

int valid_partial_constraint( struct constraint *ptr );
//return 1 if all constraints have valid entries, 0 otherwise **UNTESTED
//helper for valid_partial_kenken

int copy_kenken( struct kenken *kkptr, struct kenken *newkkptr );
//copies the contents of kkptr to newkkptr, returns 0 in event of malloc failure

struct node_ctr *copy_constraints( struct kenken *kkptr );
//copies the contents of kkptr to newkkptr, returns 0 in event of malloc failure

int update_usr_kenken( struct kenken *usrkk );
//ensures that all the "entry" records in the struct node_square's match the number given in usrkk->grid

#endif
