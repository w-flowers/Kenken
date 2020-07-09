#include"kenken-basic.h"

/******************************************

Functions below

*******************************************/

                 
int valid_constraint( struct constraint *ptr ){
	
	
	//check for valid value of operation
	if( ( ptr->op ) < 0 || ( ptr->op ) > 4 ) return 0;
	
	//check result of addition
	if( ptr->op == ADDOP ){
		
		if( list_length( ptr->numbers ) <= 1 ) return 0;
		
		int test_res = 0;
		
		struct node_square *a_ptr;
		
		for( a_ptr = ptr->numbers; a_ptr != NULL; a_ptr = a_ptr->next_node ){
			
			test_res += ( a_ptr->entry );
			
		}
		
		if( test_res != ptr->result ) return 0;
		
	}
	
	//check correct number of arguments for subtraction
	if( ptr->op == SUBOP ){
		
		if( list_length( ptr->numbers ) != 2 ) return 0;
		
		int a = ( ptr->numbers )->entry;
		
		int b = ( ( ptr->numbers )->next_node )->entry;
		
		if( a == b ) return 0;
		
		if( a > b ){
			
			if( ( a-b ) != ptr->result ) return 0;
			
		}
		
		if( b > a ){
			
			if( ( b-a ) != ptr->result ) return 0;
			
		}
		
	}
	
	//check result of multiplication
	if( ptr->op == MULTOP ){
		
		if( list_length( ptr->numbers ) <= 1 ) return 0;
		
		int test_res = 1;
		
		struct node_square *a_ptr;
		
		for( a_ptr = ptr->numbers; a_ptr != NULL; a_ptr = a_ptr->next_node ){
			
			test_res = test_res*( a_ptr->entry );
			
		}
		
		if( test_res != ptr->result ) return 0;
		
	}
	//check mod 0 and number of arguments for division
	if( ptr->op == DIVOP ){
		
		if( list_length( ptr->numbers ) != 2 ) return 0;
		
		int a = ( ptr->numbers )->entry;
		
		int b = ( ( ptr->numbers )->next_node )->entry;
		
		if( a == b || a <= 0 || b <= 0 ) return 0;
		
		if( a > b ){
			
			if( ( a/b ) != ptr->result || a%b != 0 ) return 0;
			
		}
		
		if( b > a ){
			
			if( ( b/a ) != ptr->result || b%a != 0 ) return 0;
			
		}
		
	}
	
	//check value of result given op
	if( ptr->op == NOOP ){
		
		if( list_length( ptr->numbers ) != 1 ){
			
			//printf( "list too long\n" );
			return 0;
			
		}
		
		else if( ptr->result != ( ptr->numbers )->entry ){
			
			//printf( "unequal values\n" );
			return 0;
			
		}
		
		else return 1;
		
	}
	
	//check position of squares for adjacency to at least 1 other square
	struct node_square *b_ptr;
	
	for( b_ptr = ptr->numbers; b_ptr != NULL; b_ptr = b_ptr->next_node ){
		
		struct node_square *c_ptr;
		
		int has_adj_sqrs = 0;
		
		for( c_ptr = ptr->numbers; c_ptr != NULL; c_ptr = c_ptr->next_node ){
			
			if( ( abs( ( b_ptr->pos[0] )-( c_ptr->pos[0] ) )+abs( ( b_ptr->pos[1] )-( c_ptr->pos[1] ) ) ) == 1 ) has_adj_sqrs = 1; 
			
		}
		
		if( !has_adj_sqrs ) return 0;
		
	}
	
	return 1;
	
}



int valid_grid( int arr[6][6] ){
	
	for( int cellcont = 1; cellcont < 7; cellcont++ ){
		
		//check rows
		for( int rownum = 0; rownum < 6; rownum++ ){
			
			int num_num = 0;
			
			for( int colnum = 0; colnum < 6; colnum++ ){
				
				if( arr[rownum][colnum] == cellcont ) num_num++;
				
			}
			
			if( num_num != 1 ) return 0;
			
		}
		
		//check columns
		for( int colnum = 0; colnum < 6; colnum++ ){
			
			int num_num = 0;
			
			for( int rownum = 0; rownum < 6; rownum++ ){
				
				if( arr[rownum][colnum] == cellcont ) num_num++;
				
			}
			
			if( num_num != 1 ) return 0;
			
		}
		
	}
	
	return 1;
	
}


