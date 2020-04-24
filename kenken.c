#include<GUI-kenken.h>

int main( int argc, char* args[] )
{
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;
	
	//SDL_Texture* texture = NULL;
	
	SDL_Renderer* renderer = NULL;
	int t = (int) time(NULL);
	printf("%d\n", t);
	srand(t);
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Create window
        window = SDL_CreateWindow("Kenny's Ken Ken Ken-undrum", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if( window == NULL )
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
			renderer = SDL_CreateRenderer(window, -1, 0);
			if(TTF_Init()==-1) {
    			printf("TTF_Init: %s\n", TTF_GetError());
   		 		exit(2);
			}
			
			TTF_Font *font2;
			font2 = TTF_OpenFont(TYPEFACE, BIG_FONT);
			SDL_Rect texture_rect;
			texture_rect.x = 0;  //the x coordinate
			texture_rect.y = 0; // the y coordinate
			texture_rect.w = SCREEN_WIDTH; //the width of the texture
			texture_rect.h = SCREEN_HEIGHT; //the height of the texture	
			
			SDL_RenderSetScale(renderer, 1.0, 1.0);
			
			SDL_Point points[49];
			
			for(int i = 0; i < 49; i++){
				points[i].x = MARGIN + SQR_SIZE*(i%7);
				points[i].y = MARGIN + SQR_SIZE*(i/7);
			}
			
			//Squares for the game
			SDL_Rect rects[36];
			
			for(int i = 0; i < 36; i++){
				rects[i].x = MARGIN + SQR_SIZE*(i%6)+1;
				rects[i].y = MARGIN + SQR_SIZE*(i/6)+1;
				rects[i].w = SQR_SIZE-1;
				rects[i].h = SQR_SIZE-1;
			}
			
			//Box for displaying the status of the kenken
			/*SDL_Rect check_kk_btn;
			
			check_kk_btn.x = MARGIN + SQR_SIZE*7+3;
			check_kk_btn.y = MARGIN;
			check_kk_btn.w = 5*SQR_SIZE/2;
			check_kk_btn.h = SQR_SIZE;
			
			SDL_Point checkbtn_cnrs[5];
			checkbtn_cnrs[0].x = check_kk_btn.x-1;
			checkbtn_cnrs[0].y = check_kk_btn.y-1;
			checkbtn_cnrs[1].x = check_kk_btn.x+check_kk_btn.w;
			checkbtn_cnrs[1].y = check_kk_btn.y-1;
			checkbtn_cnrs[2].x = check_kk_btn.x+check_kk_btn.w;
			checkbtn_cnrs[2].y = check_kk_btn.y+check_kk_btn.h;
			checkbtn_cnrs[3].x = check_kk_btn.x-1;
			checkbtn_cnrs[3].y = check_kk_btn.y+check_kk_btn.h;
			checkbtn_cnrs[4].x = checkbtn_cnrs[0].x;
			checkbtn_cnrs[4].y = checkbtn_cnrs[0].y;
			*/
			
			struct button_w_border check_kk_btn;
			create_button_w_border(&check_kk_btn, MARGIN + SQR_SIZE*7+3, MARGIN, 5*SQR_SIZE/2, SQR_SIZE);
			SDL_Texture *chkbtntxt = NULL;
			int chkbtntxtdims[2];
			char *chkbtn = "Check Progress";
			draw_button_text(renderer, &chkbtntxt, chkbtntxtdims, chkbtn);
			
			struct button_w_border showkkstatus;
			create_button_w_border(&showkkstatus, MARGIN + SQR_SIZE*7+3, MARGIN + SQR_SIZE*2, 5*SQR_SIZE/2, SQR_SIZE);
			SDL_Texture *skkstatmsg1 = NULL;
			int statmsgdims1[2];
			char *msg1 = "On Track";
			draw_button_text(renderer, &skkstatmsg1, statmsgdims1, msg1);
			SDL_Texture *skkstatmsg2 = NULL;
			int statmsgdims2[2];
			char *msg2 = "Invalid Entry";
			draw_button_text(renderer, &skkstatmsg2, statmsgdims2, msg2);
			SDL_Texture *skkstatmsg3 = NULL;
			int statmsgdims3[2];
			char *msg3 = "You Solved It!";
			draw_button_text(renderer, &skkstatmsg3, statmsgdims3, msg3);
			
			struct button_w_border showkksolution;
			create_button_w_border(&showkksolution, MARGIN + SQR_SIZE*7+3, MARGIN + SQR_SIZE*4, 5*SQR_SIZE/2, SQR_SIZE);
			SDL_Texture *skksoltxt = NULL;
			int skkstxtdims[2];
			char *skksol = "Show Solution";
			draw_button_text(renderer, &skksoltxt, skkstxtdims, skksol);
			
			SDL_Rect corner_numbers[36]; //rect for displaying puzzle clues
			
			for(int i = 0; i < 36; i++){
				corner_numbers[i].x = MARGIN + SQR_SIZE*(i%6)+1;
				corner_numbers[i].y = MARGIN + SQR_SIZE*(i/6)+1;
				corner_numbers[i].w = 1;
				corner_numbers[i].h = 1;
			}
			
			//textures for the corner numbers
			SDL_Texture *textrects[36];
			for(int i = 0; i < 36; i++) textrects[i] = NULL;
			
			//textures for user entered numbers
			SDL_Texture *num_texts[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
			
			//array containing num_texts dimensions
			int txtboxdim[6][2];
			
			draw_central_number_textures(renderer, num_texts, txtboxdim);
			
			struct kenken game;
			
			generate_kenken(&game);
			struct kenken dmygame;
			copy_kenken(&game, &dmygame);
			
			for(int i = 0; i < 6; i++){
				for(int j = 0; j < 6; j++){
					game.grid[i][j] = 0;
				}
			}
			update_usr_kenken(&game);
			while(solve_kenken(&game) != 1){
				destroy_kenken(&game);
				generate_kenken(&game);
				copy_kenken(&game, &dmygame);
				for(int i = 0; i < 6; i++){
					for(int j = 0; j < 6; j++){
						game.grid[i][j] = 0;
					}
				}
				update_usr_kenken(&game);
			}
			copy_kenken(&dmygame, &game);
			destroy_kenken(&dmygame);
			
			int vertedge[5][6];
			
			int horiedge[6][5];
			
			for(int i = 0; i < 6; i++){
				for(int j = 0; j < 5; j++){
					vertedge[j][i] = 1;
					horiedge[i][j] = 1;
				}
			}
			
			struct node_ctrdraw *tlhead = NULL; //list of topleft squares of the constraints
			
			for(struct node_ctr *dmy = game.ctrs; dmy != NULL; dmy = dmy->next_node){
				struct node_ctrdraw *element = malloc(sizeof(struct node_ctrdraw));
				element->next_node = tlhead;
				element->result = dmy->constraint.result;
				element->op = dmy->constraint.op;
				element->topleft[0] = 5;
				element->topleft[1] = 5;
				
				for(struct node_square *dmy2 = dmy->constraint.numbers; dmy2 != NULL; dmy2 = dmy2->next_node){
					if(element->topleft[0] > dmy2->pos[0]){
						element->topleft[0] = dmy2->pos[0];
						element->topleft[1] = dmy2->pos[1];
					}
					else if(element->topleft[0] == dmy2->pos[0] && element->topleft[1] > dmy2->pos[1]){
						element->topleft[1] = dmy2->pos[1];
					}
				}
				
				tlhead = element;
			}
			
			
			struct kenken usrkk;
			copy_kenken(&game, &usrkk);
			
			for(int i = 0; i < 6; i++){
				for(int j = 0; j < 6; j++){
					usrkk.grid[i][j] = 0;
				}
			}
			
			update_usr_kenken(&usrkk);
			
			draw_corner_number_textures(renderer, tlhead, textrects, corner_numbers);
			
			set_kenken_boundaries(vertedge, horiedge, &game);
			
			SDL_Point corners[5] = {points[0], points[6], points[48], points[42], points[0]};
			
			//SDL_RenderCopy(renderer, texture, NULL, &texture_rect);
			//SDL_SetRenderTarget(renderer, texture);

            //Exit event
		    SDL_Event e;
			
		    int quit = 0;
			
			SDL_Rect *selected_sqr = &rects[0];
			
			int selected_index = 0;
			
			int check_msg_status = NONE;
			
			//SDL_RenderPresent(renderer);
			
			while(!quit)
		    {
                 SDL_WaitEvent( &e );
		         
		         if( e.type == SDL_QUIT )
		         {
		                  quit = 1;
						  break;
		         }
				 if( e.type == SDL_MOUSEBUTTONUP ){
				 	for(int i = 0; i < 36; i++){
				 		if( e.button.x > rects[i].x && e.button.x < rects[i].x + rects[i].w && e.button.y > rects[i].y && e.button.y < rects[i].y + rects[i].h){
				 			
							if((*selected_sqr).x == rects[i].x && (*selected_sqr).y == rects[i].y) break;
							
							//SDL_SetRenderDrawColor(renderer, 210, 210, 250, SDL_ALPHA_OPAQUE);
							
							selected_sqr = &rects[i];
							
							selected_index = i;

				 		}
				 	}
					if( e.button.x > check_kk_btn.btn.x && e.button.x < check_kk_btn.btn.x + check_kk_btn.btn.w && e.button.y > check_kk_btn.btn.y && e.button.y < check_kk_btn.btn.y + check_kk_btn.btn.h){
						if(valid_partial_kenken(usrkk)){
							if(kenken_valid(&usrkk)) check_msg_status = CORRECT;
							else{
								struct kenken dmykenken;
								copy_kenken(&usrkk, &dmykenken);
								if(solve_kenken(&dmykenken) == 1) check_msg_status = PARTCORRECT;
								else check_msg_status = INCORRECT;
								destroy_kenken(&dmykenken);
							}
						}
						else check_msg_status = INCORRECT;
					}
					if(e.button.x > showkksolution.btn.x && e.button.x < showkksolution.btn.x + showkksolution.btn.w && e.button.y > showkksolution.btn.y && e.button.y < showkksolution.btn.y + showkksolution.btn.h){
						copy_kenken(&game, &usrkk);
					}
				}
				if( e.type == SDL_KEYDOWN){
					switch(e.key.keysym.sym){
						case SDLK_UP:
						if(selected_index > 5){
							selected_index = selected_index - 6;
							selected_sqr = &rects[selected_index];
						}
						break;
						case SDLK_DOWN:
						if(selected_index < 30){
							selected_index = selected_index + 6;
							selected_sqr = &rects[selected_index];
						}
						break;
						case SDLK_LEFT:
						if(selected_index % 6 != 0){
							selected_index = selected_index - 1;
							selected_sqr = &rects[selected_index];
						}
						break;
						case SDLK_RIGHT:
						if(selected_index % 6 != 5){
							selected_index = selected_index + 1;
							selected_sqr = &rects[selected_index];
						}
						break;
						case SDLK_BACKSPACE:
						usrkk.grid[selected_index%6][selected_index/6] = 0;
						break;
						case SDLK_1:
						usrkk.grid[selected_index%6][selected_index/6] = 1;
						break;
						case SDLK_2:
						usrkk.grid[selected_index%6][selected_index/6] = 2;
						break;
						case SDLK_3:
						usrkk.grid[selected_index%6][selected_index/6] = 3;
						break;
						case SDLK_4:
						usrkk.grid[selected_index%6][selected_index/6] = 4;
						break;
						case SDLK_5:
						usrkk.grid[selected_index%6][selected_index/6] = 5;
						break;
						case SDLK_6:
						usrkk.grid[selected_index%6][selected_index/6] = 6;
						break;
					}
					
					update_usr_kenken(&usrkk);
				
				}
				
				draw_function(window, renderer, &texture_rect, corners, points, rects, selected_sqr, vertedge, horiedge);
				
				for(int i = 0; i < 36; i++){
					if(textrects[i] != NULL){
						SDL_RenderCopy(renderer, textrects[i], NULL, &corner_numbers[i]);
					}
				}
				
				if(check_msg_status == CORRECT) draw_button(renderer, skkstatmsg3, &showkkstatus, statmsgdims3);
				else if(check_msg_status == PARTCORRECT) draw_button(renderer, skkstatmsg1, &showkkstatus, statmsgdims1);
				else if(check_msg_status == INCORRECT) draw_button(renderer, skkstatmsg2, &showkkstatus, statmsgdims2);
				
				draw_button(renderer, skksoltxt, &showkksolution, skkstxtdims);
				draw_button(renderer, chkbtntxt, &check_kk_btn, chkbtntxtdims);
				
				draw_central_numbers(renderer, num_texts, rects, txtboxdim, usrkk.grid);
				
				/*if(valid_partial_kenken(usrkk)){
					SDL_SetRenderDrawColor(renderer, 10, 210, 10, SDL_ALPHA_OPAQUE);
				}
				else{
					SDL_SetRenderDrawColor(renderer, 210, 10, 10, SDL_ALPHA_OPAQUE);
				}*/
				
				SDL_RenderPresent(renderer);
		    }
        }
    }
	//Destroy renderer
	SDL_DestroyRenderer(renderer);
	
	//TTF_CloseFont(font2);
	
	TTF_Quit();
	
    //Destroy window
    SDL_DestroyWindow( window );
	
    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}