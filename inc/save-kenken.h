#ifndef SAVE_KENKEN_H_
#define SAVE_KENKEN_H_

#include<string.h>
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include "kenken-basic.h"

//saves a kenken to a file in the format listed above
//Returns 1 on failure
int save_kenken(struct kenken *kkptr, int usrgrid[6][6], char* filename);

//takes the contents of a .kenken and loads it to a kenken pointer
//returns one and wipes the kkptr on failure
int load_kenken(struct kenken *kkptr, int usrgrid[6][6], char* filename);

// Helper function for save_kenken
int save_grid( int grid[6][6] , FILE *kkfile );

// Helper function for save_kenken
int save_constraints( struct node_ctr *ctrs, FILE *kkfile );

#endif
