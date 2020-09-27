#ifndef GUIKENKEN_H_
#define GUIKENKEN_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "kenken-solver.h"

#define NONE 0
#define INCORRECT 1
#define PARTCORRECT 2
#define CORRECT 3
#define TYPEFACE "OpenSans-Regular.ttf"

//Screen dimension constants
const int MARGIN = 50;
const int SQR_SIZE = 72;
const int SCREEN_WIDTH = 10*SQR_SIZE + 2*MARGIN;
const int SCREEN_HEIGHT = 8*SQR_SIZE + 2*MARGIN;
const int SMALL_FONT = 14;
const int MEDIUM_FONT = 22;
const int BIG_FONT = 60;

//Takes a single constraint and determines which edges of each square are internal to this 
//constraint. If an edge is internal, the corresponding int in vert/horiedge is set to 0.

int update_edge_arrays( int vertedge[5][6], int horiedge[6][5], struct constraint *cstr );


//A function which walks through the kenken's constraints and calls update_edge_array on each.

int set_kenken_boundaries( int vertedge[5][6], int horiedge[6][5], struct kenken* kenkenptr );


//A function that draws the background, grid, and selected square

int draw_function( SDL_Window *window, SDL_Renderer *renderer, SDL_Rect *text_rect_ptr, 

	SDL_Point corners[5], SDL_Point points[49], SDL_Rect rects[36], SDL_Rect *selected_sqr, 
	
	int vertedge[5][6], int horiedge[6][5] );


struct node_ctrdraw{
	int result;				//the result of the operation
	int op;					//the operation: defined by preprocessor in kenken.c
	int topleft[2];			//the position of the topleftmost square
	struct node_ctrdraw *next_node;
};


struct button_w_border{
	SDL_Rect btn;
	SDL_Point crnrs[5];
};


//Takes the position and size arguments, and a pointer to a button, and creates it.

int create_button_w_border( struct button_w_border* button, int x, int y, int w, int h );


//creates a list of the top-left squares of each constraint, which starts with tlhead

int create_cnr_nums_list( struct node_ctrdraw **tlhead, struct kenken game );


//Creates a texture for each corner number, and puts a pointer to it into the array textrects
//positional information for where to render these corner numbers is put into corner_numbers
//Must call destroy_corner_number_textures when done

int draw_corner_number_textures( SDL_Renderer *renderer, struct node_ctrdraw *tlhead, 
	
	SDL_Texture *textrects[36], SDL_Rect corner_numbers[36] );


//Frees textsrects
	
int destroy_corner_number_textures( SDL_Texture *textrects[36] );


//misnomer, destructor for linked list pointed to by tlhead

int free_corner_number_textures( struct node_ctrdraw **tlhead );


//Draws a texture for the numbers 1-6 for use later
//Must call destroy_central_number_textures when done

int draw_central_number_textures( SDL_Renderer *renderer, SDL_Texture *num_texts[6], int txtboxdims[6][2] );


//Frees num_texts

int destroy_central_number_textures( SDL_Texture *num_texts[6] );


//Renders the textures created by draw_central_number_textures to the appropriate square, based on
//the content of usrgrid.

int draw_central_numbers( SDL_Renderer *renderer, SDL_Texture *num_texts[6], 
	
	SDL_Rect rects[36], int txtboxdims[6][2], int usrgrid[6][6] );


//Renders the text content of a button.
//must call destroy_button_text on text_texture once done.
	
int draw_button_text( SDL_Renderer *renderer, SDL_Texture **text_texture, 
	
	int txtbox_dims[2], char *text );


int destroy_button_text( SDL_Texture **text_texture );


//Draws a button, and copies the rendered text from draw_button_text to it.

int draw_button( SDL_Renderer *renderer, SDL_Texture *button_text, 
	
	struct button_w_border *button, int btn_txt_dims[2] );

#endif
