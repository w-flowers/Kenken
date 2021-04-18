#include "../inc/menu.h"

int run_menu(struct run_menu_args* args)
{
   const int MENU_WIDTH = 5*SQR_SIZE/2 + 2 * MARGIN;

   const int MENU_HEIGHT = 5*SQR_SIZE + 2 * MARGIN;

   SDL_Window* window = NULL;

   SDL_Renderer* renderer = NULL;

   window = SDL_CreateWindow("Mathdoku",
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

            rlm_args.typeface = args->typeface;

            rlm_args.game = args->game;

            rlm_args.usrkk = args->usrkk;

            rlm_args.window = window;

            rlm_args.renderer = renderer;

            rlm_args.window_rect = &window_rect;

            if( quit = run_load_menu( &rlm_args ) )
            {
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
   if( !args )
   {
      fprintf( stderr, "Run load menu called with NULL argument\n");

      return 1;
   }

   struct load_menu menu;

   int quit = 0;

   if( !init_load_menu( args->renderer, &menu, args->typeface ) )
   {
      struct SDL_Rect menu_squares[LMV_SIZE];

      // Init squares for menu
      for( int i = 0; i < LMV_SIZE; i++ )
      {
         menu_squares[i].x = MARGIN;

         menu_squares[i].y = MARGIN + i*SQR_SIZE / 2;

         menu_squares[i].w = 5*SQR_SIZE / 2;

         menu_squares[i].h = SQR_SIZE / 2 - 1;
      }

      struct SDL_Point menu_corners[ 2*LMV_SIZE + 2 ];

      // Init corners to for rendering dividing lines
      for( int i = 0; i < LMV_SIZE; i++ )
      {
         menu_corners[ 2*i ].x = menu_squares[i].x - 1;

         menu_corners[ 2*i ].y = menu_squares[i].y - 1;

         menu_corners[ 2*i + 1 ].x = menu_squares[i].x + menu_squares[i].w;

         menu_corners[ 2*i + 1 ].y = menu_squares[i].y - 1;
      }

      menu_corners[ 2*LMV_SIZE ].x = menu_squares[LMV_SIZE - 1].x - 1;

      menu_corners[ 2*LMV_SIZE ].y =
         menu_squares[LMV_SIZE - 1].y + menu_squares[LMV_SIZE - 1].h;

      menu_corners[ 2*LMV_SIZE + 1 ].x = menu_squares[LMV_SIZE - 1].x + 
         menu_squares[LMV_SIZE - 1].w;

      menu_corners[ 2*LMV_SIZE + 1 ].y = 
         menu_squares[LMV_SIZE - 1].y + menu_squares[LMV_SIZE - 1].h;

      //Button that loads a kenken
      struct button_w_border loadkk;
      create_button_w_border(&loadkk, MARGIN, 
            MARGIN + (2*LMV_SIZE+1)*SQR_SIZE/4,
            5*SQR_SIZE/2, SQR_SIZE);

      SDL_Texture *loadkktxt = NULL;
      int loadkktxtdims[2];
      char *lkkt = "Load Kenken";
      draw_button_text( args->renderer, &loadkktxt, loadkktxtdims, lkkt );

      //Button which goes back to main menu
      struct button_w_border back;
      create_button_w_border(&back, MARGIN, MARGIN + (LMV_SIZE+3)*SQR_SIZE/2,
            5*SQR_SIZE/2, SQR_SIZE);

      SDL_Texture *backtxt = NULL;
      int backtxtdims[2];
      char *backt = "Back";
      draw_button_text( args->renderer, &backtxt, backtxtdims, backt );

      struct menu_view view;

      view.items = &menu.buffer[0];

      view.pos_top = 0;

      view.pos_bottom = LMV_SIZE <= menu.offset ?
         LMV_SIZE : menu.offset;

      // Render menu so that user doesn't have to input something for it to 
      // appear
      SDL_SetRenderDrawColor( args->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE );
      SDL_RenderFillRect( args->renderer, args->window_rect );

      render_load_menu( args->renderer, &view, &menu, menu_squares, menu_corners );

      draw_button( args->renderer, loadkktxt, &loadkk, loadkktxtdims );
      draw_button( args->renderer, backtxt, &back, backtxtdims );

      SDL_RenderPresent( args->renderer );

      SDL_Event e;

      //Menu loop in here
      while(!quit)
      {
         SDL_WaitEvent( &e );

         if( e.type == SDL_QUIT )
         {
            quit = QUIT_MENU;
            break;
         }

         else if( e.type == SDL_KEYDOWN )
         {
            if( ( quit = handle_key_event( &e, &menu, &view ) ) ) break;
         }

         else if( e.type == SDL_MOUSEBUTTONUP )
         {
            if( ( quit = 
                     handle_mouse_event( &e, &menu, &view, menu_squares, 
                       back.btn, loadkk.btn ) )
              ) break;
         }

         //Colour the screen
         SDL_SetRenderDrawColor( args->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE );
         SDL_RenderFillRect( args->renderer, args->window_rect );

         render_load_menu( args->renderer, &view, &menu, menu_squares, menu_corners );

         draw_button( args->renderer, loadkktxt, &loadkk, loadkktxtdims );
         draw_button( args->renderer, backtxt, &back, backtxtdims );

         SDL_RenderPresent( args->renderer );
      }

      destroy_button_text( &loadkktxt );

      if( quit == LOADING )
      {
         //insert loading code here

         // Make filename string

         int dmyusrgrid[6][6] = {0};

         if( load_kenken( args->game,
                  dmyusrgrid, menu.buffer[menu.current].filename ) )
         {
            fprintf( stderr, "Error opening file!\n" );

            quit = 0;
         }

         else
         {
            copy_kenken( args->game, args->usrkk );

            for( int i = 0; i < 36; i++ )
            {
               args->usrkk->grid[i%6][i/6] = dmyusrgrid[i%6][i/6];
            }

            update_usr_kenken( args->usrkk );
         }
      }

      if( quit == TO_MENU )
      {
         quit = 0;
      }
   }

   destroy_load_menu( &menu );

   return quit;
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

   if( snprintf(dir_buff, sizeof(dir_buff), "%s/%s", FILEPATH, "savegames") >= sizeof(dir_buff) )
   {
      fprintf(stderr, "Filepath too long!!!\n");
      return 1;
   }

   DIR *dir = opendir( dir_buff );

   if( !dir )
   {
      fprintf( stderr, "Error opening directory %s!\n", dir_buff );
      return 1;
   }

   while( ( direntry = readdir(dir) ) )
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

      if( !create_load_menu_item( renderer, &menu->buffer[menu->offset],
               direntry, typeface ) )
      {
         menu->offset++;
      }
   }

   return 0;
}


int create_load_menu_item( SDL_Renderer *renderer, struct load_menu_item *item, 
      struct dirent *entry, char *typeface )
{
   if( snprintf( item->filename, FILENM_SIZE, entry->d_name ) >= FILENM_SIZE )
   {
      fprintf( stderr, "Filename too long\n" );

      return 1;
   }

   char *dmyptr = item->filename;

   if( entry->d_name[0] == '.' ) return 1;

   while( *(++dmyptr) != '.' );

   *dmyptr = '\0';

   SDL_Color colour = { 0, 0, 0, 255 };

   TTF_Font *font;

   font = TTF_OpenFont( typeface, 12 );

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

               view->pos_bottom = ( view->pos_top + LMV_SIZE <= menu->offset ) ?
                  ( view->pos_top + LMV_SIZE ) : menu->offset; 
            }
         }

         return 0;

      case SDLK_DOWN:
         if( menu->current < menu->offset - 1 )
         {
            menu->current++;

            if( view->pos_bottom <= menu->current )
            {
               view->pos_bottom = menu->current + 1;

               view->pos_top = view->pos_bottom - LMV_SIZE;

               view->items = &menu->buffer[view->pos_top];
            }
         }

         return 0;

      case SDLK_RETURN:
         // Return LOADING - Loads currently selected square
         return LOADING;
   }
   return 0;
}

