#include "../inc/menu.h"

int run_menu(struct run_menu_args* args)
{
   const int MENU_WIDTH = 5*SQR_SIZE/2 + 2 * MARGIN;

   const int MENU_HEIGHT = 5*SQR_SIZE + 2 * MARGIN;

   SDL_Window* window = NULL;

   SDL_Renderer* renderer = NULL;

   window = SDL_CreateWindow("Kenny's Ken Ken Ken-undrum",
         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
         MENU_WIDTH, MENU_HEIGHT, SDL_WINDOW_SHOWN);

   if( window == NULL )
   {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());

      return 1;
   }

   renderer = SDL_CreateRenderer(window, -1, 0 );

   if(TTF_Init()==-1) 
   {
      printf("TTF_Init: %s\n", TTF_GetError());

      exit(2);
   }

   //Rectangle for whole screen
   SDL_Rect window_rect;
   window_rect.x = 0;  //the x coordinate
   window_rect.y = 0; // the y coordinate
   window_rect.w = MENU_WIDTH; //the width of the texture
   window_rect.h = MENU_HEIGHT; //the height of the texture	

   //Button that opens a new kenken
   struct button_w_border newkk;
   create_button_w_border(&newkk, MARGIN, MARGIN, 5*SQR_SIZE/2, SQR_SIZE);

   SDL_Texture *newkktxt = NULL;
   int newkktxtdims[2];
   char *nkkt = "New Kenken";
   draw_button_text(renderer, &newkktxt, newkktxtdims, nkkt);

   //Button that loads a kenken
   struct button_w_border loadkk;
   create_button_w_border(&loadkk, MARGIN, MARGIN + 2*SQR_SIZE, 5*SQR_SIZE/2, SQR_SIZE);

   SDL_Texture *loadkktxt = NULL;
   int loadkktxtdims[2];
   char *lkkt = "Load Kenken";
   draw_button_text(renderer, &loadkktxt, loadkktxtdims, lkkt);

   //Button which quits the program
   struct button_w_border quitkk;
   create_button_w_border(&quitkk, MARGIN, MARGIN + SQR_SIZE*4, 5*SQR_SIZE/2, SQR_SIZE);

   SDL_Texture *quitkktxt = NULL;
   int quitkktxtdims[2];
   char *qkkt = "Quit Kenken";
   draw_button_text(renderer, &quitkktxt, quitkktxtdims, qkkt);

   SDL_Event e;

   int quit = 0;

   while(!quit)
   {
      SDL_WaitEvent( &e );

      if( e.type == SDL_QUIT )
      {
         quit = QUIT_MENU;
         break;
      }
      if( e.type == SDL_MOUSEBUTTONUP ){

         //Click on Quit Kenken button
         if( e.button.x > quitkk.btn.x && e.button.x < quitkk.btn.x + quitkk.btn.w &&
               e.button.y > quitkk.btn.y && e.button.y < quitkk.btn.y + quitkk.btn.h ){

            quit = QUIT_MENU;
            break;

         }

         //Click on New Kenken button
         if( e.button.x > newkk.btn.x && e.button.x < newkk.btn.x + newkk.btn.w && 
               e.button.y > newkk.btn.y && e.button.y < newkk.btn.y + newkk.btn.h ){

            quit = GO_TO_GAME;
            break;
         }

         //Click on Load Kenken button
         if( e.button.x > loadkk.btn.x && e.button.x < loadkk.btn.x + loadkk.btn.w && 
               e.button.y > loadkk.btn.y && e.button.y < loadkk.btn.y + loadkk.btn.h ){

            struct load_menu_args rlm_args;

            rlm_args.typeface = args.typeface;

            rlm_args.game = args.game;

            rlm_args.usrkk = args.usrkk;

            rlm_args.window = window;

            rlm_args.renderer = renderer;

            rlm_arg.window_rect = &window_rect;

            if( !run_load_menu( &rlm_args ) )
            {
               quit = LOADING;
               break;
            }
         }
      }
      //End handling mouse click

      //Colour the screen
      SDL_SetRenderDrawColor( renderer, 255, 255, 255, SDL_ALPHA_OPAQUE );
      SDL_RenderFillRect( renderer, &window_rect );

      //draw other buttons
      draw_button( renderer, quitkktxt, &quitkk, quitkktxtdims );
      draw_button( renderer, newkktxt, &newkk, newkktxtdims );
      draw_button( renderer, loadkktxt, &loadkk, loadkktxtdims );

      SDL_RenderPresent( renderer );
   }
   //Cleanup
   TTF_Quit();

   destroy_button_text( &quitkktxt );
   destroy_button_text( &newkktxt );
   destroy_button_text( &loadkktxt );

   SDL_DestroyRenderer( renderer );

   SDL_DestroyWindow( window );

   return quit;
}

