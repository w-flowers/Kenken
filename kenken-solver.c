#include"kenken-solver.h"

int solve_kenken( struct kenken *kkptr ){
	
	
	struct pzlsqr psqrs[6][6];
	
	for( int i = 0; i < 6; i++ ){
		
		for( int j = 0; j<6; j++ ){
			
			if( kkptr->grid[i][j] == 0 ){
				
				psqrs[i][j].solved=-1;
				
				for( int k = 0; k < 6; k++ ){
					
					psqrs[i][j].avlbl_vals[k]=k+1;
					
				}
				
			}
			
			else{
				
				psqrs[i][j].solved=kkptr->grid[i][j];
				
				psqrs[i][j].avlbl_vals[0]=kkptr->grid[i][j];
				
				for( int k = 1; k < 6; k++ ){
					
					psqrs[i][j].avlbl_vals[k]=0;
					
				}
				
			}
			
		}
		
	}
	
	//create local constraints and copy contents of kkptr to them
	struct node_ctr *local_ctrs_head = malloc( sizeof( struct node_ctr ) );
	
	local_ctrs_head->constraint.op = kkptr->ctrs->constraint.op;
	
	local_ctrs_head->constraint.result = kkptr->ctrs->constraint.result;
	
	struct node_square *this_sqr = malloc( sizeof( struct node_square ) );
	
	local_ctrs_head->constraint.numbers = this_sqr;
	
	local_ctrs_head->constraint.numbers->pos[0] = kkptr->ctrs->constraint.numbers->pos[0];
	
	local_ctrs_head->constraint.numbers->pos[1] = kkptr->ctrs->constraint.numbers->pos[1];
	
	local_ctrs_head->constraint.numbers->entry = kkptr->ctrs->constraint.numbers->entry;
	
	
	for( struct node_square *dmy2 = kkptr->ctrs->constraint.numbers; dmy2 != NULL; 
		
		dmy2 = dmy2->next_node )
	
	{
		
		
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
		
			for( struct node_square *dmy2 = dmy->next_node->constraint.numbers; 
				dmy2 != NULL; dmy2 = dmy2->next_node )
			{
		
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
	
	struct node_ctr *sqrctr[6][6];
	struct node_square *sqrnumnd[6][6];
	
	for( struct node_ctr *dmy = local_ctrs_head; dmy!=NULL; dmy = dmy->next_node ){
		
		for( struct node_square *dmy2 = dmy->constraint.numbers; dmy2!=NULL;
			dmy2 = dmy2->next_node ){
				
			sqrctr[dmy2->pos[0]][dmy2->pos[1]] = dmy;
			sqrnumnd[dmy2->pos[0]][dmy2->pos[1]] = dmy2;
			
		}
		
	}
	
	
	for( struct node_ctr *dmy = local_ctrs_head; dmy!=NULL; dmy = dmy->next_node ){
		
		reduce_constraint_arrays_1( &( dmy->constraint ), psqrs );
	}

	
	int nodiff = 4;
	int norep = 20;
	
	while( nodiff && norep ){
		
		nodiff = 0;
		//struct pzlsqr dmypsqrs[6][6];
		//copy_pzlsqrs( dmypsqrs, psqrs );

		int testint1 = reduce_by_grid( psqrs, sqrnumnd );
		
		if( testint1 == -1 ) printf( "reduced grid too far\n" );
		
		nodiff += testint1;
		
		
		for( struct node_ctr *dmy = local_ctrs_head; dmy!=NULL; dmy = dmy->next_node ){
			
			int testint2 = reduce_constraint_arrays_2( &( dmy->constraint ), psqrs );
			
			if( testint2 == -1 ) printf( "reduced constraints too far\n" );
			
			nodiff += testint2;
			
		}
		
		
		nodiff += reduce_by_rowcol( psqrs, sqrnumnd );
		
		
		if( !nodiff ) nodiff += reduce_by_hiddenpairs( psqrs );
		
		
		norep--;
		
	}
	
	
	struct pzlsqr psqrs2[6][6];
	copy_pzlsqrs( psqrs2, psqrs );
	
	
	//create copy of local constraints
	struct node_ctr *local_ctrs_head_copy = malloc( sizeof( struct node_ctr ) );
	
	local_ctrs_head_copy->constraint.op = local_ctrs_head->constraint.op;
	
	local_ctrs_head_copy->constraint.result = local_ctrs_head->constraint.result;
	
	
	this_sqr = malloc( sizeof( struct node_square ) );
	
	local_ctrs_head_copy->constraint.numbers = this_sqr;
	
	local_ctrs_head_copy->constraint.numbers->pos[0] = local_ctrs_head->constraint.numbers->pos[0];
	
	local_ctrs_head_copy->constraint.numbers->pos[1] = local_ctrs_head->constraint.numbers->pos[1];
	
	local_ctrs_head_copy->constraint.numbers->entry = local_ctrs_head->constraint.numbers->entry;
	
	
	for( struct node_square *dmy2 = local_ctrs_head->constraint.numbers; 
		
		dmy2 != NULL; dmy2 = dmy2->next_node ){
	
	
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
	
	
	dmyctrptr = local_ctrs_head_copy;
	
	for( struct node_ctr *dmy = local_ctrs_head; dmy!=NULL; dmy = dmy->next_node ){
		
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
	
	
	int zero_mat[6][6];
	
	for( int i = 0; i < 36; i++ ) zero_mat[i%6][i/6] = 0;
	
	
	int success_value = brute_fill_grid_2( local_ctrs_head, 
		
		local_ctrs_head->constraint.numbers, psqrs, psqrs2, zero_mat, local_ctrs_head_copy );
	
	
	
	for( int i = 0; i < 36; i++ ) kkptr->grid[i%6][i/6] = psqrs[i%6][i/6].solved;
	
	update_usr_kenken( kkptr );
	
	
	for( struct node_ctr *dmy_ptr = local_ctrs_head; dmy_ptr != NULL; ){
		
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
	
	
	for( struct node_ctr *dmy_ptr = local_ctrs_head_copy; dmy_ptr != NULL; ){
		
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
	
	return success_value;
	
}

int reduce_constraint_arrays_1( struct constraint *ctr, struct pzlsqr psqrs[6][6] ){
	
	
	int change_count = 0;
	
	int res = ctr->result;
	
	for( struct node_square *dmy = ctr->numbers; dmy != NULL; dmy = dmy->next_node ){
		
		int x = dmy->pos[0];
		int y = dmy->pos[1];
		
		if( ctr->op == NOOP ){
			
			psqrs[x][y].solved = res;
			dmy->entry = res;
			
		}
		
		if( ctr->op == DIVOP ){
			
			for( int i = 0; i < 6; i++ ){
				
				int n = psqrs[x][y].avlbl_vals[i];
				
				if( n != 0 ){
					
					if( res * n > 6 && n % res != 0 ) 
						change_count += remove_available( psqrs[x][y].avlbl_vals, n );
				
				}
				
			}
			
		}
		
		if( ctr->op == SUBOP ){
			
			for( int i = 0; i < 6; i++ ){
				
				int n = psqrs[x][y].avlbl_vals[i];
				
				if( n != 0 ){
					
					if( ( res+n > 6 ) && ( n-res < 1 ) ) change_count += remove_available( psqrs[x][y].avlbl_vals, n );
				
				}
			
			}
		
		}
		
		if( ctr->op == MULTOP ){
			
			for( int i = 0; i < 6; i++ ){
				
				int n = psqrs[x][y].avlbl_vals[i];
				
				if( n != 0 ){
					
					if( res%n ) change_count += remove_available( psqrs[x][y].avlbl_vals, n );
					
				}
				
			}
			
		}
		
		if( ctr->op == ADDOP ){
			
			for( int i = 0; i < 6; i++ ){
				
				int n = psqrs[x][y].avlbl_vals[i];
				
				if( n != 0 ){
					
					if( n > res - 1 ) change_count += remove_available( psqrs[x][y].avlbl_vals, n );
				
				}
			
			}
		
		}
		
		if( psqrs[x][y].avlbl_vals[0] == 0 ) return -1;
		
		if( psqrs[x][y].avlbl_vals[1] == 0 ){
			
			psqrs[x][y].solved = psqrs[x][y].avlbl_vals[0];
			
			dmy->entry = psqrs[x][y].solved;
			
			change_count++; 
		
		}
	
	}
	
	return change_count;

}


int reduce_constraint_arrays_2( struct constraint *ctr, struct pzlsqr psqrs[6][6] ){
	
	
	int change_count = 0;
	
	struct dbl_node_sqr *head = NULL;
	struct dbl_node_sqr *tail = NULL;
	
	for( struct node_square *dmy = ctr->numbers; dmy != NULL; dmy = dmy->next_node ){
		
		if( dmy == ctr->numbers ){
			
			struct dbl_node_sqr *nxt_ele = malloc( sizeof( struct dbl_node_sqr ) );
			
			head = nxt_ele;
			
			head->pos[0] = dmy -> pos[0];
			head->pos[1] = dmy -> pos[1];
			
			head->entry = dmy->entry;
			
			head->next_node = NULL;
			head->prev_node = NULL;
			
			tail = head;
		
		}
		
		else{
			
			struct dbl_node_sqr *nxt_ele = malloc( sizeof( struct dbl_node_sqr ) );
			
			tail->next_node = nxt_ele;
			
			nxt_ele->pos[0] = dmy -> pos[0];
			nxt_ele->pos[1] = dmy -> pos[1];
			
			nxt_ele->entry = dmy->entry;
			
			nxt_ele->next_node = NULL;
			nxt_ele->prev_node = tail;
			
			tail = nxt_ele;
		
		}
	
	}
	
	int res = ctr->result;
	
	if( ctr->op == NOOP ){
		
		psqrs[head->pos[0]][head->pos[1]].solved = res;
		
		ctr->numbers->entry = psqrs[head->pos[0]][head->pos[1]].solved;
		
		return 1;
	
	}
	
	if( ctr->op == DIVOP ){
		
		for( struct dbl_node_sqr *dmy = head; dmy != NULL; dmy = dmy->next_node ){
			
			for( int i = 0; i < 6; i++ ){
				
				if( psqrs[dmy->pos[0]][dmy->pos[1]].solved != -1 ) break;
				
				int n = psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals[i];
				
				int marker = 1;
				
				if( n != 0 ){
					
					for( int j = 0; j < 6; j++ ){
						
						if( dmy->prev_node == NULL ){
							
							int m = 
								psqrs[dmy->next_node->pos[0]][dmy->next_node->pos[1]].avlbl_vals[j];
							
							if( m != 0 && ( ( res * n == m ) || ( res * m == n ) ) ){
								
								marker = 1;
								break;
								
							}
							
						}
						
						if( dmy->next_node == NULL ){
							
							int m = psqrs[dmy->prev_node->pos[0]][dmy->prev_node->pos[1]].avlbl_vals[j];
							
							if( m != 0 && ( ( res * n == m ) || ( res * m == n ) ) ){
								
								marker = 1;
								break;
								
							}
							
						}
						
					}
					
					if( marker == 0 ) change_count += 
						remove_available( psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals, n );
					
				}
				
			}
			
		}
		
	}
	
	if( ctr->op == SUBOP ){
		
		for( struct dbl_node_sqr *dmy = head; dmy != NULL; dmy = dmy->next_node ){
			
			for( int i = 0; i < 6; i++ ){
				
				if( psqrs[dmy->pos[0]][dmy->pos[1]].solved != -1 ) break;
				
				int n = psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals[i];
				
				int marker = 1;
				
				if( n != 0 ){
					
					for( int j = 0; j < 6; j++ ){
						
						if( dmy->prev_node == NULL ){
							
							int m = psqrs[dmy->next_node->pos[0]][dmy->next_node->pos[1]].avlbl_vals[j];
							
							if( m != 0 && ( ( res + n == m ) || ( res + m == n ) ) ){
							
								marker = 1;
								break;
							
							}
						
						}
						
						if( dmy->next_node == NULL ){
							
							int m = 
								psqrs[dmy->prev_node->pos[0]][dmy->prev_node->pos[1]].avlbl_vals[j];
							
							if( m != 0 && ( ( res + n == m ) || ( res + m == n ) ) ){
							
								marker = 1;
								break;
							
							}
						
						}
					
					}
					
					if( marker == 0 ) change_count += 
						remove_available( psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals, n );
				
				}
			
			}
		
		}
	
	}
	
	if( ctr->op == MULTOP ){
		
		for( struct dbl_node_sqr *dmy = head; dmy != NULL; dmy = dmy->next_node ){
			
			for( int i = 0; i < 6 && psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals[i]; i++ ){
				
				if( psqrs[dmy->pos[0]][dmy->pos[1]].solved != -1 ) break;
				
				int n = psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals[i];
				
				int test_res = n;
				
				int rhf_res = recursive_helper_f_mult( dmy->next_node, dmy->prev_node, 
					psqrs, test_res, res );
					
				
				if( rhf_res == 0 ) change_count += 
					remove_available( psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals, n );
			
			}
		
		}
	
	}
	
	if( ctr->op == ADDOP ){
		
		for( struct dbl_node_sqr *dmy = head; dmy != NULL; dmy = dmy->next_node ){
			
			for( int i = 0; i < 6 && psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals[i]; i++ ){
				
				if( psqrs[dmy->pos[0]][dmy->pos[1]].solved != -1 ) break;
				
				int n = psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals[i];
				
				int test_res = res - n;
				
				int rhf_res = recursive_helper_f_add( dmy->next_node, dmy->prev_node, 
					psqrs, test_res );
				
				if( rhf_res == 0 ) change_count += remove_available( psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals, n );
			
			}
			
		}
	}
	
	
	for( struct dbl_node_sqr *dmy = head; dmy!=NULL; ){
		
		struct dbl_node_sqr *dltdsqrptr = dmy;
		
		dmy = dmy->next_node;
		
		free( dltdsqrptr );
	
	}
	
	for( struct node_square *dmy = ctr->numbers; dmy != NULL; dmy = dmy->next_node ){
		
		if( psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals[0] == 0 ) return -1;
		
		if( psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals[1] == 0 ){
			
			psqrs[dmy->pos[0]][dmy->pos[1]].solved = psqrs[dmy->pos[0]][dmy->pos[1]].avlbl_vals[0];
			
			dmy->entry = psqrs[dmy->pos[0]][dmy->pos[1]].solved;
			
			change_count++;
		
		}
	
	}
	
	return change_count;

}

int reduce_by_grid( struct pzlsqr psqrs[6][6], struct node_square *sqrnumnd[6][6] ){
	
	int change_count = 0;
	
	for( int i = 0; i < 6; i++ ){
		
		for( int j = 0; j < 6; j++ ){
			
			if( psqrs[i][j].solved == -1 ){
				
				for( int k = 0; k < 6; k++ ){
					
					if( psqrs[k][j].solved != -1 ) change_count += 
						remove_available( psqrs[i][j].avlbl_vals, psqrs[k][j].solved );
				
				}
				
				for( int k = 0; k < 6; k++ ){
					
					if( psqrs[i][k].solved != -1 ) change_count += 
						remove_available( psqrs[i][j].avlbl_vals, psqrs[i][k].solved );
					
				}
				
				if( psqrs[i][j].avlbl_vals[0] == 0 ) return -1;
				
				if( psqrs[i][j].avlbl_vals[1] == 0 ){
					
					psqrs[i][j].solved = psqrs[i][j].avlbl_vals[0];
					
					sqrnumnd[i][j]->entry = psqrs[i][j].solved;
					
					change_count++;
				
				}
			
			}
		
		}
	
	}
	
	return change_count;

}

int reduce_by_rowcol( struct pzlsqr psqrs[6][6], struct node_square *sqrnumnd[6][6] ){
	
	
	int change_count = 0;
	
	for( int rownum = 0; rownum < 6; rownum++ ){
		
		for( int ent = 1; ent < 7; ent++ ){
			
			int entcount = 0;
			int entsqr[2];
			
			for( int i = 0; i < 6; i++ ){
				
				if( psqrs[i][rownum].solved == ent ){
					
					entcount = 0;
					break;
					
				}
				
				if( psqrs[i][rownum].solved == -1 ){
					
					for( int avind = 0; avind < 6 && psqrs[i][rownum].avlbl_vals[avind]; avind++ ){
						
						if( psqrs[i][rownum].avlbl_vals[avind] == ent ){
							
							entcount++;
							
							entsqr[0] = i;
							entsqr[1] = rownum;
							break;
						
						}
					
					}
					
					if( entcount > 1 ) break;
				
				}
			
			}
			
			if( entcount == 1 ){
				
				psqrs[entsqr[0]][entsqr[1]].solved = ent;
				
				sqrnumnd[entsqr[0]][entsqr[1]]->entry = ent;
				
				change_count++;
			
			}
		
		}
	
	}
	
	for( int colnum = 0; colnum < 6; colnum++ ){
		
		for( int ent = 1; ent < 7; ent++ ){
			
			int entcount = 0;
			int entsqr[2];
			
			for( int j = 0; j < 6; j++ ){
				
				if( psqrs[colnum][j].solved == ent ){
					
					entcount = 0;
					break;
				
				}
				
				if( psqrs[colnum][j].solved == -1 ){
					
					for( int avind = 0; avind < 6 && psqrs[colnum][j].avlbl_vals[avind]; avind++ ){
						
						if( psqrs[colnum][j].avlbl_vals[avind] == ent ){
							
							entcount++;
							
							entsqr[0] = colnum;
							entsqr[1] = j;
							break;
						
						}
					
					}
					
					if( entcount > 1 ) break;
				
				}
			
			}
			
			if( entcount == 1 ){
				
				psqrs[entsqr[0]][entsqr[1]].solved = ent;
				
				sqrnumnd[entsqr[0]][entsqr[1]]->entry = ent;
				
				change_count++;
			
			}
		}
		
	}
	
	return change_count;

}

int reduce_by_hiddenpairs( struct pzlsqr psqrs[6][6] ){
	
	
	int change_count = 0;
	
	for( int rownum = 0; rownum < 6; rownum++ ){
		
		for( int i = 0; i < 6; i++ ){
			
			if( psqrs[i][rownum].solved != -1 && nzero_length( psqrs[i][rownum].avlbl_vals ) == 2 ){
				
				for( int j = 0; j < 6; j++ ){
					
					if( j != i && psqrs[j][rownum].solved != -1 ){
						
						if( psqrs[i][rownum].avlbl_vals[0] == psqrs[j][rownum].avlbl_vals[0] && 
							psqrs[i][rownum].avlbl_vals[1] == psqrs[j][rownum].avlbl_vals[1] && 
							!psqrs[j][rownum].avlbl_vals[2] ){
						
							for( int k = 0; k < 6; k++ ){
								
								if( k != i && k!= j ){
									
									change_count += 
										remove_available( psqrs[k][rownum].avlbl_vals, 
										psqrs[i][rownum].avlbl_vals[0] );
									
									change_count += 
										remove_available( psqrs[k][rownum].avlbl_vals, 
										psqrs[i][rownum].avlbl_vals[1] );
								
								}
								
							}
							
							break;
							
						}
						
					}
					
				}
				
			}
			
		}
		
	}
	
	for( int colnum = 0; colnum < 6; colnum++ ){
		
		for( int i = 0; i < 6; i++ ){
			
			if( psqrs[colnum][i].solved != -1 && nzero_length( psqrs[colnum][i].avlbl_vals ) == 2 ){
				
				for( int j = 0; j < 6; j++ ){
					
					if( j != i && psqrs[colnum][i].solved != -1 ){
						
						if( psqrs[colnum][i].avlbl_vals[0] == psqrs[colnum][j].avlbl_vals[0] && 
							psqrs[colnum][i].avlbl_vals[1] == psqrs[colnum][j].avlbl_vals[1] && 
							!psqrs[colnum][j].avlbl_vals[2] ){
							
							for( int k = 0; k < 6; k++ ){
								
								if( k != i && k!= j ){
									
									change_count += 
										remove_available( psqrs[colnum][k].avlbl_vals, 
										psqrs[colnum][i].avlbl_vals[0] );
										
									change_count += 
										remove_available( psqrs[colnum][k].avlbl_vals, 
										psqrs[colnum][i].avlbl_vals[1] );
								}
								
							}
							
							break;
							
						}
						
					}
					
				}
				
			}
			
		}
		
	}
	
	return change_count;
	
}

int nzero_length( int arr[6] ){
	
	int length = 0;
	while( length < 6 && arr[length] ) length++;
	
	return length;
	
}


int recursive_helper_f_mult( struct dbl_node_sqr *dmynxt, struct dbl_node_sqr *dmyprv, 
	
	struct pzlsqr psqrs[6][6], int test_res, int result ){
	
	
	if( dmynxt != NULL ){
		
		if( psqrs[dmynxt->pos[0]][dmynxt->pos[1]].solved != -1 ){
			
			int m = psqrs[dmynxt->pos[0]][dmynxt->pos[1]].solved;
			
			int test_result = test_res*m;
			
			if( dmynxt->next_node == NULL && dmyprv == NULL ){
				
				if( test_result == result ) return 1;
				else return 0;
				
			}
			
			else{
				
				if( recursive_helper_f_mult( dmynxt->next_node, dmyprv, psqrs, test_result, result ) ) return 1;
				
			}			
			
		}
		
		else{
			
			for( int i = 0; i < 6 && psqrs[dmynxt->pos[0]][dmynxt->pos[1]].avlbl_vals[i]; i++ ){
				
				int n = psqrs[dmynxt->pos[0]][dmynxt->pos[1]].avlbl_vals[i];
				
				int test_result = test_res*n;
				
				if( dmynxt->next_node == NULL && dmyprv == NULL ){
					
					if( test_result == result ) return 1;
					
				}
				
				else if( recursive_helper_f_mult( dmynxt->next_node, dmyprv, 
					psqrs, test_result, result ) ) return 1;
					
			}
			
		}
		
	}
	
	else if( dmyprv != NULL ){
		
		if( psqrs[dmyprv->pos[0]][dmyprv->pos[1]].solved != -1 ){
			
			int m = psqrs[dmyprv->pos[0]][dmyprv->pos[1]].solved;
			
			int test_result = test_res*m;
			
			if( dmyprv->prev_node == NULL ){
				
				if( test_result == result ) return 1;
				
				else return 0;
				
			}
			
			else{
				
				if( recursive_helper_f_mult( NULL, dmyprv->prev_node, psqrs, test_result, result ) ) return 1;
				
			}
					
		}
		
		else{
			
			for( int i = 0; i < 6 && psqrs[dmyprv->pos[0]][dmyprv->pos[1]].avlbl_vals[i]; i++ ){
				
				int n = psqrs[dmyprv->pos[0]][dmyprv->pos[1]].avlbl_vals[i];
				
				int test_result = test_res*n;
				
				if( dmyprv->prev_node == NULL ){
					
					if( test_result == result ) return 1;
					
				}
				
				else if( recursive_helper_f_mult( NULL, dmyprv->prev_node, psqrs, test_result, 
					result ) ) return 1;
				
			}
			
		}
		
	}
	
	return 0;
}

int recursive_helper_f_add( struct dbl_node_sqr *dmynxt, struct dbl_node_sqr *dmyprv, 

	struct pzlsqr psqrs[6][6], int test_res ){
	
	
	if( dmynxt != NULL ){
		
		if( psqrs[dmynxt->pos[0]][dmynxt->pos[1]].solved != -1 ){
			
			int m = psqrs[dmynxt->pos[0]][dmynxt->pos[1]].solved;
			
			int test_result = test_res-m;
			
			if( dmynxt->next_node == NULL && dmyprv == NULL ){
				
				if( test_result ) return 0;
				else return 1;
				
			}
			
			else{
				
				if( recursive_helper_f_add( dmynxt->next_node, dmyprv, psqrs, test_result ) )
					return 1;
				
			}
			
		}
		
		else{
			
			for( int i = 0; i < 6 && psqrs[dmynxt->pos[0]][dmynxt->pos[1]].avlbl_vals[i]; i++ ){
				
				int n = psqrs[dmynxt->pos[0]][dmynxt->pos[1]].avlbl_vals[i];
				
				int test_result = test_res-n;
				
				if( dmynxt->next_node == NULL && dmyprv == NULL ){
					
					if( !test_result ) return 1;
					
				}
				
				else if( recursive_helper_f_add( dmynxt->next_node, dmyprv, psqrs, test_result ) ) 
					return 1;
				
			}
			
		}
		
	}
	
	else if( dmyprv != NULL ){
		
		if( psqrs[dmyprv->pos[0]][dmyprv->pos[1]].solved != -1 ){
			
			int m = psqrs[dmyprv->pos[0]][dmyprv->pos[1]].solved;
			
			int test_result = test_res-m;
			
			if( dmyprv->prev_node == NULL ){
				
				if( test_result ) return 0;
				
				else return 1;
				
			}
			
			else{
				
				if( recursive_helper_f_add( NULL, dmyprv->prev_node, psqrs, test_result ) ) 
					return 1;
				
			}		
			
		}
		
		else{
			
			for( int i = 0; i < 6 && psqrs[dmyprv->pos[0]][dmyprv->pos[1]].avlbl_vals[i]; i++ ){
				
				int n = psqrs[dmyprv->pos[0]][dmyprv->pos[1]].avlbl_vals[i];
				
				int test_result = test_res-n;
				
				if( dmyprv->prev_node == NULL ){
					
					if( !test_result ) return 1;
					
				}
				
				else if( recursive_helper_f_add( NULL, dmyprv->prev_node, psqrs, test_result ) ) 
					return 1;
				
			}
			
		}
		
	}
	
	return 0;
	
}


//init solution needs to be zero on the first pass, then the found solution on the second
//must update entries in constraints based on psqrs
int brute_fill_grid_2( struct node_ctr *curr_cst, struct node_square *curr_sqr, 
	
	struct pzlsqr psqrs[6][6], struct pzlsqr psqrs2[6][6], int init_sol[6][6], 
	
	struct node_ctr *init_csts ){
	
	
	struct pzlsqr dmypsqrs[6][6];
	copy_pzlsqrs( dmypsqrs, psqrs );
	
	int hori_ind = curr_sqr->pos[0];
	int vert_ind = curr_sqr->pos[1];
	
	reduce_by_grid_2( psqrs, hori_ind, vert_ind );
	
	if( curr_sqr->next_node == NULL ){
		
		if( curr_cst->next_node == NULL ){
			
			//base case for recursion - reaching end of constraint list
			if( psqrs[hori_ind][vert_ind].solved != -1 ){
				
				//Check for mult solutions, then return 1 if successful, -1 if mult sol
				
				if( !valid_constraint( &( curr_cst->constraint ) ) ) return 0;
				
				return bfg2_has_mult_sol( curr_cst, curr_sqr, psqrs, psqrs2, dmypsqrs, init_sol, 
					init_csts );
			}
			else{
				
				if( psqrs[hori_ind][vert_ind].avlbl_vals[0] != 0 ){
					
					psqrs[hori_ind][vert_ind].solved = psqrs[hori_ind][vert_ind].avlbl_vals[0];
					
					curr_sqr->entry = psqrs[hori_ind][vert_ind].solved;
					
					if( !valid_constraint( &( curr_cst->constraint ) ) ) return 0;
					
					//check for mult solutions, the return 1 if successful, -1 if mult sol
					
					return bfg2_has_mult_sol( curr_cst, curr_sqr, psqrs, psqrs2, dmypsqrs, 
						init_sol, init_csts );
					
				}
				
			}
			
		}
		
		//below walks to the first node of the next constraint
		else{
			
			if( psqrs[hori_ind][vert_ind].solved != -1 ){
				
				//check if current constraint is valid, return 0 if not
				if( !valid_constraint( &( curr_cst->constraint ) ) ) return 0;
				
				return brute_fill_grid_2( curr_cst->next_node, 
					curr_cst->next_node->constraint.numbers, psqrs, psqrs2, init_sol, init_csts );
			
			}
			
			else{
				
				//find an available value
				//check if current constraint is valid with this value, try next value if not
				//then check against first node of next_constraint
				
				while( psqrs[hori_ind][vert_ind].avlbl_vals[0] ){
					
					psqrs[hori_ind][vert_ind].solved = psqrs[hori_ind][vert_ind].avlbl_vals[0];
					
					curr_sqr->entry = psqrs[hori_ind][vert_ind].solved;
					
					if( !valid_constraint( &( curr_cst->constraint ) ) ){
						
						remove_available( psqrs[hori_ind][vert_ind].avlbl_vals, 
							psqrs[hori_ind][vert_ind].solved );
						
					}
					
					else{
						
						int nxtsqrres = brute_fill_grid_2( curr_cst->next_node, 
							curr_cst->next_node->constraint.numbers, psqrs, psqrs2, 
							init_sol, init_csts );
						
						if( nxtsqrres ) return nxtsqrres;
						
						else{
							
							remove_available( psqrs[hori_ind][vert_ind].avlbl_vals, 
								psqrs[hori_ind][vert_ind].solved );
							
						}
						
					}
					
				}
				
			}
			
		}
		
	}
	
	//below walks to the next node of the current constraint
	else{
		
		if( psqrs[hori_ind][vert_ind].solved != -1 ){
			
			//check against next square in constraint
			return brute_fill_grid_2( curr_cst, curr_sqr->next_node, psqrs, psqrs2, init_sol, 
				init_csts );
				
		}
		
		else{
			
			//find an available value, then test against next square of constraint
			
			while( psqrs[hori_ind][vert_ind].avlbl_vals[0] ){
				
				psqrs[hori_ind][vert_ind].solved = psqrs[hori_ind][vert_ind].avlbl_vals[0];
				
				curr_sqr->entry = psqrs[hori_ind][vert_ind].solved;
				
				int nxtsqrres = brute_fill_grid_2( curr_cst, curr_sqr->next_node, psqrs, psqrs2, 
					init_sol, init_csts );
				
				if( nxtsqrres ) return nxtsqrres;
				
				else{
					
					remove_available( psqrs[hori_ind][vert_ind].avlbl_vals, 
						psqrs[hori_ind][vert_ind].solved );
					
				}
				
			}
			
		}
		
	}
	
	copy_pzlsqrs( psqrs, dmypsqrs );
	
	return 0;
	
}

//NOTE: Contains a call to brute_fill_grid_2 -- best viewed as a helper function for it
int bfg2_has_mult_sol( struct node_ctr *curr_cst, struct node_square *curr_sqr, 
	
	struct pzlsqr psqrs[6][6], struct pzlsqr psqrs2[6][6], struct pzlsqr dmypsqrs[6][6], 
	
	int init_sol[6][6], struct node_ctr *init_csts ){
	
	
	if( init_sol[0][0] ){
		
		if( eq_init_sol( psqrs, init_sol ) ){
			
				copy_pzlsqrs( psqrs, dmypsqrs );
				return 0;
				
		}
		
		else return 1;
		
	}
	
	else{
		
		int arr[6][6];
		
		for( int i = 0; i < 6; i++ ){
			for( int j = 0; j < 6; j++ ){
				
				arr[i][j] = psqrs[i][j].solved;
				
			}
			
		}
		
		struct pzlsqr psqrs3[6][6];
		copy_pzlsqrs( psqrs3, psqrs2 );
		
		int mult_sol = brute_fill_grid_2( init_csts, init_csts->constraint.numbers, psqrs2, psqrs3, 
			arr, init_csts );
		
		if( mult_sol ) return -1;
		
		else return 1;
		
	}
	
}

int eq_init_sol( struct pzlsqr psqrs[6][6], int init_sol[6][6] ){
	
	for( int i = 0; i < 6; i++ ){
		
		for( int j = 0; j < 6; j++ ){
			
			if( init_sol[i][j] != psqrs[i][j].solved ) return 0;
			
		}
		
	}
	
	return 1;
	
}

int eq_init_sol_2( struct pzlsqr psqrs[6][6], struct pzlsqr init_sol[6][6] ){
	
	for( int i = 0; i < 6; i++ ){
		
		for( int j = 0; j < 6; j++ ){
			
			if( init_sol[i][j].solved != psqrs[i][j].solved ) return 0;
			
			for( int k = 0; k < 6; k++ ){
				
				if( init_sol[i][j].avlbl_vals[k] != psqrs[i][j].avlbl_vals[k] ) return 0;
				
			}
			
		}
		
	}
	
	return 1;
	
}

int reduce_by_grid_2( struct pzlsqr psqrs[6][6], int hori_ind, int vert_ind ){
	
	
	if( psqrs[hori_ind][vert_ind].solved != -1 ) return 0;
	
	for( int k = 0; k < 6; k++ ){
		
		if( psqrs[k][vert_ind].solved != -1 ) 
			remove_available( psqrs[hori_ind][vert_ind].avlbl_vals, psqrs[k][vert_ind].solved );
		
	}
	
	for( int k = 0; k < 6; k++ ){
		
		if( psqrs[hori_ind][k].solved != -1 ) 
			remove_available( psqrs[hori_ind][vert_ind].avlbl_vals, psqrs[hori_ind][k].solved );
		
	}
	
	return 0;
	
}

int copy_pzlsqrs( struct pzlsqr psqrs2[6][6], struct pzlsqr psqrs[6][6] ){
	
	
	for( int i = 0; i < 36; i++ ){
		
		psqrs2[i%6][i/6].solved = psqrs[i%6][i/6].solved;
		
		for( int j = 0; j < 6; j++ ) 
			psqrs2[i%6][i/6].avlbl_vals[j] = psqrs[i%6][i/6].avlbl_vals[j];
		
	}
	
	return 0;
	
}

//DOES NOT WORK -- Likely needs to be rewritten, and needs to have the kenken ptr passed to it to check constraints and to check for validity

//Also, randomly leads to program infinite looping, main doesn't even execute

//fills a grid by brute force ( after trimming ), returning 0 if solution is not unique or there are no solutions
//start this using array of 0s
int brute_fill_grid( int hori_ind, int vert_ind, struct pzlsqr psqrs[6][6], struct pzlsqr psqrs2[6][6], int init_sol[6][6] ){
	struct pzlsqr dmypsqrs[6][6];
	copy_pzlsqrs( dmypsqrs, psqrs );
	if( psqrs[hori_ind][vert_ind].solved != -1 ){
		if( hori_ind == 5 && vert_ind == 5 ){
			if( init_sol[0][0] ){
				printf( "reached end\n" );
				if( eq_init_sol( psqrs, init_sol ) ){
						for( int i = hori_ind + vert_ind*6; i < 36; i++ ){
							for( int k = 0; k < 6; k++ ) psqrs[hori_ind][vert_ind].avlbl_vals[k] = psqrs2[hori_ind][vert_ind].avlbl_vals[k];
						}
						return 0;
					}
				
				else return 1;
			}
			else{
				int arr[6][6];
				for( int i = 0; i < 6; i++ ){
					for( int j = 0; j < 6; j++ ){
						arr[i][j] = psqrs[i][j].solved;
					}
				}
				struct pzlsqr psqrs3[6][6];
				for( int i = 0; i < 36; i++ ){
					psqrs3[i%6][i/6].solved = psqrs2[i%6][i/6].solved;
					for( int j = 0; j < 6; j++ ) psqrs3[i%6][i/6].avlbl_vals[j] = psqrs2[i%6][i/6].avlbl_vals[j];
				}
				printf( "reached middle\n" );
				int mult_sol = brute_fill_grid( 0, 0, psqrs2, psqrs3, arr );
				if( mult_sol ) return -1;
				else return 1;
			}
		}
		return brute_fill_grid( ( hori_ind + 1 )%6, ( vert_ind + ( hori_ind+1 )/6 ), psqrs, psqrs2, init_sol );
	}
	else{
		reduce_by_grid_2( psqrs, hori_ind, vert_ind );
		//if( sol_poss == -1 ) return 0;
		while( psqrs[hori_ind][vert_ind].avlbl_vals[0] != 0 ){
			psqrs[hori_ind][vert_ind].solved = random_available( psqrs[hori_ind][vert_ind].avlbl_vals );
			if( hori_ind == 5 && vert_ind == 5 ){
				if( init_sol[0][0] ){
					printf( "reached end\n" );
					if( eq_init_sol( psqrs, init_sol ) ){
						for( int i = hori_ind + vert_ind*6; i < 36; i++ ){
							for( int k = 0; k < 6; k++ ) psqrs[hori_ind][vert_ind].avlbl_vals[k] = psqrs2[hori_ind][vert_ind].avlbl_vals[k];
						}
						return 0;
					}
					else return 1;
				}
				else{
					//make solution array to pass to second solution attempt
					int arr[6][6];
					for( int i = 0; i < 6; i++ ){
						for( int j = 0; j < 6; j++ ){
							arr[i][j] = psqrs[i][j].solved;
						}
					}
					struct pzlsqr psqrs3[6][6];
					copy_pzlsqrs( psqrs3, psqrs2 );
					
					printf( "reached middle\n" );
					int mult_sol = brute_fill_grid( 0, 0, psqrs2, psqrs3, arr );
					if( mult_sol ) return -1;
					else return 1;
				}
			}
			int nxt_sqr = brute_fill_grid( ( hori_ind + 1 )%6, ( vert_ind + ( hori_ind+1 )/6 ), psqrs, psqrs2, init_sol );
			if( nxt_sqr == -1 ) return -1;
			if( nxt_sqr == 0 ){
				//printf( "walkback\n" );
				remove_available( psqrs[hori_ind][vert_ind].avlbl_vals, psqrs[hori_ind][vert_ind].solved );
				psqrs[hori_ind][vert_ind].solved = -1;
			}
			if( nxt_sqr == 1 ) return 1;
		}
	}
	for( int i = 0; i < 36; i++ ){
		psqrs[i%6][i/6].solved = dmypsqrs[i%6][i/6].solved;
		for( int j = 0; j < 6; j++ ) psqrs[i%6][i/6].avlbl_vals[j] = dmypsqrs[i%6][i/6].avlbl_vals[j];
	}
	return 0;
}
