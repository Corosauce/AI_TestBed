#include "screen.h"

#include "glSDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "SDL_opengl.h"

Screen::Screen() {

	/*SCREEN_WIDTH = 1280;
	SCREEN_HEIGHT = 960;
	SCREEN_BPP = 32;
	FRAMES_PER_SECOND = 60;*/

}

Screen::~Screen() {

	/*SCREEN_WIDTH = 1280;
	SCREEN_HEIGHT = 960;
	SCREEN_BPP = 32;
	FRAMES_PER_SECOND = 60;*/

}

bool Screen::Init() {

	background = NULL;
	screen = NULL;
	message = NULL;

	//font = NULL;
	//int a = 1;
	textColor.r = 255; textColor.g = 255; textColor.b = 255;

	TTF_Init();
	font = TTF_OpenFont( "arial.ttf", 22 );
	if (font == NULL) {
		return false;
	}
	
	return true;

}

void Screen::printText(int x, int y, const char* text) {
	
	
	message = TTF_RenderText_Solid( font, text, textColor );

	apply_surface(x, y, message, screen, NULL );

	//delete message;
	//message = NULL;
	SDL_FreeSurface(message); 

	//TTF_RenderText

}

void Screen::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

void Screen::clean_up()
{
    //Free the image
    SDL_FreeSurface( background );
	SDL_FreeSurface( screen );
	//SDL_FreeSurface( message );

    //Quit SDL
    SDL_Quit();
}
