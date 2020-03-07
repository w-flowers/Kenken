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

int valid_grid(int[6][6]);
//returns 1 if it is valid, 0 if not **TESTED -- WORKS

int valid_constraint(struct constraint *);
//returns 1 if valid, 0 if not **TESTED -- WORKS

int kenken_valid(struct kenken *);
//returns 1 if valid, 0 if not **TESTED IN MAIN USING r_fill_grid -- works in that case

int list_length(struct node_square *);
//gives the length of a linked list for node_square

struct node_square create_numbers_list();
//creates the initial node_square

struct node_ctr create_ctr_list();
//creates the initial node_ctr

void r_fill_grid(int [6][6]);
//randomly generates an underlying grid for a kenken, given a pointer to a blank array **TESTED -- WORKS

int random_available(int[6]);
//takes an array of available integers padded with 0's and returns one of those integers **TESTED -- WORKS

int remove_available(int*, int);
//removes a given number (1-6) from the list of available numbers for a square **TESTED -- WORKS

int fill_square(int, int, int [6][6]);
//fills squares of grid for generate function **TESTED -- WORKS

int generate_kenken(struct kenken *);
//generates the kenken structure using the generate_constraints and r_fill_grid functions
//returns 0 in the event of a malloc failure ****TESTED-Works

int destroy_kenken(struct kenken *);
//Deallocates allocated memory in a of a generated kenken. **TESTED - WORKS

int generate_constraints(struct kenken *);
//generates the list of constraints for a puzzle, and maintains a list of unused squares
//marks a square as unused by setting it to 6 in local array **TESTED-Works

//NOTE: this function may require tweaking to generate good Kenkens to solve, however, the initial design goal is to create a working function

struct node_square *random_square_walk(int, int [36][2], int [6][6]);
//takes an integer length and a list of available cells, and uses this to randomly return a list
//of available squares of this length, and then removes the given cells from the availability array
//(by setting as 0) **TESTED-Works

int create_constraint(int, struct node_square*, struct constraint *);
//takes the operation and a pointer to a list of squares and populates the constraint,
//returns 0 if a failure occurs, else 1 **TESTED-Works

int r_assign_op(int, int, int);
//Assigns an operation given the length of the list of nubmers of a constraint

int (* random_available_sqr(int [36][2], int[2], int[4][2]))[2];
//as above but for squares, and uses 6 as a dead value instead of 0
//if second argument is [6][6], returns any random square, else returns one adjacent to argument if possible
//returns 0 otherwise **TESTED-Works

int remove_available_sqr(int [36][2], int[2]);
//removes a given square i 0-5, j 0-5, from the list of available squares **TESTED-Works

int solve_kenken(struct kenken *kenken);
//Given an empty or partially completed grid and a list of constraints, solves the kenken for 
//these conditions, or returns 0 if the initial conditions are not solvable or have multiple 
//solutions. 

int valid_partial_kenken(struct kenken kenken);
//Return 1 if a kenken is correct so far, or 0 if there are no solutions for the given state.

/******************************************

Functions below

*******************************************/

