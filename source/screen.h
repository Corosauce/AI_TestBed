#ifndef SCREEN_H
#define SCREEN_H

#include "glSDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <iostream>
#include <sstream>
#include <string>

#include "SDL_opengl.h"

class Screen {
public:

	//The attributes of the screen
	//static const int SCREEN_WIDTH = 1152;//1024;
	//static const int SCREEN_HEIGHT = 720;//768;
	static const int SCREEN_BPP = 32;
	static const int FRAMES_PER_SECOND = 60;

	static const int VIEWPORT_WIDTH = 1024;
	static const int VIEWPORT_HEIGHT = 640;


	Screen();
	~Screen();

	bool Init();
	void printText(int x, int y, const char* text);
	void changeFont(const char* font, int size);
	void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
	void clean_up();

	bool hwSurfaceUsed;

	//The surfaces that will be used
	
	SDL_Surface *background;
	SDL_Surface *screen;

	SDL_Surface *message;
	TTF_Font *font;
	SDL_Color textColor;

	GLuint GLtexture;
};

#endif