#ifndef MENU_H_
#define MENU_H_

#include "GUI-kenken.h"

#include <dirent.h>

#define QUIT_MENU 1
#define GO_TO_GAME 2
#define LOADING 3
#define TO_MENU 4

#define FILENM_SIZE 256

//size of menu_view to render
#define LMV_SIZE 5

struct run_menu_args
{
   char *typeface;

   struct kenken *game;

   struct kenken *usrkk;
};

struct load_menu_args
{
   char *typeface;

   struct kenken *game;

   struct kenken *usrkk;

   SDL_Window *window;

   SDL_Renderer *renderer;

   SDL_Rect *window_rect;
};

struct load_menu_item
{
   char filename[FILENM_SIZE];

   SDL_Texture *text;

   int text_dims[2];
};

struct load_menu
{
   // current selected entry
   int current;

   // current position to enter new element
   int offset;

   //size of array
   int size;

   struct load_menu_item *buffer;
};

struct menu_view
{
   int pos_top;

   int pos_bottom;

   struct load_menu_item *items;
};

int run_menu(struct run_menu_args* args);

int run_load_menu(struct load_menu_args* args);

int init_load_menu( SDL_Renderer *renderer, struct load_menu *menu, 
      char *typeface );

int create_load_menu_item( SDL_Renderer *renderer, struct load_menu_item *item, 
      struct dirent *entry, char *typeface );

int destroy_load_menu( struct load_menu *menu );

int handle_key_event( SDL_Event *e, struct load_menu *menu, 
      struct menu_view *view );

int handle_mouse_event( SDL_Event *e, struct load_menu *menu,
      struct menu_view *view, SDL_Rect menu_squares[],
     SDL_Rect back, SDL_Rect load );

int render_load_menu( SDL_Renderer *renderer, struct menu_view *view,
      struct load_menu *menu, SDL_Rect menu_squares[],
      SDL_Point menu_corners[] );

int draw_loadmenu_text( SDL_Renderer *renderer, struct load_menu *menu,
      struct menu_view *view, SDL_Rect menu_squares[] );

#endif