int kenken_invalid( struct kenken *kenken_ptr ){
	
	if( !valid_grid( kenken_ptr->grid ) ) return 0;
	
	for( struct node_ctr *d_ptr = kenken_ptr->ctrs; d_ptr != 0; d_ptr = d_ptr->next_node ){
		
		if( !valid_constraint( &( d_ptr->constraint ) ) ){
			
			//printf( "Invalid constraint\n" );
			return 1;
			
		}
		
	}
	
	//check each square occurs exactly once in constraint list,
	//and that that square's entry matches the entry in the constraint
	for( int i = 0; i < 6; i++ ){
		
		for( int j = 0; j < 6; j++ ){
			
			int n_sqr = 0; //no. of occurences of square
			
			for( struct node_ctr *d2_ptr = kenken_ptr->ctrs; d2_ptr != 0; 
				d2_ptr = d2_ptr->next_node ){
				
				//walk through constraint list
				for( struct node_square *d1_ptr = ( d2_ptr->constraint ).numbers; d1_ptr != NULL;
					 d1_ptr = d1_ptr->next_node ){ //walk through constraint numbers
					
					if( d1_ptr->pos[0] == i && d1_ptr->pos[1] == j ){
						
						if( d1_ptr->entry != kenken_ptr->grid[i][j] ){
							
							 //printf( "Wrong element at square\n" );
							 return 2;
							 
						 }
						 
						else n_sqr++;
						
					}
					
				}
				
			}
			
			if( n_sqr != 1 ) {
				
				//printf( "wrong number of squares\n" );
				return 3;
				
			}
			
		}
		
	}
	
	return 0;
	
}

void r_fill_grid( int grid[6][6] ){
	
	fill_square( 0, 0, grid );
	
}

int random_available( int available[6] ){ 
	
	//picks a random number from a list, padded by 0's at the end
	
	int i = 0;
	
	for( int *k = &available[0]; *k != 0 && k < &available[6]; k++ ) i++;
	
	if( i == 1 ) return available[0];
	
	if( i == 0 ) return 0;
	
	int index = rand(  ) % ( i );
	
	return available[index];
	
}

int remove_available( int* available, int number ){
	
	if( number < 1 || number > 6 ) return 0;
	
	int i = 0;
	
	while( *( available+i ) != number && i < 6 ) i++;
	
	if( i == 6 ) return 0; //not in array
	
	for( int j = i; j < 5; j++ ) *( available + j ) = *( available + j + 1 );
	
	*( available+5 ) = 0;
	
	return 1; //success
	
}

int fill_square( int i, int j, int arr[6][6] ){
	
	int available[6] = { 1 , 2 , 3 , 4 , 5 , 6 };
	
	for( int k = 0; k < j; k++ ){ //eliminates any numbers already in the row 
		
		int sqr = arr[i][k];
		
		remove_available( available, sqr );
		
	}
	
	for( int k = 0; k < i; k++ ){ //eliminates any numbers already in the column
		
		
		int sqr = arr[k][j];
		
		remove_available( available, sqr );
		
	}
	
	while( available[0] != 0 ){ //randomly select a number which hasn't been taken
	
		arr[i][j] = random_available( available );
		
		if( i == 5 && j == 5 ){
			
			return 1;
			
		}
		
		if( fill_square( ( i+( j+1 )/6 ),( j+1 ) % 6 , arr ) == 0 ){ 
			
			//checks if the next square successfully returns, and if it doesn't, removes its 
			//selection and picks another number
			
			remove_available( available, arr[i][j] );
			
		}
		
		else{
			
			return 1;
			
		}
		
	}
	
	return 0;
	
}

int generate_kenken( struct kenken *kenkenptr ){
	
	//srand( time( NULL ) );
	r_fill_grid( kenkenptr->grid );
	
	//printf( "Filled the grid\n" );
	int x = generate_constraints( kenkenptr );
	
	//printf( "generated the constraints\n" );
	return x;
}

int destroy_kenken( struct kenken *kenkenptr ){
	
	//walk through constraints
	for( struct node_ctr *dmy_ptr = kenkenptr->ctrs; dmy_ptr != NULL; ){
		
		//walk through node_square s, freeing them
		for( struct node_square *dmysqrptr = dmy_ptr->constraint.numbers; dmysqrptr != NULL; ){
			
			struct node_square *dltsqrptr = dmysqrptr;
			
			dmysqrptr = dmysqrptr->next_node;
			
			free( dltsqrptr );
			
		}
		
		//free constraint, then next constraint
		struct node_ctr *dltctrptr = dmy_ptr;
		
		dmy_ptr = dmy_ptr->next_node;
		
		free( dltctrptr );
		
	}
	
	return 1;
	
}