//the following functions need modification to work in this environment
/*void append_node(struct node_square *, int[]);
void prepend_node(struct node_square *, int[], struct node_square **);
TYPE access_node(struct node_square *, int);
void remove_node(struct node_square *, int, struct node_square **);*/
/*
int main(){
	int arr[6][6] = {
		{1, 2, 3, 4, 5, 6},
		{2, 3, 4, 5, 6, 1},
		{3, 4, 5, 6, 1, 2},
		{4, 5, 6, 1, 2, 3},
		{5, 6, 1, 2, 3, 4},
		{6, 1, 2, 3, 4, 5}
	};
	int row_test[6][6] = {
		{2, 2, 3, 4, 5, 6},
		{1, 3, 4, 5, 6, 1},
		{3, 4, 5, 6, 1, 2},
		{4, 5, 6, 1, 2, 3},
		{5, 6, 1, 2, 3, 4},
		{6, 1, 2, 3, 4, 5}
	};
	int col_test[6][6] = {
		{1, 2, 3, 4, 5, 6},
		{2, 3, 4, 5, 6, 1},
		{3, 4, 5, 6, 1, 2},
		{4, 5, 2, 1, 6, 3},
		{5, 6, 1, 2, 3, 4},
		{6, 1, 2, 3, 4, 5}
	};
	int invalid_entry_test[6][6] = {
		{1, 2, 3, 4, 5, 6},
		{2, 3, 4, 5, 6, 1},
		{3, 4, 5, 6, 1, 2},
		{4, 5, 6, 1, 2, 3},
		{5, 6, 1, 2, 3, 4},
		{6, 1, 2, 3, 4, 0}
	};
	if(valid_grid(arr)) printf("Returns 1 for true Kenken\n");
	else printf("ERROR: Returns 0 for true Kenken\n");
	if(!valid_grid(row_test)) printf("Returns 0 for invalid rows\n");
	else printf("ERROR: Returns 1 for invalid rows\n");
	if(!valid_grid(col_test)) printf("Returns 0 for invalid cols\n");
	else printf("ERROR: Returns 1 for invalid cols\n");
	if(!valid_grid(invalid_entry_test)) printf("Returns 0 for invalid entries\n");
	else printf("ERROR: Returns 1 for invalid entries\n");
	printf("return result for true Kenken is %d\n", valid_grid(arr));
	printf("return result for invalid rows is %d\n", valid_grid(row_test));
	printf("return result for invalid cols is %d\n", valid_grid(col_test));
	printf("return result for invalid entries is %d\n", valid_grid(invalid_entry_test));*/
	//test constraint check function
	/*struct constraint tsqrctr; //true one square constraint
	tsqrctr.result = 5;
	tsqrctr.op = 0;
	tsqrctr.numbers.pos[0] = 1;
	tsqrctr.numbers.pos[1] = 1;
	tsqrctr.numbers.entry = 5;
	tsqrctr.numbers.next_node = NULL;
	printf("Valid 1 Square Constraint returns %d\n", valid_constraint(&tsqrctr));
	struct constraint fsqrctr; //false one square constraint
	fsqrctr.result = 5;
	fsqrctr.op = 0;
	fsqrctr.numbers.pos[0] = 1;
	fsqrctr.numbers.pos[1] = 1;
	fsqrctr.numbers.entry = 4;
	fsqrctr.numbers.next_node = NULL;
	printf("False 1 Square Constraint returns %d, unequal values\n", valid_constraint(&fsqrctr));
	struct constraint f2sqrctr; //false one square constraint
	f2sqrctr.result = 5;
	f2sqrctr.op = 0;
	f2sqrctr.numbers.pos[0] = 1;
	f2sqrctr.numbers.pos[1] = 1;
	f2sqrctr.numbers.entry = 5;
	struct node_square ele2;
	ele2.pos[0] = 1;
	ele2.pos[1] = 2;
	ele2.entry = 4;
	ele2.next_node = NULL;
	f2sqrctr.numbers.next_node = &ele2;
	printf("False 1 Square Constraint returns %d, list too long\n", valid_constraint(&f2sqrctr));*/
	/*struct constraint a2sqrctr; //true 2 square add constraint
	a2sqrctr.result = 9;
	a2sqrctr.op = 1;
	a2sqrctr.numbers.pos[0] = 1;
	a2sqrctr.numbers.pos[1] = 1;
	a2sqrctr.numbers.entry = 5;
	struct node_square aele2;
	aele2.pos[0] = 1;
	aele2.pos[1] = 2;
	aele2.entry = 4;
	aele2.next_node = NULL;
	a2sqrctr.numbers.next_node = &aele2;
	printf("true 2 Square Constraint add returns %d, \n", valid_constraint(&a2sqrctr));*/
	/*struct constraint sqrctr; //true 2 square add constraint
	sqrctr.result = 12;
	sqrctr.op = 1;
	sqrctr.numbers.pos[0] = 1;
	sqrctr.numbers.pos[1] = 3;
	sqrctr.numbers.entry = 6;
	struct node_square aele2;
	aele2.pos[0] = 2;
	aele2.pos[1] = 3;
	aele2.entry = 6;
	aele2.next_node = NULL;
	struct node_square aele3;
	aele3.pos[0] = 2;
	aele3.pos[1] = 4;
	aele3.entry = 6;
	aele3.next_node = NULL;
	aele2.next_node = &aele3;
	sqrctr.numbers.next_node = &aele2;
	printf("false 2 Square Constraint add returns %d, \n", valid_constraint(&sqrctr));
	//test random generation function
	int grid[6][6];
	int counter = 0;
	int counter2 = 0;
	srand(time(NULL));
	r_fill_grid(grid);
	for(int i = 0; i < 6; i++){
		printf("{ ");
		for(int j = 0; j < 6; j++){
			printf("%d ", grid[i][j]);
		}
		printf("}\n");
	}*/
	/*int counter = 0;
	for(int i = 0; i < 10000; i++){
		int grid2[6][6];
		r_fill_grid(grid2);
		if(!valid_grid(grid2)){
			counter++;
		}
	}
	printf("Number of errors is: %d\n", counter);
	return 0; 
	int a = (int) time(NULL);
	for(int i = 0; i < 10000000; i++){
		struct kenken example;
		generate_kenken(&example);
		if(!valid_grid(example.grid)){
			counter++;
		}
		if(!kenken_valid(&example)) counter2++;
		destroy_kenken(&example);
	}
	int b = (int) time(NULL);
	printf("Number of grid errors is: %d\n", counter);
	printf("Number of constraint errors is: %d\n", counter2);
	printf("Time taken is %d\n", (b - a));
	return 0;
}*/
                 
