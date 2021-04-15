#include "save-kenken.c"
#include <stdio.h>

int main(){
	struct kenken kkptr;
	generate_kenken(&kkptr);
	int usrgrid[6][6];
	for(int i = 0; i < 36; i++) usrgrid[i/6][i%6] = 0;
	save_kenken(&kkptr, usrgrid, "name");
	struct kenken kenken2;
	for(int i = 0; i < 36; i++) usrgrid[i/6][i%6] = 1;
	int test = load_kenken(&kenken2, usrgrid, "name");
	//int test = kenken_valid(&kenken2);
	printf("%d\n", test);
	return 0;
}