int generate_constraints( struct kenken *kenkenptr ){
	
	//printf( "entered gc\n" );
	int available[36][2];
	
	for( int i = 0; i < 36; i++ ){ //creates list of avaiable squares for a constraint
		
		available[i][0] = i / 6;
		
		available[i][1] = i % 6;
		
	}
	
	struct node_ctr *dummy_ptr;
	
	
	//creates constraint of 4 squares
	struct node_square *list = random_square_walk( 4, available, kenkenptr->grid );
	
	if( list != NULL ){
		
		struct node_ctr *fst_ele = ( struct node_ctr * )malloc( sizeof( struct node_ctr ) );
		
		if( fst_ele == NULL ) return 0;
		
		create_constraint( r_assign_op( 4, 4, 5 ), list, ( &( fst_ele->constraint ) ) );
		
		fst_ele->next_node = NULL;
		
		dummy_ptr = fst_ele;
		
	}
	
	
	//creates up to 2 constraints of 3 squares ( this can fail )
	int counter = 0;
	
	while( counter < 3 ){
		
		list = random_square_walk( 3, available, kenkenptr->grid );
		
		if( list != NULL ){
			
			struct node_ctr *next_ele = ( struct node_ctr * )malloc( sizeof( struct node_ctr ) );
			
			if( next_ele == NULL ) return 0;
			
			create_constraint( r_assign_op( 3, 4, 5 ), list, &( next_ele->constraint ) );
			
			next_ele->next_node = dummy_ptr;
			
			dummy_ptr = next_ele;
			
			//printf( "3 square, head, %p, tail\n", dummy_ptr );
			
		}
		
		counter++;
		
	}
	
	
	//creates up to 15 constraints of length 2 ( this can create less due to random failure )
	counter = 0;
	
	while( counter < 15 ){
		
		list = random_square_walk( 2, available, kenkenptr->grid );
		
		if( list != NULL ){
			
			struct node_ctr *next_ele = ( struct node_ctr * )malloc( sizeof( struct node_ctr ) );
			
			if( next_ele == NULL ) return 0;
			
			create_constraint( r_assign_op( 2, list->entry, list->next_node->entry ), 
				list, &( next_ele->constraint ) );
			
			next_ele->next_node = dummy_ptr;
			
			dummy_ptr = next_ele;
			
		}
		
		counter++;
		
	}
	
	
	//populate remaining squares with singleton cages
	while( 1 ){
		
		list = random_square_walk( 1, available, kenkenptr->grid );
		
		if( list != NULL ){
			
			struct node_ctr *next_ele = ( struct node_ctr * )malloc( sizeof( struct node_ctr ) );
			
			if( next_ele == NULL ) return 0;
			
			create_constraint( r_assign_op( 1, 4, 5 ), list, &( next_ele->constraint ) );
			
			next_ele->next_node = dummy_ptr;
			
			dummy_ptr = next_ele;
			
			//if( available[0][0] == 6 || available[0][1] == 6 ) break;
			
		}
		
		if( available[0][0] == 6 || available[0][1] == 6 ) break;
		
	}
	
	
	
	kenkenptr->ctrs = dummy_ptr;
	
	return 1;
	
}


struct node_square *random_square_walk( int length, int arr[36][2], int kenken[6][6] ){
	
	//printf( "entered random squarewalk\n" );
	struct node_square *head;
	
	int dmy_arr[36][2];
	
