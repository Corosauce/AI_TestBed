#ifndef DISPLAY_H
#define DISPLAY_H

#include "screen.h"
#include "control.h"


//Our window
class Window
{
	public:
	SDL_Event event;
    
    //Whether the window is windowed or not
    bool windowed;

    //Whether the window is fine
    bool windowOK;

    
    //Constructor
    void Init(Screen* screenpointer, Game_Engine* worldpointer);

    //Handle window events
    void handle_events();

	Screen*	ptrScreen;
	Game_Engine* ptrWorld;

	SDL_Color textColor;

    //Turn fullscreen on/off
    void toggle_fullscreen();

    //Check if anything's wrong with the window
    bool error();
};

#endif