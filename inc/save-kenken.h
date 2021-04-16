#ifndef SAVE_KENKEN_H_
#define SAVE_KENKEN_H_

#include<string.h>
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include "kenken-basic.h"

int save_kenken(struct kenken *kkptr, int usrgrid[6][6], char* filename);

int load_kenken(struct kenken *kkptr, int usrgrid[6][6], char* filename);

int save_grid( int grid[6][6] , FILE *kkfile );

int save_constraints( struct node_ctr *ctrs, FILE *kkfile );

int read_grid_helper ( int target[6][6], FILE *kkfile );

int read_constraints( struct kenken *kkptr, FILE *kkfile );

#endif
