#include "kenken.c"

//vertedge and horiedge are indexed according to the square above/left of them
int update_edge_arrays(int vertedge[5][6], int horiedge[6][5], struct constraint *cstr){
	for(struct node_square *ele1 = cstr->numbers; ele1 != NULL; ele1 = ele1->next_node){
		for(struct node_square *ele2 = ele1->next_node; ele2 != NULL; ele2 = ele2->next_node){
			if(ele1->pos[0] == ele2->pos[0] && ele1->pos[1] == ele2->pos[1] - 1){
				horiedge[ele1->pos[0]][ele1->pos[1]] = 0;
			}
			if(ele1->pos[0] == ele2->pos[0] && ele1->pos[1] == ele2->pos[1] + 1){
				horiedge[ele1->pos[0]][ele2->pos[1]] = 0;
			}
			if(ele1->pos[0] == ele2->pos[0] - 1 && ele1->pos[1] == ele2->pos[1]){
				vertedge[ele1->pos[0]][ele2->pos[1]] = 0;
			}
			if(ele1->pos[0] == ele2->pos[0] + 1 && ele1->pos[1] == ele2->pos[1]){
				vertedge[ele2->pos[0]][ele2->pos[1]] = 0;
			}
		}
	}
	return 1;
}

//function to update the arrays of vertical and horizontal edges to be drawn
int draw_kenken_boundaries(int vertedge[5][6], int horiedge[6][5], struct kenken* kenkenptr){
	for(struct node_ctr *dmy = kenkenptr->ctrs; dmy != NULL; dmy = dmy->next_node){
		update_edge_arrays(vertedge, horiedge, dmy->constraint);
	}
	return 1;
}