int handle_mouse_event( SDL_Event *e, struct load_menu *menu, 
      struct menu_view *view, SDL_Rect menu_squares[],
     SDL_Rect back, SDL_Rect load )
{
   // Handle case where user clicks on one of the load_menu_items
   for( int i = 0; i < LMV_SIZE; i++ )
   {
      if( e->button.x > menu_squares[i].x && e->button.y > menu_squares[i].y
            && e->button.x < menu_squares[i].x + menu_squares[i].w
            && e->button.y < menu_squares[i].y + menu_squares[i].h )
      {
         menu->current = view->pos_top + i;
      }
   }

   // Handle case where user clicks on the load button
   if( e->button.x > load.x && e->button.y > load.y
         && e->button.x < load.x + load.w && e->button.y < load.y + load.h )
   {
      return LOADING;
   }

   // Handle case where user clicks on the back button
   if( e->button.x > back.x && e->button.y > back.y
         && e->button.x < back.x + back.w && e->button.y < back.y + back.h )
   {
      return TO_MENU;
   }

   return 0;
}

int render_load_menu( SDL_Renderer *renderer, struct menu_view *view,
      struct load_menu *menu, SDL_Rect menu_squares[],
      SDL_Point menu_corners[] )
{
   SDL_SetRenderDrawColor( renderer, 245, 245, 245, SDL_ALPHA_OPAQUE );
   SDL_RenderFillRects( renderer, menu_squares, LMV_SIZE );

   SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
   SDL_RenderDrawPoints( renderer, menu_corners, 2*LMV_SIZE + 2 );

   for( int i = 0; i < LMV_SIZE; i++ )
   {
      SDL_RenderDrawLine( renderer, menu_corners[2*i].x, menu_corners[2*i].y,
            menu_corners[2*i + 1].x, menu_corners[2*i + 1].y);

      SDL_RenderDrawLine( renderer, menu_corners[2*i].x, menu_corners[2*i].y,
            menu_corners[2*i + 2].x, menu_corners[2*i + 2].y);
      
      SDL_RenderDrawLine( renderer, menu_corners[2*i + 1].x,
            menu_corners[2*i + 1].y, menu_corners[2*i + 3].x,
            menu_corners[2*i + 3].y);
   }

   SDL_RenderDrawLine( renderer, menu_corners[2*LMV_SIZE].x,
          menu_corners[2*LMV_SIZE].y, menu_corners[2*LMV_SIZE + 1].x,
          menu_corners[2*LMV_SIZE + 1].y );


   SDL_SetRenderDrawColor( renderer, 210, 210, 250, SDL_ALPHA_OPAQUE );
   SDL_RenderFillRect( renderer, &menu_squares[menu->current - view->pos_top] );

   draw_loadmenu_text( renderer, menu, view, menu_squares );

   return 0;
}

int draw_loadmenu_text( SDL_Renderer *renderer, struct load_menu *menu,
      struct menu_view *view, SDL_Rect menu_squares[] )
{
   for( int i = 0; i < LMV_SIZE; i++ )
   {
      if( view->pos_top + i < menu->offset )
      {
         SDL_Rect txtbox;

         int c_x = menu_squares[i].x + menu_squares[i].w/2;
         int c_y = menu_squares[i].y + menu_squares[i].h/2;

         txtbox.x = c_x - view->items[i].text_dims[0]/2;
         txtbox.y = c_y - view->items[i].text_dims[1]/2;

         txtbox.w = view->items[i].text_dims[0];
         txtbox.h = view->items[i].text_dims[1];

         SDL_RenderCopy( renderer, view->items[i].text, NULL, &txtbox ); 
      }
   }

   return 0;
}

