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
//solutions. **UNWRITTEN

int valid_partial_kenken(struct kenken kenken);
//Return 1 if a kenken is correct so far, or 0 if there are no solutions for the given state. 
//**UNTESTED

int valid_partial_grid(int arr[6][6]);
//Return 1 if the grid has no invalid partial rows or columns **UNTESTED
//helper for valid_partial_kenken

int valid_partial_constraint(struct constraint *ptr);
//return 1 if all constraints have valid entries, 0 otherwise **UNTESTED
//helper for valid_partial_kenken

/******************************************

Functions below

*******************************************/

                 
int valid_constraint(struct constraint *ptr){
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
	for(struct node_ctr *d_ptr = kenken_ptr->ctrs; d_ptr != 0; d_ptr = d_ptr->next_node){
		if(!valid_constraint(&(d_ptr->constraint))){
			printf("Invalid constraint\n");
			return 0;
		}
	}
	//check each square occurs exactly once in constraint list,
	//and that that square's entry matches the entry in the constraint
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 6; j++){
			int n_sqr = 0; //no. of occurences of square
			for(struct node_ctr *d2_ptr = kenken_ptr->ctrs; d2_ptr != 0; d2_ptr = d2_ptr->next_node){
				//walk through constraint list
				for(struct node_square *d1_ptr = (d2_ptr->constraint).numbers; d1_ptr != NULL; d1_ptr = d1_ptr->next_node){ //walk through constraint numbers
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
	fill_square(0, 0, grid);
}

int random_available(int available[6]){ //picks a random number from a list, padded by 0's at the end
	int i = 0;
	for(int *k = &available[0]; *k != 0 && k < &available[6]; k++) i++;
	if(i == 1) return available[0];
	int index = rand() % (i);
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
	int available[6] = {1,2,3,4,5,6};
	for(int k = 0; k < j; k++){ //eliminates any numbers already in the row 
		int sqr = arr[i][k];
		remove_available(available, sqr);
	}
	for(int k = 0; k < i; k++){ //eliminates any numbers already in the column
		int sqr = arr[k][j];
		remove_available(available, sqr);
	}
	while(available[0] != 0){ //randomly select a number which hasn't been taken
		arr[i][j] = random_available(available);
		if(i == 5 && j == 5){
			return 1;
		}
		if(fill_square((i+(j+1)/6),(j+1)%6,arr) == 0){ 
			//checks if the next square successfully returns, and if it doesn't, removes its 
			//selection and picks another number
			remove_available(available, arr[i][j]);
		}
		else{
			return 1;
		}
	}
	return 0;
}

int generate_kenken(struct kenken *kenkenptr){
	srand(time(NULL));
	r_fill_grid(kenkenptr->grid);
	int x = generate_constraints(kenkenptr);
	return x;
}

int destroy_kenken(struct kenken *kenkenptr){
	//walk through constraints
	for(struct node_ctr *dmy_ptr = kenkenptr->ctrs; dmy_ptr != NULL;){
		//walk through node_square s, freeing them
		for(struct node_square *dmysqrptr = dmy_ptr->constraint.numbers; dmysqrptr != NULL;){
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
	for(int i = 0; i < 36; i++){ //creates list of avaiable squares for a constraint
		available[i][0] = i/6;
		available[i][1] = i%6;
	}
	struct node_ctr *dummy_ptr;
	
	//creates constraint of 4 squares
	struct node_square *list = random_square_walk(4, available, kenkenptr->grid);
	if(list != NULL){
		struct node_ctr *fst_ele = (struct node_ctr *)malloc(sizeof(struct node_ctr));
		if(fst_ele == NULL) return 0;
		create_constraint(r_assign_op(4, 4, 5), list, (&(fst_ele->constraint)));
		fst_ele->next_node = NULL;
		dummy_ptr = fst_ele;
	}
	
	//creates up to 2 constraints of 3 squares (this can fail)
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
	
	//creates up to 15 constraints of length 2 (this can create less due to random failure)
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
	
	//populate remaining squares with singleton cages
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
	
	
	kenkenptr->ctrs = dummy_ptr;
	
	return 1;
}

struct node_square *random_square_walk(int length, int arr[36][2], int kenken[6][6]){
	
	struct node_square *head;
	int dmy_arr[36][2];
	for(int i = 0; i < 36; i++){
		for(int j = 0; j < 2; j++){
			dmy_arr[i][j] = arr[i][j];
		}
	} //used to reset available list in the event that the function call fails
	
	for(int l = 0; l < length; l++){ 
		struct node_square *sqr_ptr = (struct node_square *)malloc(sizeof(struct node_square));
		if(sqr_ptr == NULL){
			printf("malloc failure\n");//dynamically allocated -  must be freed manually later when the kenken is destroyed
			return NULL;
		}
		int arr2[4][2];
		if(l == 0){ //randomly selects first square of the walk
			int dmy[2] = {6, 6};
			int (*sqr)[2] = random_available_sqr(arr, dmy, arr2);
			if(sqr == NULL) { //if random avaiable fails - reset function
				free(sqr_ptr);
				for(int i = 0; i < 36; i++){
					for(int j = 0; j < 2; j++){
						arr[i][j] = dmy_arr[i][j];
					}
				}
				return NULL;
			}
			
			//creates last element of list of squares
			sqr_ptr->pos[0] = (*sqr)[0];
			sqr_ptr->pos[1] = (*sqr)[1];
			sqr_ptr->entry = kenken[(*sqr)[0]][(*sqr)[1]];
			sqr_ptr->next_node = NULL;
			head = sqr_ptr;
			remove_available_sqr(arr, *sqr);
		}
		
		
		else{ //randomly selects from adjacent squares
			int (*sqr)[2] = random_available_sqr(arr, head->pos, arr2);
			struct node_square *dummy_ptr = head->next_node;
			
			//walks through existing squares, finds one adjacent to the one stored by dummy_ptr
			while(sqr == NULL && dummy_ptr != NULL){
				sqr = random_available_sqr(arr, dummy_ptr->pos, arr2);
				dummy_ptr = dummy_ptr->next_node;
			}
			//if no adjacent squares available
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
			
			//prepends square to list
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
	
	return head;
}

int create_constraint(int oper, struct node_square *ctr_sqrs, struct constraint *ctr){
	//THIS FUNCTION DOES NOT HANDLE LOGIC, AND ASSUMES IT RECEIVES LISTS OF THE CORRECT LENGTHS AND CORRESPONDING OPERATIONS
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
	
	if(sqr[0] == 6 || sqr[1] == 6){ //randomly selects any square from the list available
		int i = 0;
		for(int k = 0; available[k][0] != 6 && available[k][1] != 6 && k < 36; k++) i++;
		if(i==0) return NULL;
		if(i == 1) return &available[0];
		int index = rand() % (i);
		return &available[index];
	}
	int jctr;
	jctr = 0;
	
	//this populates the list of adjacent squares in the event that an initial square is passed to the function
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
		//populates end of array with NIL values (6)
		for(int l = jctr; l < 4; l++){
			arr[l][0] = 6;
			arr[l][1] = 6;
		}
		//randomly selects from the list of available adjacent squares
		if(jctr == 1) return &arr[0];
		int index = rand() % (jctr);
		return &arr[index];
	}
}

int remove_available_sqr(int available[36][2], int sqr[2]){
	//check for valid input
	for(int k = 0; k < 2; k++) if(sqr[k] < 0 || sqr[k] > 5) return 0;
	
	int i = 0;
	//searches for element to remove
	while(available[i][0] != 6 && available[i][1] != 6 && !(available[i][0] == sqr[0] && available[i][1] == sqr[1]) && i < 36){
		i++;
	}
	if(available[i][0] == 6 || available[i][1] == 6 || i == 36) {
		return 0; //not in array
	}
	for(int j = i; j < 35; j++){ //deletes element to be removed, pads array with 6's
		available[j][0] = available[j+1][0];
		available[j][1] = available[j+1][1];
	}
	available[35][0] = 6;
	available[35][1] = 6;
	return 1; //success
}

int valid_partial_kenken(struct kenken kenken){
	if(!valid_partial_grid(kenken.grid)) return 0;
	for(struct node_ctr *d_ptr = kenken.ctrs; d_ptr != 0; d_ptr = d_ptr->next_node){
		if(!valid_partial_constraint(&(d_ptr->constraint))){
			//printf("Invalid constraint\n");
			return 0;
		}
	}
	return 1;
}

int valid_partial_grid(int arr[6][6]){
	for(int cellcont = 1; cellcont < 7; cellcont++){
		//check rows
		for(int rownum = 0; rownum < 6; rownum++){
			int num_num = 0;
			for(int colnum = 0; colnum < 6; colnum++){
				if(arr[rownum][colnum] == cellcont) num_num++;
			}
			if(num_num > 1) return 0;
		}
		//check columns
		for(int colnum = 0; colnum < 6; colnum++){
			int num_num = 0;
			for(int rownum = 0; rownum < 6; rownum++){
				if(arr[rownum][colnum] == cellcont) num_num++;
			}
			if(num_num > 1) return 0;
		}
	}
	return 1;
}

//testing github                
int valid_partial_constraint(struct constraint *ptr){
	//check for valid value of operation
	if((ptr->op) < 0 || (ptr->op) > 4) return 0;
	//check result of addition
	if(ptr->op == ADDOP){
		if(list_length(ptr->numbers) <= 1) return 0;
		int test_res = 0;
		int zero_eles = 0;
		struct node_square *a_ptr;
		for(a_ptr = ptr->numbers; a_ptr != NULL; a_ptr = a_ptr->next_node){
			test_res += (a_ptr->entry);
			if(a_ptr->entry == 0) zero_eles++;
		}
		if(zero_eles == 0 && test_res != ptr->result) return 0;
		if(zero_eles > 0 && (test_res > ptr->result - zero_eles || test_res < ptr->result - 6*zero_eles)) return 0;
	}
	//check correct number of arguments for subtraction
	if(ptr->op == SUBOP){
		if(list_length(ptr->numbers) != 2) return 0;
		int a = (ptr->numbers)->entry;
		int b = ((ptr->numbers)->next_node)->entry;
		if(a == b && a != 0) return 0;
		if(a > b){
			if((a-b) != ptr->result && b != 0) return 0;
		}
		if(b > a){
			if((b-a) != ptr->result && a != 0) return 0;
		}
	}
	//check result of multiplication
	if(ptr->op == MULTOP){
		if(list_length(ptr->numbers) <= 1) return 0;
		int test_res = 1;
		int zero_eles = 0;
		struct node_square *a_ptr;
		for(a_ptr = ptr->numbers; a_ptr != NULL; a_ptr = a_ptr->next_node){
			if(a_ptr->entry != 0) test_res = test_res*(a_ptr->entry);
			else zero_eles++;
		}
		if(zero_eles == 0 && test_res != ptr->result) return 0;
		else if(zero_eles < list_length(ptr->numbers) && ptr->result % test_res != 0) return 0;
	}
	//check mod 0 and number of arguments for division
	if(ptr->op == DIVOP){
		if(list_length(ptr->numbers) != 2) return 0;
		int a = (ptr->numbers)->entry;
		int b = ((ptr->numbers)->next_node)->entry;
		if(a == b && b != 0) return 0;
		if(a > b && b != 0){
			if((a/b) != ptr->result || a%b != 0) return 0;
		}
		if(b > a && a != 0){
			if((b/a) != ptr->result || b%a != 0) return 0;
		}
		if(a == 0 && b != 0){
			if(ptr->result * b > 6 && b % ptr->result) return 0;
		}
		if(b == 0 && a != 0){
			if(ptr->result * a > 6 && a % ptr->result) return 0;
		}
	}
	//check value of result given op
	if(ptr->op == NOOP){
		if(list_length(ptr->numbers) != 1){
			printf("list too long\n");
			return 0;
		}
		else if((ptr->result != (ptr->numbers)->entry) && ((ptr->numbers)->entry != 0)){
			printf("unequal values\n");
			return 0;
		}
		else return 1;
	}
	return 1;
}


int list_length(struct node_square *dummy_ptr){
	int length = 0;
	while(dummy_ptr!=NULL){
				length++;
				dummy_ptr = dummy_ptr->next_node;
	}
	return length;
}