	for( int i = 0; i < 36; i++ ){
		
		for( int j = 0; j < 2; j++ ){
			
			dmy_arr[i][j] = arr[i][j];
			
		}
		
	} //used to reset available list in the event that the function call fails
	
	
	for( int l = 0; l < length; l++ ){ 
		
		struct node_square *sqr_ptr = ( struct node_square * )malloc( sizeof( struct node_square ) );
		
		if( sqr_ptr == NULL ){
			
			printf( "malloc failure\n" );//dynamically allocated -  must be freed manually later when the kenken is destroyed
			
			return NULL;
			
		}
		
		int arr2[4][2];
		
		if( l == 0 ){ //randomly selects first square of the walk
			
			int dmy[2] = {6, 6};
			
			int ( *sqr )[2] = random_available_sqr( arr, dmy, arr2 );
			
			if( sqr == NULL ) { //if random avaiable fails - reset function
				
				free( sqr_ptr );
				
				for( int i = 0; i < 36; i++ ){
					
					for( int j = 0; j < 2; j++ ){
						
						arr[i][j] = dmy_arr[i][j];
						
					}
					
				}
				
				return NULL;
				
			}
			
			
			//creates last element of list of squares
			sqr_ptr->pos[0] = ( *sqr )[0];
			sqr_ptr->pos[1] = ( *sqr )[1];
			
			sqr_ptr->entry = kenken[( *sqr )[0]][( *sqr )[1]];
			
			sqr_ptr->next_node = NULL;
			
			head = sqr_ptr;
			
			remove_available_sqr( arr, *sqr );
			
		}
		
		
		else{ //randomly selects from adjacent squares
			
			int ( *sqr )[2] = random_available_sqr( arr, head->pos, arr2 );
			
			struct node_square *dummy_ptr = head->next_node;
			
			
			//walks through existing squares, finds one adjacent to the one stored by dummy_ptr
			while( sqr == NULL && dummy_ptr != NULL ){
				
				sqr = random_available_sqr( arr, dummy_ptr->pos, arr2 );
				
				dummy_ptr = dummy_ptr->next_node;
				
			}
			
			//if no adjacent squares available
			if( sqr == NULL ){
				
				free( sqr_ptr );
				
				for( int i = 0; i < 36; i++ ){
					
					for( int j = 0; j < 2; j++ ){
						
						arr[i][j] = dmy_arr[i][j];
						
					}
					
				}
				
				for( struct node_square *dlt_ptr = head; dlt_ptr != NULL;  ){
					
					struct node_square *dummy_ptr = dlt_ptr;
					
					dlt_ptr = dlt_ptr->next_node;
					
					free( dummy_ptr );
					
				}
				
				return NULL;
				
			}
			
			
			//prepends square to list
			else{
				
				sqr_ptr->pos[0] = ( *sqr )[0];
				
				sqr_ptr->pos[1] = ( *sqr )[1];
				
				sqr_ptr->entry = kenken[( *sqr )[0]][( *sqr )[1]];
				
				sqr_ptr->next_node = head;
				
				head = sqr_ptr;
				
				remove_available_sqr( arr, *sqr );
				
			}
			
		}
		
		
		
	}
	
	
	
	return head;
	
}


int create_constraint( int oper, struct node_square *ctr_sqrs, struct constraint *ctr ){
	
	//THIS FUNCTION DOES NOT HANDLE LOGIC, AND ASSUMES IT RECEIVES LISTS OF THE CORRECT LENGTHS AND CORRESPONDING OPERATIONS
	
	//printf( "entered create_constraint\n" );
	ctr->op = oper;
	ctr->numbers = ctr_sqrs;
	
	if( oper == ADDOP ){		//add
		
		int test_res = 0;
		
		struct node_square *a_ptr;
		
		for( a_ptr = ctr_sqrs; a_ptr != NULL; a_ptr = a_ptr->next_node ){
			
			test_res += ( a_ptr->entry );
			
		}
		
		ctr->result = test_res;
		
	}
	
	if( oper == SUBOP ){		//sub
		
		int a = ctr->numbers->entry;
		
		int b = ( ctr->numbers->next_node )->entry;
		
		if( a == b ) return 0; //This should never occur
		
		if( a > b ){
			
			ctr->result = ( a-b );
			
		}
		
		if( b > a ){
			
			ctr->result = ( b-a );
			
		}
		
	}
	
	if( oper == MULTOP ){		//mult
		
		int test_res = 1;
		
		struct node_square *a_ptr;
		
		for( a_ptr = ctr_sqrs; a_ptr != NULL; a_ptr = a_ptr->next_node ){
			
			test_res = test_res*( a_ptr->entry );
			
		}
		
		ctr->result = test_res;
		
	}
	
	if( oper == DIVOP ){		//div NOTE: Must check in the fucntion controlling logic that the two numbers yield a mod of 0
		
		int a = ( ctr->numbers )->entry;
		
		int b = ( ( ctr->numbers )->next_node )->entry;
		
		if( a == b ) return 0; //This should never occur
		
		if( a > b ){
			
			ctr->result = a/b;
			
		}
		
		if( b > a ){
			
			ctr->result = b/a;
			
		}
		
	}
	
	//no op
	if( oper == NOOP ){
		
		ctr->result = ctr->numbers->entry;
		
	}
	
	if( oper > 4 || oper < 0 ) return 0;
	
	return 1;
	
}