int valid_constraint(struct constraint *ptr){
	//printf("valid constraint called, ptrop is %d\n", ptr->op);
	//check for valid value of operation
	if((ptr->op) < 0 || (ptr->op) > 4) return 0;
	//check result of addition
	if(ptr->op == ADDOP){
		if(list_length(ptr->numbers) <= 1) return 0;
		int test_res = 0;
		struct node_square *a_ptr;
		for(a_ptr = ptr->numbers; a_ptr != NULL; a_ptr = a_ptr->next_node){
			test_res += (a_ptr->entry);
		}
		if(test_res != ptr->result) return 0;
	}
	//check correct number of arguments for subtraction
	if(ptr->op == SUBOP){
		if(list_length(ptr->numbers) != 2) return 0;
		int a = (ptr->numbers)->entry;
		int b = ((ptr->numbers)->next_node)->entry;
		if(a == b) return 0;
		if(a > b){
			if((a-b) != ptr->result) return 0;
		}
		if(b > a){
			if((b-a) != ptr->result) return 0;
		}
	}
	//check result of multiplication
	if(ptr->op == MULTOP){
		if(list_length(ptr->numbers) <= 1) return 0;
		int test_res = 1;
		struct node_square *a_ptr;
		for(a_ptr = ptr->numbers; a_ptr != NULL; a_ptr = a_ptr->next_node){
			test_res = test_res*(a_ptr->entry);
		}
		if(test_res != ptr->result) return 0;
		
	}
	//check mod 0 and number of arguments for division
	if(ptr->op == DIVOP){
		if(list_length(ptr->numbers) != 2) return 0;
		int a = (ptr->numbers)->entry;
		int b = ((ptr->numbers)->next_node)->entry;
		if(a == b) return 0;
		if(a > b){
			if((a/b) != ptr->result || a%b != 0) return 0;
		}
		if(b > a){
			if((b/a) != ptr->result || b%a != 0) return 0;
		}
	}
	//check value of result given op
	if(ptr->op == NOOP){
		if(list_length(ptr->numbers) != 1){
			printf("list too long\n");
			return 0;
		}
		else if(ptr->result != (ptr->numbers)->entry){
			printf("unequal values\n");
			return 0;
		}
		else return 1;
	}
	//check position of squares for adjacency to at least 1 other square
	struct node_square *b_ptr;
	for(b_ptr = ptr->numbers; b_ptr != NULL; b_ptr = b_ptr->next_node){
		struct node_square *c_ptr;
		int has_adj_sqrs = 0;
		for(c_ptr = ptr->numbers; c_ptr != NULL; c_ptr = c_ptr->next_node){
			if((abs((b_ptr->pos[0])-(c_ptr->pos[0]))+abs((b_ptr->pos[1])-(c_ptr->pos[1]))) == 1) has_adj_sqrs = 1; 
		}
		if(!has_adj_sqrs) return 0;
	}
	return 1;
}



