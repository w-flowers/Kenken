#ifndef KENKENSOLVE
#define KENKENSOLVE

#include "kenken-basic.h"

struct dbl_node_sqr {
	int pos[2];				//position in kenken, valid values are 0-5, pos[0] must come from grid[*][], pos[1] from grid[][*]
	int entry;				//number in said position, valid values are 1-6
	struct dbl_node_sqr *next_node;	//next element
	struct dbl_node_sqr *prev_node; //previous element
};

//thoroughly INCOMPLETE
struct pzlsqr {
	int solved; //values of -1 or solution, or 0 if no solutions
	int avlbl_vals[6]; //contains 1-6, values will be deleted and replaced with 0s
};

//returns 0 if there are no solutions, -1 if there are multiple solutions, and 1 if there is a unique solution
int solve_kenken(struct kenken *kkptr);

//preliminary trim of constraint values
int reduce_constraint_arrays_1(struct constraint *ctr, struct pzlsqr psqrs[6][6]);

//more thorough trims, checks for a solution set for the whole constraint
int reduce_constraint_arrays_2(struct constraint *ctr, struct pzlsqr psqrs[6][6]);

//preliminary trim of grid value
int reduce_by_grid(struct pzlsqr psqrs[6][6], struct node_square *sqrnumnd[6][6]);

//go through rows/cols by value, and identify any numbers which can only go in one square
int reduce_by_rowcol(struct pzlsqr psqrs[6][6], struct node_square *sqrnumnd[6][6]);

int reduce_by_hiddenpairs(struct pzlsqr psqrs[6][6]);

int nzero_length(int arr[6]);

int recursive_helper_f_mult(struct dbl_node_sqr *dmynxt, struct dbl_node_sqr *dmyprv, 
	
	struct pzlsqr psqrs[6][6], int test_res, int result);
	

int recursive_helper_f_add(struct dbl_node_sqr *dmynxt, struct dbl_node_sqr *dmyprv, 
	
	struct pzlsqr psqrs[6][6], int test_res);
	


//major rework of brute fill grid **TESTED -- WORKS
int brute_fill_grid_2(struct node_ctr *curr_cst, struct node_square *curr_sqr, 
	
	struct pzlsqr psqrs[6][6], struct pzlsqr psqrs2[6][6], int init_sol[6][6], 
	
	struct node_ctr *init_csts);


//helper for brute fill grid 2
int bfg2_has_mult_sol(struct node_ctr *curr_cst, struct node_square *curr_sqr, 

	struct pzlsqr psqrs[6][6], struct pzlsqr psqrs2[6][6], struct pzlsqr dmypsqrs[6][6], 
	
	int init_sol[6][6], struct node_ctr *init_csts);


//returns 1 if psqrs == init_sol, otherwise 0
int eq_init_sol(struct pzlsqr psqrs[6][6], int init_sol[6][6]);

int eq_init_sol_2(struct pzlsqr psqrs[6][6], struct pzlsqr init_sol[6][6]);

//eliminates values for an individual square based on the existing grid
int reduce_by_grid_2(struct pzlsqr psqrs[6][6], int hori_ind, int vert_ind);

//copies psqrs to psqrs2
int copy_pzlsqrs(struct pzlsqr psqrs2[6][6], struct pzlsqr psqrs[6][6]);

#endif
