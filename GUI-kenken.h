#ifndef GUIKENKEN
#define GUIKENKEN

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include"kenken-solver.h"

#define NONE 0
#define INCORRECT 1
#define PARTCORRECT 2
#define CORRECT 3
#define TYPEFACE "OpenSans-Regular.ttf"

//Screen dimension constants
const int SCREEN_WIDTH = 780;
const int SCREEN_HEIGHT = 600;
const int MARGIN = 30;
const int SQR_SIZE = 72;
const int SOLUTION_BOX = 15;
const int SMALL_FONT = 14;
const int MEDIUM_FONT = 22;
const int BIG_FONT = 60;

int update_edge_arrays(int vertedge[5][6], int horiedge[6][5], struct constraint *cstr);

int set_kenken_boundaries(int vertedge[5][6], int horiedge[6][5], struct kenken* kenkenptr);

int draw_function(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect *text_rect_ptr, SDL_Point corners[5], SDL_Point points[49], SDL_Rect rects[36], SDL_Rect *selected_sqr, int vertedge[5][6], int horiedge[6][5]);

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

int create_button_w_border(struct button_w_border* button, int x, int y, int w, int h);

int draw_corner_number_textures(SDL_Renderer *renderer, struct node_ctrdraw *tlhead, SDL_Texture *textrects[36], SDL_Rect corner_numbers[36]);

int draw_central_number_textures(SDL_Renderer *renderer, SDL_Texture *num_texts[6], int txtboxdims[6][2]);

int draw_central_numbers(SDL_Renderer *renderer, SDL_Texture *num_texts[6], SDL_Rect rects[36], int txtboxdims[6][2], int usrgrid[6][6]);

int draw_button_text(SDL_Renderer *renderer, SDL_Texture **text_texture, int txtbox_dims[2], char *text);

int draw_button(SDL_Renderer *renderer, SDL_Texture *button_text, struct button_w_border *button, int btn_txt_dims[2]);

#endif