int valid_grid(int arr[6][6]){
	for(int cellcont = 1; cellcont < 7; cellcont++){
		//check rows
		for(int rownum = 0; rownum < 6; rownum++){
			int num_num = 0;
			for(int colnum = 0; colnum < 6; colnum++){
				if(arr[rownum][colnum] == cellcont) num_num++;
			}
			if(num_num != 1) return 0;
		}
		//check columns
		for(int colnum = 0; colnum < 6; colnum++){
			int num_num = 0;
			for(int rownum = 0; rownum < 6; rownum++){
				if(arr[rownum][colnum] == cellcont) num_num++;
			}
			if(num_num != 1) return 0;
		}
	}
	return 1;
}

int kenken_valid(struct kenken *kenken_ptr){
	if(!valid_grid(kenken_ptr->grid)) return 0;
	//printf("initial pointer is %p\n", &(kenken_ptr->ctrs));
	for(struct node_ctr *d_ptr = kenken_ptr->ctrs; d_ptr != 0; d_ptr = d_ptr->next_node){
		//printf("not infinite looping for some reason\n");
		if(!valid_constraint(&(d_ptr->constraint))){
			printf("Invalid constraint\n");
			return 0;
		}
	}
	//printf("returned all constraints valid\n");
	//check each square occurs exactly once in constraint list,
	//and that that square's entry matches the entry in the constraint
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			int n_sqr = 0; //no. of occurences of square
			for(struct node_ctr *d2_ptr = kenken_ptr->ctrs; d2_ptr != 0; d2_ptr = d2_ptr->next_node){
				//printf("constraint list is result %d op %d squares:\n", d2_ptr->constraint.result, d2_ptr->constraint.op);
				//walk through constraint list
				for(struct node_square *d1_ptr = (d2_ptr->constraint).numbers; d1_ptr != NULL; d1_ptr = d1_ptr->next_node){ //walk through constraint numbers
					//printf("entry pos %d %d value is %d\n", d1_ptr->pos[0], d1_ptr->pos[1], d1_ptr->entry);
					if(d1_ptr->pos[0] == i && d1_ptr->pos[1] == j){
						if(d1_ptr->entry != kenken_ptr->grid[i][j]){
							 printf("Wrong element at square\n");
							 return 0;
						 }
						else n_sqr++;
					}
				}
			}
			if(n_sqr != 1) {
				printf("wrong number of squares\n");
				return 0;
			}
		}
		
	}
	return 1;
}

void r_fill_grid(int grid[6][6]){
	//int arr[6][6];
	fill_square(0, 0, grid);
	/*for(int i = 0; i < 6; i++){
		//printf("{ ");
		for(int j = 0; j < 6; j++){
			//printf("%d ", arr[i][j]);
			grid[i][j] = arr[i][j];
		}
		//printf("}\n");
	}*/
}

int random_available(int available[6]){
	int i = 0;
	//srand (time(NULL));
	for(int *k = &available[0]; *k != 0 && k < &available[6]; k++) i++;
	if(i == 1) return available[0];
	int index = rand() % (i);
	//printf("rand returned this i:%d index:%d number:%d\n", i, index, available[index]);
	return available[index];
}

