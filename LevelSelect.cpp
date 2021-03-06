#include "Classes.h"
#include "Functions.h"
#include "Globals.h"
#include "Objects.h"
#include <sstream>
#include <SDL_ttf.h>
#include <SDL.h>
#include <iostream>

////////////////////NUMBER////////////////////////
Number::Number( )
{
	s_image = NULL;
	s_level = NULL;

	myBox.x = 0; myBox.y = 0; myBox.h = 50; myBox.w = 50;
}

Number::~Number()
{
        SDL_FreeSurface(s_image);     
        SDL_FreeSurface(s_level);    
}

void Number::init(int number, SDL_Rect box )
{
	if ( o_mylevels.get_locked(number) == false )
	{
		s_level = load_image("data/gfx/level.png");
	}

	else { s_level = load_image("data/gfx/levellocked.png"); }
                
	std::stringstream text;
                
	text << number;
                
	SDL_Color black = { 0,0,0 };
                
	s_image = TTF_RenderText_Solid(font, text.str().c_str(), black);
                
    myBox.x = box.x; myBox.y = box.y; myBox.h = 50; myBox.w = 50; 
}

void Number::show()
{
     apply_surface( myBox.x,myBox.y, s_level, screen,NULL );
     
     apply_surface( (myBox.x + 25 - (s_image->w / 2)), (myBox.y + 25 - (s_image->h / 2)), s_image, screen, NULL );
}


/////////////////////LEVEL SELECT/////////////////////
LevelSelect::LevelSelect()
{
                          s_background = load_image("data/gfx/levelselect.png");    
                          

                                                   
                          for ( int n = 0; n < o_mylevels.get_level_number(); n++ )
                          {
								 o_number.push_back( Number () );
								
                              
                            
                          }     

						  lol = 0;
						  titleA = 250;
}

LevelSelect::~LevelSelect()
{
                           SDL_FreeSurface(s_background);                        
}

void LevelSelect::handle_events()
{
     while ( SDL_PollEvent(&event) )
     {
         if ( event.type == SDL_QUIT )
		 {
			 next_state(STATE_EXIT);
		 }

		 if ( event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT )
		 {
				check_mouse();
		 }

		 if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE )
		 {
			 next_state(STATE_MENU);
		 }

		 if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_s )
			{
				if ( Mix_PlayingMusic() == 1 )
				{
					Mix_HaltMusic();
				}

				else 
				{
					Mix_PlayMusic(music,-1);
				}				
			}
     }
}

void LevelSelect::check_mouse()
{
	int x, y;

	SDL_GetMouseState(&x,&y);

	SDL_Rect mouse = { x,y,5,5};

	for ( int n = 0; n < (signed)o_mylevels.get_level_number(); n++ )
	{
		if ( o_mylevels.get_locked(n) == false )
		{
			if ( check_collision( mouse, o_number[n].myBox ) == true )
			{
				o_mylevels.set_level(n);
				next_state(STATE_GAME);
			}
		}
	}
}


void LevelSelect::logic()
{
     if ( lol == 0 )
	 {
		  SDL_Rect box;

          box.x= 60; box.y= 90; box.h=0; box.w=0;

		 for ( int n = 0; n < o_mylevels.get_level_number(); n++ )
                          {
								 o_number[n].init( n, box );
                              
                              box.x += 100;
                              
                              if ( box.x >= 750 ) { box.x= 60; box.y += 80; }
                          }     
		
		 lol = 1;
	 }

	titleA--;
	titleA--;
	titleA--;

	if ( titleA > 0 )
			{
				titleA -= 4;
				SDL_SetAlpha(s_black, SDL_SRCALPHA, titleA);
			}

			if ( titleA < 0  )
			{
				titleA = 0;
				SDL_SetAlpha(s_black, SDL_SRCALPHA, titleA);
			}
}

void LevelSelect::render()
{
     apply_surface( 0 , 0, s_background, screen, NULL );
     
     for ( int n = 0; n < (signed)o_number.size(); n++ )
     {
        o_number[n].show();
               
     }    

	 if ( titleA > 1 ) { apply_surface(0,0,s_black,screen,NULL);}

	 SDL_Flip(screen);
}
