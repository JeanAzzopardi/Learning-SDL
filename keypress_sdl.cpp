#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>
#include <iostream>
//Screen attributes
const int SCREEN_WIDTH  	= 640;
const int SCREEN_HEIGHT 	= 480;
const int SCREEN_BPP		= 32;

//The surfaces
SDL_Surface *background 	= NULL;
SDL_Surface *screen 		= NULL;
SDL_Surface *upMessage		= NULL;
SDL_Surface *downMessage 	= NULL; 
SDL_Surface *leftMessage	= NULL;
SDL_Surface *rightMessage	= NULL;




SDL_Event event;
TTF_Font *font 		= NULL;
SDL_Color textcolor	= {255,255,255};

SDL_Surface *load_image( std::string filename ) 
{ 
	 
	SDL_Surface* loadedImage = NULL; 
	SDL_Surface* optimizedImage = NULL;
	
	loadedImage = IMG_Load( filename.c_str() ); 
	
	if( loadedImage != NULL ) 
	{ 
		optimizedImage = SDL_DisplayFormat( loadedImage ); 
		SDL_FreeSurface( loadedImage ); 
	} 
	return optimizedImage; 
} 

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) 
{ 
	SDL_Rect offset; 
	offset.x = x; 
	offset.y = y; 
	SDL_BlitSurface( source, NULL, destination, &offset ); 
}

bool init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	//Set up the screen
	screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE);

	if (screen == NULL) return false;

	if (TTF_Init() == -1) return false;

	SDL_WM_SetCaption("Testing TTF",NULL);

	return true;
}

bool load_files(std::string fontname, std::string imagename)
{
	background = load_image(imagename.c_str());
	font = TTF_OpenFont(fontname.c_str(),28);

	if (background == NULL || font == NULL) return false;
	return true;
}

void clean_up()
{
	SDL_FreeSurface(background);
	SDL_FreeSurface(upMessage);
	SDL_FreeSurface(downMessage);
	SDL_FreeSurface(leftMessage);
	SDL_FreeSurface(rightMessage);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}




int main(int argc, char* argv[])
{
	bool quit = false;
	SDL_Surface *message = NULL;
	if (argc != 3)
	{
		std::cout << "Usage : keypress_sdl <fontname> <image name>\n";
		return 1;
	}

	if (init() == false) return 1;
	if (load_files(argv[1],argv[2]) == false) return 1;
	
	//Render the text messages
	
	upMessage = TTF_RenderText_Solid(font,"Up was pressed",textcolor);
	downMessage = TTF_RenderText_Solid(font,"Down was pressed",textcolor);
	leftMessage = TTF_RenderText_Solid(font,"Left was pressed",textcolor);
	rightMessage = TTF_RenderText_Solid(font,"Right was pressed",textcolor);
	
	apply_surface(0,0,background,screen);
	
	
	
	if (SDL_Flip(screen) == -1) return 1;
	
	while (quit == false)
	{
		while (SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_UP	:message = upMessage; 	 break;
					case SDLK_DOWN	:message = downMessage;  break;
					case SDLK_LEFT	:message = leftMessage;  break;
					case SDLK_RIGHT :message = rightMessage; break;
					default : ;
				}
			}
			if (message != NULL)
			{
				apply_surface(0,0,background,screen);
				apply_surface((SCREEN_WIDTH - message->w)/2,(SCREEN_HEIGHT-message->h)/2,message,screen);
				message = NULL;
			}
			if (SDL_Flip(screen) == -1) return 1;

		}
	}
	
	clean_up();
	return 0;
}