int remove_available(int* available, int number){
	if(number < 1 || number > 6) return 0;
	int i = 0;
	while(*(available+i) != number && i < 6) i++;
	if(i == 6) return 0; //not in array
	for(int j = i; j < 5; j++) *(available + j) = *(available + j + 1);
	*(available+5) = 0;
	return 1; //success
}

int fill_square(int i, int j, int arr[6][6]){
	//printf("fill square called at i:%d, j:%d\n", i, j);
	int available[6] = {1,2,3,4,5,6};
	for(int k = 0; k < j; k++){
		int sqr = arr[i][k];
		remove_available(available, sqr);
	}
	for(int k = 0; k < i; k++){
		int sqr = arr[k][j];
		remove_available(available, sqr);
	}
	while(available[0] != 0){
		arr[i][j] = random_available(available);
		if(i == 5 && j == 5){
			return 1;
		}
		if(fill_square((i+(j+1)/6),(j+1)%6,arr) == 0){
			remove_available(available, arr[i][j]);
		}
		else{
			//printf("1 returned ij = %d%d\n", i, j);
			return 1;
		}
	}
	//printf("0 returned ij %d%d\n", i, j);
	return 0;
}

int generate_kenken(struct kenken *kenkenptr){
	srand(time(NULL));
	r_fill_grid(kenkenptr->grid);
	//struct node_ctr *constraints = (struct node_ctr *)malloc(sizeof(struct node_ctr));;
	//constraints = &(kenkenptr->ctrs);
	//struct node_ctr **head = &constraints;
	//kenkenptr->ctrs.next_node = NULL;
	int x = generate_constraints(kenkenptr);
	//kenkenptr->ctrs = *constraints;
	return x;
}

int destroy_kenken(struct kenken *kenkenptr){
	//walk through constraints
	for(struct node_ctr *dmy_ptr = kenkenptr->ctrs; dmy_ptr != NULL;){
		//walk through node_square s, freeing them
		//printf("gets to outer loop\n");
		for(struct node_square *dmysqrptr = dmy_ptr->constraint.numbers; dmysqrptr != NULL;){
			//printf("gets to inner loop\n");
			struct node_square *dltsqrptr = dmysqrptr;
			dmysqrptr = dmysqrptr->next_node;
			free(dltsqrptr);
		}
		//free constraint, then next constraint
		struct node_ctr *dltctrptr = dmy_ptr;
		dmy_ptr = dmy_ptr->next_node;
		free(dltctrptr);
	}
	return 1;
}

int generate_constraints(struct kenken *kenkenptr){
	int available[36][2];
	for(int i = 0; i < 36; i++){
		available[i][0] = i/6;
		available[i][1] = i%6;
		//printf("(%d, %d)", available[i][0], available[i][1]);
	}
	struct node_ctr *dummy_ptr;
	//dummy_ptr = *head;
	//printf("generate constraints executed once\n");
	struct node_square *list = random_square_walk(4, available, kenkenptr->grid);
	//printf("random square walk executed once\n");
	if(list != NULL){
		struct node_ctr *fst_ele = (struct node_ctr *)malloc(sizeof(struct node_ctr));
		if(fst_ele == NULL) return 0;
	//	printf("random square walk returned a valid pointer\n");
		create_constraint(r_assign_op(4, 4, 5), list, (&(fst_ele->constraint)));
		fst_ele->next_node = NULL;
		dummy_ptr = fst_ele;
		//printf("4 square, head, %p, tail %p\n", dummy_ptr, dummy_ptr->next_node);
	}
	//else printf("random square walk returned null\n");
	int counter = 0;
	while(counter < 2){
		list = random_square_walk(3, available, kenkenptr->grid);
		if(list != NULL){
			struct node_ctr *next_ele = (struct node_ctr *)malloc(sizeof(struct node_ctr));
			if(next_ele == NULL) return 0;
			create_constraint(r_assign_op(3, 4, 5), list, &(next_ele->constraint));
			next_ele->next_node = dummy_ptr;
			dummy_ptr = next_ele;
			counter++;
			//printf("3 square, head, %p, tail\n", dummy_ptr);
		}
	}
	counter = 0;
	while(counter < 15){
		list = random_square_walk(2, available, kenkenptr->grid);
		if(list != NULL){
			struct node_ctr *next_ele = (struct node_ctr *)malloc(sizeof(struct node_ctr));
			if(next_ele == NULL) return 0;
			create_constraint(r_assign_op(2, list->entry, list->next_node->entry), list, &(next_ele->constraint));
			next_ele->next_node = dummy_ptr;
			dummy_ptr = next_ele;
		}
		counter++;
	}
	while(1){
		list = random_square_walk(1, available, kenkenptr->grid);
		if(list != NULL){
			struct node_ctr *next_ele = (struct node_ctr *)malloc(sizeof(struct node_ctr));
			if(next_ele == NULL) return 0;
			create_constraint(r_assign_op(1, 4, 5), list, &(next_ele->constraint));
			next_ele->next_node = dummy_ptr;
			dummy_ptr = next_ele;
			if(available[0][0] == 6 || available[0][1] == 6) break;
		}
	}
	// **head = *dummy_ptr;
	kenkenptr->ctrs = dummy_ptr;
	//printf("final return list address and assigned address %p, %p\n", dummy_ptr, &(kenkenptr->ctrs));
	return 1;
}

