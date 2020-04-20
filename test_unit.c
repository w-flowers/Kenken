#include "kenken-solver.c"

int main(){
	//printf("Main entered");
	srand(time(NULL));
	int t1 = (int) time(NULL);
	int t2;
	struct kenken test;
	int onesol = 0;
	int multsol = 0;
	int nosol = 0;
	int dmyarr[6][6];
	int test2;
	for(int i = 0; i < 1000; i++){
		//printf("pre gk entered\n");
		generate_kenken(&test);
		//printf("here\n");
		if(!kenken_valid(&test)) printf("big fuck up");
		//printf("post gk entered\n");
		for(int j = 0; j < 36; j++){
			//printf("%d", test.grid[j%6][j/6]);
			//if(!((j+1)%6))printf("\n");
			dmyarr[j%6][j/6] = test.grid[j%6][j/6];
			test.grid[j%6][j/6] = 0;
		}
		//printf("post grid 0\n");
		update_usr_kenken(&test);
		test2 = solve_kenken(&test);
		update_usr_kenken(&test);
		//if(!kenken_valid(&test)) printf("invalid kenken result %d", test2);
		//printf("%d\n", test2);
		if(!test2){
			for(int j = 0; j < 36; j++){
				printf("%d", dmyarr[j%6][j/6]);
				if(!((j+1)%6))printf("\n");
			}
			for(struct node_ctr *dmy = test.ctrs; dmy != NULL; dmy = dmy->next_node){
				printf("ctr res = %d ctr op = %d\n", dmy->constraint.result, dmy->constraint.op);
				for(struct node_square *dmy2 = dmy->constraint.numbers; dmy2 != NULL; dmy2 = dmy2->next_node){
					printf("%d ", dmy2->entry);
				}
				printf("\n");
			}
			nosol++;
		}
		else if(test2 == 1) onesol++;
		else multsol++;
		
		destroy_kenken(&test);
	}
	printf("os%dns%dms%d\n", onesol, nosol, multsol);
	t2 = (int) time(NULL);
	printf("%ds\n", (t2-t1));
	return 0;
}