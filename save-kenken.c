#include<string.h>
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include"kenken-basic.h"
#include "kenken-basic.c"

const int max = 200;
const int KKSIZE = 6;
/*
.kenken is formatted as follows
g: //start the grid
[1,2,3,4,5,6,]
etc
/g //end the grid
c: //start the constraints
{result,op,(pos0,pos1,entry)} //format of a constraint, number list in ().
/c //end the constraints
*/

int save_grid( int grid[6][6] , FILE *kkfile );

int save_constraints( struct node_ctr *ctrs, FILE *kkfile );

int read_grid_helper ( int target[6][6], FILE *kkfile );

int read_constraints( struct kenken *kkptr, FILE *kkfile );

//saves a kenken to a file in the format listed above
int save_kenken(struct kenken *kkptr, int usrgrid[6][6], char* filename){
	
	char filepath[50];
	
	int filenmlen = strlen(filename);
	
	snprintf(filepath, 20 + filenmlen, "savegames/%s.kenken", filename);
	
	FILE *kkfile = fopen(filepath, "w");
	
	if(kkfile == NULL) return 1;
	
	setbuf(kkfile, NULL);
	
	fprintf(kkfile, "g:\n");
	
	save_grid( kkptr->grid, kkfile );
	
	fprintf(kkfile, "/g\n");
	
	fprintf(kkfile, "c:\n");
	
	save_constraints( kkptr->ctrs, kkfile );
	
	fprintf(kkfile, "/c\n");
	
	fprintf(kkfile, "u:\n");
	
	save_grid( usrgrid, kkfile );
	
	fprintf(kkfile, "/u\n");
	
	fclose(kkfile);
	
	return 0;
}