struct node_square *random_square_walk(int length, int arr[36][2], int kenken[6][6]){
	//printf("random square walk called\n");
	struct node_square *head;
	int dmy_arr[36][2];
	for(int i = 0; i < 36; i++){
		for(int j = 0; j < 2; j++){
			dmy_arr[i][j] = arr[i][j];
		}
	} //used to reset available in the event that the function call fails
	for(int l = 0; l < length; l++){ 
		struct node_square *sqr_ptr = (struct node_square *)malloc(sizeof(struct node_square));
		if(sqr_ptr == NULL){
			printf("malloc failure\n");//dynamically allocated -  must be freed manually later when the kenken is destroyed
			return NULL;
		}
		int arr2[4][2];
		if(l == 0){
			int dmy[2] = {6, 6};
			int (*sqr)[2] = random_available_sqr(arr, dmy, arr2);
			//printf("after random available called\n");
			if(sqr == NULL) {
				free(sqr_ptr);
				for(int i = 0; i < 36; i++){
					for(int j = 0; j < 2; j++){
						arr[i][j] = dmy_arr[i][j];
					}
				}
				return NULL;
			}
			sqr_ptr->pos[0] = (*sqr)[0];
			sqr_ptr->pos[1] = (*sqr)[1];
			sqr_ptr->entry = kenken[(*sqr)[0]][(*sqr)[1]];
			sqr_ptr->next_node = NULL;
			head = sqr_ptr;
			remove_available_sqr(arr, *sqr);
		}
		else{
			int (*sqr)[2] = random_available_sqr(arr, head->pos, arr2);
			struct node_square *dummy_ptr = head->next_node;
			while(sqr == NULL && dummy_ptr != NULL){
				sqr = random_available_sqr(arr, dummy_ptr->pos, arr2);
				dummy_ptr = dummy_ptr->next_node;
			}
			if(sqr == NULL){
				free(sqr_ptr);
				for(int i = 0; i < 36; i++){
					for(int j = 0; j < 2; j++){
						arr[i][j] = dmy_arr[i][j];
					}
				}
				for(struct node_square *dlt_ptr = head; dlt_ptr != NULL; ){
					struct node_square *dummy_ptr = dlt_ptr;
					dlt_ptr = dlt_ptr->next_node;
					free(dummy_ptr);
				}
				return NULL;
			}
			else{
				sqr_ptr->pos[0] = (*sqr)[0];
				sqr_ptr->pos[1] = (*sqr)[1];
				sqr_ptr->entry = kenken[(*sqr)[0]][(*sqr)[1]];
				sqr_ptr->next_node = head;
				head = sqr_ptr;
				remove_available_sqr(arr, *sqr);
			}
		}
		
	}
	//for(struct node_square *k = head; k != NULL; k = k->next_node){
	//	printf("%d %d %d\n", k->pos[0], k->pos[1], k->entry);
	//}
	