int r_assign_op( int length, int arg1, int arg2 ){
	
	//printf( "entered random assignop\n" );
	//srand( time( NULL ) );
	
	if( length == 1 ) return NOOP;
	
	if( length == 2 && ( arg1 % arg2 == 0 || arg2 % arg1 == 0 ) ) return rand(  ) % 4; 
	//assumes a 2 square cannot have two of the same number
	
	else if( length == 2 ) return rand(  ) % 3;
	
	else return rand(  ) % 2; //returns ADDOP or MULTOP
	
}


int ( *random_available_sqr( int available[36][2], int sqr[2], int arr[4][2] ) )[2]{
	
	if( sqr[0] == 6 || sqr[1] == 6 ){ //randomly selects any square from the list available
		
		int i = 0;
		
		for( int k = 0; available[k][0] != 6 && available[k][1] != 6 && k < 36; k++ ) i++;
		
		if( i==0 ) return NULL;
		
		if( i == 1 ) return &available[0];
		
		int index = rand(  ) % ( i );
		
		return &available[index];
		
	}
	
	int jctr;
	
	jctr = 0;
	
	
	//this populates the list of adjacent squares in the event that an initial square is passed to the function
	
	for( int k = 0; available[k][0] != 6 && available[k][1] != 6 && k < 36; k++ ){
		
		if( ( available[k][0] == sqr[0] && available[k][1] == ( sqr[1]+1 ) ) || 
			( available[k][0] == sqr[0] && available[k][1] == ( sqr[1]-1 ) ) || 
			( available[k][0] == ( sqr[0]+1 ) && available[k][1] == sqr[1] ) || 
			( available[k][0] == ( sqr[0]-1 ) && available[k][1] == sqr[1] ) ){
			
			//the above checks if an adjacent square is available
			
			arr[jctr][0] = available[k][0];
			
			arr[jctr][1] = available[k][1];
			
			jctr++;
			
		}
		
	}
	
	
	if( jctr == 0 ){
		
		//printf( "no available squares\n" );
		return NULL;
		
	}
	
	
	else{
		
		//populates end of array with NIL values ( 6 )
		for( int l = jctr; l < 4; l++ ){
			
			arr[l][0] = 6;
			arr[l][1] = 6;
			
		}
		
		//randomly selects from the list of available adjacent squares
		if( jctr == 1 ) return &arr[0];
		
		int index = rand(  ) % ( jctr );
		
		return &arr[index];
		
	}
	
}


int remove_available_sqr( int available[36][2], int sqr[2] ){
	
	//check for valid input
	for( int k = 0; k < 2; k++ ) if( sqr[k] < 0 || sqr[k] > 5 ) return 0;
	
	int i = 0;
	//searches for element to remove
	while( available[i][0] != 6 && available[i][1] != 6 && 
		!( available[i][0] == sqr[0] && available[i][1] == sqr[1] ) && i < 36 ){
		
		i++;
		
	}
	
	if( available[i][0] == 6 || available[i][1] == 6 || i == 36 ) {
		
		return 0; //not in array
		
	}
	
	for( int j = i; j < 35; j++ ){ //deletes element to be removed, pads array with 6's
	
		available[j][0] = available[j+1][0];
		available[j][1] = available[j+1][1];
		
	}
	
	available[35][0] = 6;
	available[35][1] = 6;
	
	return 1; //success
	
}


int valid_partial_kenken( struct kenken kenken ){
	
	if( !valid_partial_grid( kenken.grid ) ) return 0;
	
	for( struct node_ctr *d_ptr = kenken.ctrs; d_ptr != 0; d_ptr = d_ptr->next_node ){
		
		if( !valid_partial_constraint( &( d_ptr->constraint ) ) ){
			
			//printf( "Invalid constraint\n" );
			return 0;
			
		}
		
	}
	
	return 1;
	
}


