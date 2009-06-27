#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP	= 32;

//Button state definitions
const int CLIP_MOUSEOVER 	= 0;
const int CLIP_MOUSEOUT		= 1;
const int CLIP_MOUSEDOWN	= 2;
const int CLIP_MOUSEUP		= 3;



//The surfaces
SDL_Surface *image 		= NULL;
SDL_Surface *screen 		= NULL;
SDL_Surface *buttonSheet 	= NULL;
SDL_Rect clips[4];
SDL_Event event;

class Button
{
	private:
		//Button attributes
		SDL_Rect box;

		//The part of the button sprite sheet to be shown
		SDL_Rect* clip;
	public:
		//Init variables
		Button (int x, int y, int w, int h);

		//Handles events and sets the button's sprite region
		void handle_events();

		//Show button on screen
		void show();
};

Button::Button (int x, int y, int w, int h)
{
	//Sets the button attributes
	box.x	= x;
	box.y 	= y;
	box.w 	= w;
	box.h 	= h;
	
	//Set default clip
	clip 	= &clips[CLIP_MOUSEOUT];
}

void Button::handle_events()
{
	//mouse offsets
	int x = 0, y = 0;

	//If the mouse moved
	if (event.type == SDL_MOUSEMOTION)
	{
		x = event.motion.x;
		y = event.motion.y;

		//if mouse cursor is over button
		if ( (x > box.x) && (x < box.x + box.w) && (y>box.y) && (y < box.y + box.h))
		{
			clip = &clips[CLIP_MOUSEOVER];
		}
		else
		{
			clip = &clips[CLIP_MOUSEOUT];
		}
	}
	//If a mouse button was pressed
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		//if left mouse button
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			x = event.button.x;
			y = event.button.y;
			//If the mouse is over the button
			if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
			{
				clip = &clips[CLIP_MOUSEDOWN];
			}
			else clip = &clips[CLIP_MOUSEUP];
		}
	}
	
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			x = event.button.x;
			y = event.button.y;

			if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
			{
				clip = &clips[CLIP_MOUSEUP];
			}
		}
	}
}


void set_clips()
{
	//Clips the sprite sheet into 4 regions
	clips[ CLIP_MOUSEOVER ].x = 0;
	clips[ CLIP_MOUSEOVER ].y = 0;
	clips[ CLIP_MOUSEOVER ].w = 320;
	clips[ CLIP_MOUSEOVER ].h = 240;

	clips[ CLIP_MOUSEOUT ].x = 320;
	clips[ CLIP_MOUSEOUT ].y = 0;
	clips[ CLIP_MOUSEOUT ].w = 320;
	clips[ CLIP_MOUSEOUT ].h = 240;

	clips[ CLIP_MOUSEDOWN ].x = 0;
	clips[ CLIP_MOUSEDOWN ].y = 240;
	clips[ CLIP_MOUSEDOWN ].w = 320;
	clips[ CLIP_MOUSEDOWN ].h = 240;

	clips[ CLIP_MOUSEUP ].x = 320;
	clips[ CLIP_MOUSEUP ].y = 240;
	clips[ CLIP_MOUSEUP ].w = 320;
	clips[ CLIP_MOUSEUP ].h = 240;
}


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

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip=NULL ) 
{ 
	SDL_Rect offset; 
	offset.x = x; 
	offset.y = y; 
	SDL_BlitSurface( source, clip, destination, &offset ); 
}


void Button::show()
{
	//Show the button
	apply_surface (box.x,box.y,buttonSheet,screen,clip);
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

	SDL_WM_SetCaption("Event test",NULL);

	return true;
}

bool load_files()
{
	buttonSheet = load_image("button.png");
	image = load_image("test.jpg");
	if (image == NULL || buttonSheet == NULL) return false;
	return true;
}

void clean_up()
{
	SDL_FreeSurface(image);
	SDL_FreeSurface(buttonSheet);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	bool quit = false;
	if (init() == false) return 1;

	if (load_files() == false) return 1;

	apply_surface(0,0,image,screen);
	set_clips();
	Button myButton(170,120,320,240);
	myButton.show();

	if (SDL_Flip(screen) == -1) return 1;

	while (quit == false)
	{
		while (SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
			myButton.handle_events();
			myButton.show();
			if (SDL_Flip(screen) == -1) return 1;
		}
	}
	clean_up();
	return 0;
}