	//printf("random square walk returns %p\n", head);
	return head;
}

int create_constraint(int oper, struct node_square *ctr_sqrs, struct constraint *ctr){
	//THIS FUNCTION DOES NOT HANDLE LOGIC, AND ASSUMES IT RECEIVES LISTS OF THE CORRECT LENGTHS AND CORRESPONDING OPERATIONS
	//printf("create constraint called\n");
	ctr->op = oper;
	ctr->numbers = ctr_sqrs;
	if(oper == ADDOP){		//add
		int test_res = 0;
		struct node_square *a_ptr;
		for(a_ptr = ctr_sqrs; a_ptr != NULL; a_ptr = a_ptr->next_node){
			test_res += (a_ptr->entry);
		}
		ctr->result = test_res;
	}
	if(oper == SUBOP){		//sub
		int a = ctr->numbers->entry;
		int b = (ctr->numbers->next_node)->entry;
		if(a == b) return 0; //This should never occur
		if(a > b){
			ctr->result = (a-b);
		}
		if(b > a){
			ctr->result = (b-a);
		}
	}
	if(oper == MULTOP){		//mult
		int test_res = 1;
		struct node_square *a_ptr;
		for(a_ptr = ctr_sqrs; a_ptr != NULL; a_ptr = a_ptr->next_node){
			test_res = test_res*(a_ptr->entry);
		}
		ctr->result = test_res;
		
	}
	if(oper == DIVOP){		//div NOTE: Must check in the fucntion controlling logic that the two numbers yield a mod of 0
		int a = (ctr->numbers)->entry;
		int b = ((ctr->numbers)->next_node)->entry;
		if(a == b) return 0; //This should never occur
		if(a > b){
			ctr->result = a/b;
		}
		if(b > a){
			ctr->result = b/a;
		}
	}
	//no op
	if(oper == NOOP){
		ctr->result = ctr->numbers->entry;
	}
	if(oper > 4 || oper < 0) return 0;
	return 1;
}

int r_assign_op(int length, int arg1, int arg2){
	//srand(time(NULL));
	if(length == 1) return NOOP;
	if(length == 2 && (arg1 % arg2 == 0 || arg2 % arg1 == 0)) return rand() % 4; //assumes a 2 square cannot have two of the same number
	else if(length == 2) return rand() % 3;
	else return rand() % 2; //returns ADDOP or MULTOP
}

int (*random_available_sqr(int available[36][2], int sqr[2], int arr[4][2]))[2]{
	//srand(time(NULL));
	if(sqr[0] == 6 || sqr[1] == 6){
		int i = 0;
		for(int k = 0; available[k][0] != 6 && available[k][1] != 6 && k < 36; k++) i++;
		if(i==0) return NULL;
		if(i == 1) return &available[0];
		int index = rand() % (i);
		//printf("rand1 returned this i:%d index:%d, content is %d %d\n", i, index, available[index][0], available[index][1]);
		return &available[index];
	}
	int jctr;
	jctr = 0;
	for(int k = 0; available[k][0] != 6 && available[k][1] != 6 && k < 36; k++){
		if((available[k][0] == sqr[0] && available[k][1] == (sqr[1]+1)) || (available[k][0] == sqr[0] && available[k][1] == (sqr[1]-1)) || (available[k][0] == (sqr[0]+1) && available[k][1] == sqr[1]) || (available[k][0] == (sqr[0]-1) && available[k][1] == sqr[1])){
			//the above checks if an adjacent square is available
			arr[jctr][0] = available[k][0];
			arr[jctr][1] = available[k][1];
			jctr++;
		}
	}
	if(jctr == 0){
		//printf("no available squares\n");
		return NULL;
	}
	else{
		for(int l = jctr; l < 4; l++){
			arr[l][0] = 6;
			arr[l][1] = 6;
		}
		if(jctr == 1) return &arr[0];
		//printf("rand2 attempted\n");
		int index = rand() % (jctr);
		//printf("rand2 returned this j:%d index:%d, content is %d %d\n", jctr, index, arr[index][0], arr[index][1]);
		return &arr[index];
	}
	//printf("j is %d\n", j);
	/*for(j; j < 4; j++){
	//	arr[j][0] = 6;
	//	arr[j][1] = 6;
	//}
	if(arr[0][0] != 6 && arr[0][1] != 6){
		int i = 0;
		for(int (*k)[2] = &arr[0]; *k[0] != 6 && *k[1] != 6 && k < &arr[4]; k++) i++;
		printf("i is %d\n", i);
		if(i == 1) return &arr[0];
		printf("rand2 attempted\n");
		int index = rand() % (i);
		printf("rand2 returned this i:%d index:%d\n", i, index);
		return &arr[index];
	}
	else{
		printf("random assignment failed\n");
		return NULL;
	}*/
}