int valid_partial_grid( int arr[6][6] ){
	
	for( int cellcont = 1; cellcont < 7; cellcont++ ){
		
		//check rows
		for( int rownum = 0; rownum < 6; rownum++ ){
			
			int num_num = 0;
			
			for( int colnum = 0; colnum < 6; colnum++ ){
				
				if( arr[rownum][colnum] == cellcont ) num_num++;
				
			}
			
			if( num_num > 1 ) return 0;
			
		}
		
		//check columns
		for( int colnum = 0; colnum < 6; colnum++ ){
			
			int num_num = 0;
			
			for( int rownum = 0; rownum < 6; rownum++ ){
				
				if( arr[rownum][colnum] == cellcont ) num_num++;
				
			}
			
			if( num_num > 1 ) return 0;
			
		}
		
	}
	
	return 1;
	
}


//testing github                
int valid_partial_constraint( struct constraint *ptr ){
	
	//check for valid value of operation
	if( ( ptr->op ) < 0 || ( ptr->op ) > 4 ) return 0;
	
	//check result of addition
	if( ptr->op == ADDOP ){
		
		if( list_length( ptr->numbers ) <= 1 ) return 0;
		
		int test_res = 0;
		
		int zero_eles = 0;
		
		struct node_square *a_ptr;
		
		for( a_ptr = ptr->numbers; a_ptr != NULL; a_ptr = a_ptr->next_node ){
			
			test_res += ( a_ptr->entry );
			
			if( a_ptr->entry == 0 ) zero_eles++;
			
		}
		
		if( zero_eles == 0 && test_res != ptr->result ) return 0;
		
		if( zero_eles > 0 && 
			( test_res > ( ptr->result - zero_eles ) || test_res < ( ptr->result - 6*zero_eles ) ) ) 
				return 0;
		
	}
	
	//check correct number of arguments for subtraction
	if( ptr->op == SUBOP ){
		
		if( list_length( ptr->numbers ) != 2 ) return 0;
		
		int a = ( ptr->numbers )->entry;
		
		int b = ( ( ptr->numbers )->next_node )->entry;
		
		if( a == b && a != 0 ) return 0;
		
		if( a > b ){
			
			if( ( a-b ) != ptr->result && b != 0 ) return 0;
			
		}
		
		if( b > a ){
			
			if( ( b-a ) != ptr->result && a != 0 ) return 0;
			
		}
		
	}
	
	//check result of multiplication
	if( ptr->op == MULTOP ){
		
		if( list_length( ptr->numbers ) <= 1 ) return 0;
		
		int test_res = 1;
		
		int zero_eles = 0;
		
		struct node_square *a_ptr;
		
		for( a_ptr = ptr->numbers; a_ptr != NULL; a_ptr = a_ptr->next_node ){
			
			if( a_ptr->entry != 0 ) test_res = test_res*( a_ptr->entry );
			
			else zero_eles++;
			
		}
		
		if( zero_eles == 0 && test_res != ptr->result ) return 0;
		
		else if( zero_eles < list_length( ptr->numbers ) && ptr->result % test_res != 0 ) return 0;
		
	}
	
	//check mod 0 and number of arguments for division
	if( ptr->op == DIVOP ){
		
		if( list_length( ptr->numbers ) != 2 ) return 0;
		
		int a = ( ptr->numbers )->entry;
		
		int b = ( ( ptr->numbers )->next_node )->entry;
		
		if( a == b && b != 0 ) return 0;
		
		if( a > b && b != 0 ){
			
			if( ( a/b ) != ptr->result || a%b != 0 ) return 0;
			
		}
		
		if( b > a && a != 0 ){
			
			if( ( b/a ) != ptr->result || b%a != 0 ) return 0;
			
		}
		
		if( a == 0 && b != 0 ){
			
			if( ptr->result * b > 6 && b % ptr->result != 0 ) return 0;
			
		}
		
		if( b == 0 && a != 0 ){
			
			if( ptr->result * a > 6 && a % ptr->result != 0 ) return 0;
			
		}
		
	}
	
	//check value of result given op
	if( ptr->op == NOOP ){
		
		if( list_length( ptr->numbers ) != 1 ){
			
			//printf( "list too long\n" );
			return 0;
			
		}
		
		else if( ( ptr->result != ( ptr->numbers )->entry ) && ( ( ptr->numbers )->entry != 0 ) ){
			
			//printf( "unequal values\n" );
			return 0;
			
		}
		
		else return 1;
		
	}
	
	return 1;
	
}


int list_length( struct node_square *dummy_ptr ){
	
	int length = 0;
	
	while( dummy_ptr!=NULL ){
		
				length++;
				
				dummy_ptr = dummy_ptr->next_node;
				
	}
	
	return length;
	
}


