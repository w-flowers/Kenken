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

//Struct which keeps a string of filename and the texture info for rendering.
struct load_menu_item
{
   char filename[FILENM_SIZE];

   SDL_Texture *text;

   int text_dims[2];
};

// Struct which stores an array of Load_Menu_Items and keeps track of its size.
// Also keeps track of which menu item is selected.
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

// Struct accesses elements of load_menu through pointers to &buffer[pos_top].
// pos_bottom is 1 index past the last element to be displayed.
struct menu_view
{
   int pos_top;

   int pos_bottom;

   struct load_menu_item *items;
};

// Function which runs the initial menu on opening.
// Returns either GO_TO_GAME or QUIT_MENU, which is passed to main.
// Function also calls run_load_menu, which can return LOADING, which is also
// passed to main.
int run_menu(struct run_menu_args* args);

// Function which opens the loading menu
// Either returns LOADING, which means it has populated the passed in pointers,
// or it returns to the menu with TO_MENU, or it quits if the corner x is used
int run_load_menu(struct load_menu_args* args);

// Initialises load menu, creating a load menu item for each entry in the
// directory savegames. This will return 1 if savegames does not exist.
// Allocates memory which must be freed in destroy load menu.
int init_load_menu( SDL_Renderer *renderer, struct load_menu *menu, 
      char *typeface );

// Takes a directory entry, copies its name (less .kenken) into the
// load_menu_item, and renders a texture to store in said items, alongside its
// dimensions.
int create_load_menu_item( SDL_Renderer *renderer, struct load_menu_item *item, 
      struct dirent *entry, char *typeface );

// Deallocates memory for SDL_Textures for each Load_menu_item, before freeing
// the variable length buffer in load_menu.
int destroy_load_menu( struct load_menu *menu );

// Handles keyboard input. Enter returns a LOADING, which is passed through
// to run_load_menu, which loads the file given by the current menu item, or
// moves the current menu item up or down. If the menu item is at the end of the
// menu view, this function will shift the menu view to include the current
// index.
int handle_key_event( SDL_Event *e, struct load_menu *menu, 
      struct menu_view *view );

// Handles mouse input. Allows user to click on a square in the menu_view in
// order to change the current menu item, click on LOAD KENKEN to return
// LOADING, (see previous comment), or click BACK to return to the main menu.
int handle_mouse_event( SDL_Event *e, struct load_menu *menu,
      struct menu_view *view, SDL_Rect menu_squares[],
     SDL_Rect back, SDL_Rect load );

// Render objects to screen. Uses menu_view to determine which menu items to
// render.
int render_load_menu( SDL_Renderer *renderer, struct menu_view *view,
      struct load_menu *menu, SDL_Rect menu_squares[],
      SDL_Point menu_corners[] );

// Helper function for render_load_menu
int draw_loadmenu_text( SDL_Renderer *renderer, struct load_menu *menu,
      struct menu_view *view, SDL_Rect menu_squares[] );

#endif