int remove_available_sqr(int available[36][2], int sqr[2]){
	//printf("remove available called with arg %d %d\nAvailable is\n", sqr[0], sqr[1]);
	for(int j = 0; j < 36; j++){
	//	printf("(%d, %d)", available[j][0], available[j][1]);
	}
	//printf("\n");
	for(int k = 0; k < 2; k++) if(sqr[k] < 0 || sqr[k] > 5) return 0;
	int i = 0;
	while(available[i][0] != 6 && available[i][1] != 6 && !(available[i][0] == sqr[0] && available[i][1] == sqr[1]) && i < 36){
		i++;
	}
	//printf("%d\n", i);
	if(available[i][0] == 6 || available[i][1] == 6 || i == 36) {
		//printf("remove available returned 0\n");
		return 0; //not in array
	}
	for(int j = i; j < 35; j++){
		available[j][0] = available[j+1][0];
		available[j][1] = available[j+1][1];
	}
	available[35][0] = 6;
	available[35][1] = 6;
	//printf("remove available returned 1\n");
	return 1; //success
}

int valid_partial_kenken(struct kenken kenken){
	
}

//int check
/*
void append_node(struct node_square *init, TYPE e){
	if((*init).next_node == NULL){
		struct node_square *nex_ele = (struct node_square*) malloc(sizeof(struct node_square));
		nex_ele->data = e;
		nex_ele->next_node = NULL;
		init->next_node = nex_ele;
		return;
	}
	else{
		append_node((*init).next_node, e);
	}
}

void prepend_node(struct node_square *ptr, TYPE e, struct node_square **ptrptr){
	struct node_square *new_1st_ele = (struct node_square*) malloc(sizeof(struct node_square));
	new_1st_ele->data = e;
	new_1st_ele->next_node = ptr;
	*ptrptr = new_1st_ele;
	return;
}
*/
int list_length(struct node_square *dummy_ptr){
	int length = 0;
	while(dummy_ptr!=NULL){
				length++;
				dummy_ptr = dummy_ptr->next_node;
	}
	return length;
}
/*
TYPE access_node (struct node_square *ptr, int n){
	if(n > list_length(ptr)||n < 1){
		printf("Out of bounds\n");
		return '0';
	}
	struct node_square *dummy_ptr = ptr;
	for(int i = 1; i < n; i++){
		dummy_ptr = dummy_ptr->next_node;
	}
	return dummy_ptr->data;
}

void remove_node(struct node_square *ptr, int n, struct node_square **ptrptr){
	if(n > list_length(ptr) || n < 1){
		printf("Out of bounds\n");
		return;
	}
	struct node_square *dummy_ptr = ptr;
	if(n == 1){
		*ptrptr = ptr->next_node;
		free(dummy_ptr);
		return;
	}
	for(int i = 1; i < (n - 1); i++){
		dummy_ptr = dummy_ptr->next_node;
	}
	struct node_square *delete_ptr = dummy_ptr->next_node;
	dummy_ptr->next_node = delete_ptr->next_node;
	free(delete_ptr);
	return;
}
*/