int copy_kenken( struct kenken *kkptr, struct kenken *newkkptr ){
	
	for( int j = 0; j < 36; j++ ){
		
		newkkptr->grid[j%6][j/6] = kkptr->grid[j%6][j/6];
		
	}
	
	newkkptr->ctrs = copy_constraints( kkptr );
	
	return 0;
	
}


struct node_ctr *copy_constraints( struct kenken *kkptr ){
	
	struct node_ctr *local_ctrs_head = malloc( sizeof( struct node_ctr ) );
	
	local_ctrs_head->constraint.op = kkptr->ctrs->constraint.op;
	
	local_ctrs_head->constraint.result = kkptr->ctrs->constraint.result;
	
	struct node_square *this_sqr = malloc( sizeof( struct node_square ) );
	
	local_ctrs_head->constraint.numbers = this_sqr;
	
	local_ctrs_head->constraint.numbers->pos[0] = kkptr->ctrs->constraint.numbers->pos[0];
	
	local_ctrs_head->constraint.numbers->pos[1] = kkptr->ctrs->constraint.numbers->pos[1];
	
	local_ctrs_head->constraint.numbers->entry = kkptr->ctrs->constraint.numbers->entry;
	
	for( struct node_square *dmy2 = kkptr->ctrs->constraint.numbers; dmy2 != NULL; 
		dmy2 = dmy2->next_node ){
		
		if( dmy2->next_node != NULL ){
			
			struct node_square *nxt_sqr = malloc( sizeof( struct node_square ) );
			
			nxt_sqr->pos[0] = dmy2->next_node->pos[0];
			
			nxt_sqr->pos[1] = dmy2->next_node->pos[1];
			
			nxt_sqr->entry = dmy2->next_node->entry;
			
			this_sqr->next_node = nxt_sqr;
			
			this_sqr = nxt_sqr;
			
		}
		
		else this_sqr->next_node = NULL;
		
	}
	
	struct node_ctr *dmyctrptr = local_ctrs_head;
	
	for( struct node_ctr *dmy = kkptr->ctrs; dmy!=NULL; dmy = dmy->next_node ){
		
		if( dmy->next_node != NULL ){
			
			struct node_ctr *nxt_ele = malloc( sizeof( struct node_ctr ) );
			
			nxt_ele->constraint.op = dmy->next_node->constraint.op;
			
			nxt_ele->constraint.result = dmy->next_node->constraint.result;
			
			this_sqr = malloc( sizeof( struct node_square ) );
			
			nxt_ele->constraint.numbers = this_sqr;
			
			nxt_ele->constraint.numbers->pos[0] = dmy->next_node->constraint.numbers->pos[0];
			
			nxt_ele->constraint.numbers->pos[1] = dmy->next_node->constraint.numbers->pos[1];
			
			nxt_ele->constraint.numbers->entry = dmy->next_node->constraint.numbers->entry;
			
			for( struct node_square *dmy2 = dmy->next_node->constraint.numbers; dmy2 != NULL; 
				dmy2 = dmy2->next_node ){
				
				if( dmy2->next_node != NULL ){
					
					struct node_square *nxt_sqr = malloc( sizeof( struct node_square ) );
					
					nxt_sqr->pos[0] = dmy2->next_node->pos[0];
					
					nxt_sqr->pos[1] = dmy2->next_node->pos[1];
					
					nxt_sqr->entry = dmy2->next_node->entry;
					
					this_sqr->next_node = nxt_sqr;
						
					this_sqr = nxt_sqr;
					
				}
				
				else this_sqr->next_node = NULL;
				
			}
			
			dmyctrptr->next_node = nxt_ele;
			
			dmyctrptr = nxt_ele;
			
		}
		
		else dmyctrptr->next_node = NULL;
		
	}
	
	return local_ctrs_head;
	
}


int update_usr_kenken( struct kenken *usrkk ){
	
	for( struct node_ctr *dmy = usrkk->ctrs; dmy != 0; dmy = dmy->next_node ){
		
		for( struct node_square *dmy2 = dmy->constraint.numbers; dmy2 != NULL; 
			dmy2 = dmy2->next_node ){
				
				dmy2->entry = usrkk->grid[dmy2->pos[0]][dmy2->pos[1]];
			
		}
		
	}
	
	return 0;
	
}