//takes the contents of a .kenken and loads it to a kenken pointer
//returns one and wipes the kkptr on failure
int load_kenken(struct kenken *kkptr, char* filename){
	//destroy_kenken(kkptr);
	char filepath[50];
	char buffer[max];
	int filenmlen = strlen(filename);
	int errind = 0;
	snprintf(filepath, 20 + filenmlen, "savegames/%s.kenken", filename);
	FILE *kkfile = fopen(filepath, "r");
	if(kkfile == NULL) return 1;
	setbuf(kkfile, NULL);
	kkptr->ctrs = NULL;
	int success_grid = 0; //Flag - set to one if grid successfully created
	int success_constraints = 0; //Flag - set to one if constraints list created
	int each_sqr[6][6]; //Flag - square is incremented each time it appears in the constraint list
	//when constraints are entered, all squares must have been increment exactly once
	for(int i = 0; i < 6; i++)for(int j = 0; j<6; j++)each_sqr[i][j]=0;
	//main loop to parse the kenken file
	while(fgets(buffer, max, kkfile)){
		if(!strncmp(buffer, "g:\n", 4)){
			int hori = 0;
			int vert = 0;
			while(fgets(buffer, max, kkfile)){
				if(!strncmp(buffer, "/g\n", 4)){
					if(vert != KKSIZE){
						errind = 1;
						printf("Err1\n");
						goto ERR_ROUTINE;
					}
					success_grid++;
					break;
				}
				else{
					if(buffer[0] != '['){
						errind = 1;
						printf("Err2\n");
						goto ERR_ROUTINE;
					}
					int bufind = 0;
					while(buffer[bufind] != ']' && buffer[bufind] && bufind < max){
						bufind++;
						char intbuf[20];
						int ibind = 0;
						while(isdigit(buffer[bufind])&& bufind < max && ibind < 20){
							intbuf[ibind] = buffer[bufind];
							bufind++;
							ibind++;
						}
						intbuf[ibind] = '\0';
						int cur_num = 0;
						if(intbuf[0] != '\0'){
							cur_num = (int) strtol(intbuf, NULL, 10);
							kkptr->grid[hori][vert] = cur_num;
							hori++;
						}
						if(hori > KKSIZE){
							errind = 1;
							printf("Err3\n");
							goto ERR_ROUTINE;
						}
					}
					if(hori != KKSIZE){
						errind = 1;
						printf("Err4\n");
						goto ERR_ROUTINE;
					}
					hori = 0;
					vert++;
				}
			}
		}
		if(!strncmp(buffer, "c:\n", 4)){
			int no_sqrs = 0;
			//struct node_ctr *head = NULL;
			while(fgets(buffer, max, kkfile)){
				if(!strncmp(buffer, "/c\n", 4)){
					if(no_sqrs != KKSIZE*KKSIZE){
						errind = 1;
						goto ERR_ROUTINE;
					}
					for(int i = 0; i < 36; i++){
						if(each_sqr[i%6][i/6] != 1){
							errind = 1;
							goto ERR_ROUTINE;
						}
					}
					success_constraints++;
					break;
				}
				else{
					if(buffer[0] != '{'){
						errind = 1;
						goto ERR_ROUTINE;
					}
					int bufind = 0;
					struct node_ctr *new_ctr = malloc(sizeof(struct node_ctr));
					new_ctr->next_node = kkptr->ctrs;
					new_ctr->constraint.numbers = NULL;
					kkptr->ctrs = new_ctr;
					//struct node_square *num_head = NULL;
					int ctr_vals_filled[2] = {0, 0};
					while(buffer[bufind] != '}' && buffer[bufind] && bufind < max){
						bufind++;
						switch(buffer[bufind]){
							case 'r':{
								int ibind = 0;
								char intbuf[20];
								bufind++;
								while(isdigit(buffer[bufind]) && bufind < max && ibind < 20){
									intbuf[ibind] = buffer[bufind];
									bufind++;
									ibind++;
								}
								intbuf[ibind] = '\0';
								int cur_num = 0;
								if(intbuf[0] != '\0'){
									cur_num = (int) strtol(intbuf, NULL, 10);
									new_ctr->constraint.result = cur_num;
									ctr_vals_filled[0]++;
								}
								else{
									errind = 1;
									printf("Err7\n");
									goto ERR_ROUTINE;
								}
								break;
							}
							case 'o':{
								int ibind = 0;
								char intbuf[20];
								bufind++;
								while(isdigit(buffer[bufind]) && bufind < max && ibind < 20){
									intbuf[ibind] = buffer[bufind];
									bufind++;
									ibind++;
								}
								intbuf[ibind] = '\0';
								int cur_num = 0;
								if(intbuf[0] != '\0'){
									cur_num = (int) strtol(intbuf, NULL, 10);
									new_ctr->constraint.op = cur_num;
									ctr_vals_filled[1]++;
								}
								else{
									errind = 1;
									printf("Err8\n");
									goto ERR_ROUTINE;
								}
								break;
							}
							case '(':{
								struct node_square *new_num = malloc(sizeof(struct node_square));
								new_num->next_node = new_ctr->constraint.numbers;
								new_ctr->constraint.numbers = new_num;
								int num_vals_filled[3] = {0, 0, 0};
								no_sqrs++;
								while(buffer[bufind] != ')' && buffer[bufind] && bufind < max){
									bufind++;
									switch(buffer[bufind]){
										case 'x':{
											char intbuf[20];
											int ibind = 0;
											bufind++;
											while(isdigit(buffer[bufind]) && bufind < max && ibind < 20){
												intbuf[ibind] = buffer[bufind];
												bufind++;
												ibind++;
											}
											intbuf[ibind] = '\0';
											int cur_num = 0;
											if(intbuf[0] != '\0'){
												cur_num = (int) strtol(intbuf, NULL, 10);
												new_num->pos[0] = cur_num;
												num_vals_filled[0]++;
											}
											else{
												errind = 1;
												printf("Err9\n");
												goto ERR_ROUTINE;
											}
											break;
										}
									
										case 'y':{
											char intbuf[20];
											int ibind = 0;
											bufind++;
											while(isdigit(buffer[bufind]) && bufind < max && ibind < 20){
												intbuf[ibind] = buffer[bufind];
												bufind++;
												ibind++;
											}
											intbuf[ibind] = '\0';
											int cur_num = 0;
											if(intbuf[0] != '\0'){
												cur_num = (int) strtol(intbuf, NULL, 10);
												new_num->pos[1] = cur_num;
												num_vals_filled[1]++;
											}
											else{
												errind = 1;
												printf("Err10\n");
												goto ERR_ROUTINE;
											}
											break;
										}
									
										case 'e':{
											char intbuf[20];
											int ibind = 0;
											bufind++;
											while(isdigit(buffer[bufind]) && bufind < max && ibind < 20){
												intbuf[ibind] = buffer[bufind];
												bufind++;
												ibind++;
											}
											intbuf[ibind] = '\0';
											int cur_num = 0;
											if(intbuf[0] != '\0'){
												cur_num = (int) strtol(intbuf, NULL, 10);
												new_num->entry = cur_num;
												num_vals_filled[2]++;
											}
											else{
												errind = 1;
												printf("Err11\n");
												goto ERR_ROUTINE;
											}
											break;
										}
									
									}
								}
								for(int i = 0; i < 3; i++){
									if(num_vals_filled[i] != 1){
										errind = 1;
										printf("Err12:i%d,num_vals_filled[i] %d\n", i, num_vals_filled[i]);
										goto ERR_ROUTINE;
									}
								}
								if(new_num->pos[0] > -1 && new_num->pos[0] < 6 && new_num->pos[1] > -1 && new_num->pos[1] < 6)each_sqr[new_num->pos[0]][new_num->pos[1]]++;
								break;
							}
						}
					}
					for(int i = 0; i < 2; i++){
						if(ctr_vals_filled[i] != 1){
							errind = 1;
							printf("Err13:i%d,ctr_vals_filled[i] %d\n", i, ctr_vals_filled[i]);
							goto ERR_ROUTINE;
						}
					}
				}
			}
		}
	}
	ERR_ROUTINE: 
	if(errind || !success_grid || !success_constraints){
		destroy_kenken(kkptr);
		fclose(kkfile);
		return 1;
	}
	fclose(kkfile);
	return 0;
}

//Only to be called in save_kenken
int save_grid( int grid[6][6] , FILE *kkfile ){
	
	for(int j = 0; j<6; j++){
		fprintf(kkfile, "[");
		for(int i = 0; i<6; i++){
			fprintf(kkfile, "%d,", grid[i][j]);
		}
		fprintf(kkfile, "]\n");
	}
	
	
	return 0;
	
}

//Only to be called in save_kenken
int save_constraints( struct node_ctr *ctrs, FILE *kkfile ){
	
	
	for(struct node_ctr *dmy = ctrs; dmy != NULL; dmy = dmy->next_node){
		
		fprintf(kkfile, "{r%d,o%d,", dmy->constraint.result, dmy->constraint.op);
		
		for(struct node_square *dmy2 = dmy->constraint.numbers; dmy2!=NULL; dmy2 = dmy2->next_node){
			
			fprintf(kkfile, "(x%d,y%d,e%d,)", dmy2->pos[0], dmy2->pos[1], dmy2->entry);
			
		}
		
		fprintf(kkfile, "}\n");
		
	}
	
	return 0;
	
}
