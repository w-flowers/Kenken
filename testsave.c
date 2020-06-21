#include "save-kenken.c"
#include <stdio.h>

int main(){
	/*struct kenken kkptr;
	generate_kenken(&kkptr);
	save_kenken(&kkptr, "name");*/
	struct kenken kenken2;
	int test = load_kenken(&kenken2, "name");
	//int test = kenken_valid(&kenken2);
	printf("%d\n", test);
	return 0;
}