int run_load_menu(struct load_menu_args* args)
{
   if( !args ) return 1;

   struct load_menu menu;

   if( !init_load_menu( args->renderer, &menu, args->typeface ) )
   {
      //Menu loop in here
      while(!quit)
      {
         SDL_WaitEvent( &e );

         if( e.type == SDL_QUIT )
         {
            quit = QUIT_MENU;
            break;
         }

         handle_key_event( &e );

         handle_mouse_event( &e );

         render_menu();
      }
   }



   destroy_load_menu( &menu );

   return 0;
}

int init_load_menu( SDL_Renderer *renderer, struct load_menu *menu, 
      char *typeface )
{
   menu->buffer = malloc( sizeof( struct load_menu_item ) * 32 );

   if( !( menu->buffer ) ) return 1;

   menu->current = 0;

   menu->size = 32;

   menu->offset = 0;

   struct dirent *direntry = NULL;

   char dir_buff[FILENM_SIZE];

   if(snprintf(dir_buff, sizeof(dir_buff), "%s/%s", FILEPATH, "savegames") >= sizeof(dir_buff))
   {
      fprintf(stderr, "Filepath too long!!!\n");
      return 1;
   }

   DIR *dir = opendir( dir_buff );

   while( direntry = readdir(dir) )
   {
      // Resize array if necessary
      if( menu->offset == menu->size )
      {
         struct load_menu_item *dmy = 
            malloc( sizeof( struct load_menu_item ) * menu->size * 4 );

         if( !dmy ) return 1;

         memcpy( dmy, menu->buffer, menu->size*sizeof( struct load_menu_item ) );

         free( menu->buffer );

         menu->buffer = dmy;

         menu->size *= 4;
      }

      if( !create_load_menu_item( renderer, &menu->buffer[offset],
               direntry, typeface ) )
      {
         offset++;
      }
   }

   return 0;
}


int create_load_menu_item( SDL_Renderer *renderer, struct load_menu_item *item, 
      struct dirent *entry, char *typeface )
{
   if( snprint( item->filename, FILENM_SIZE, entry->d_name ) >= FILENM_SIZE )
   {
      fprintf( stderr, "Filename too long\n" );

      return 1;
   }

   SDL_Color colour = { 0, 0, 0, 255 };

   TTF_Font *font;

   font = TTF_OpenFont( typeface, MEDIUM_FONT );

   SDL_Surface *textsurf;

   textsurf = TTF_RenderText_Blended( font, item->filename, colour );

   item->text = SDL_CreateTextureFromSurface( renderer, textsurf );

   item->text_dims[0] = textsurf->w;
   item->text_dims[1] = textsurf->h;

   SDL_FreeSurface( textsurf );

   TTF_CloseFont( font );

   return 0;
}

int destroy_load_menu( struct load_menu *menu )
{
   for( int i = 0; i < menu->offset; i++ )
   {
      SDL_DestroyTexture( menu->buffer[i].text );

      menu->buffer[i].text = NULL;
   }

   free( menu->buffer );

   menu -> size = 0;

   return 0;
}

int handle_key_event( SDL_Event *e, struct load_menu *menu, 
      struct menu_view *view )
{
   switch( e->key.keysym.sym )
   {
      case SDLK_UP:
         if( menu->current > 0 )
         {
            menu->current--;

            if( view->pos_top > menu->current )
            {
               view->items = &menu->buffer[menu->current];

               view->pos_top = menu->current;

               view->pos_bottom = ( pos->top + LMV_SIZE <= menu->offset ) ?
                  ( pos->top + LMV_SIZE ) : menu->offset; 
            }
         }

         break;

      case SDLK_DOWN:
         if( menu->current < menu->offset - 1 )
         {
            menu->current++;

            if( view->pos_bottom <= menu->current )
            {
               view->pos_bottom = menu->current + 1;

               view->pos_top = view->pos_bottom - 5;

               view->items = &menu->buffer[pos_top];
            }
         }

         break;

      case SDLK_RETURN:
         //load the current menu item to the pointers in args OR set a flag

         break;
   }
   return 0;
}

int handle_mouse_event( SDL_Event *e, struct load_menu *menu );

