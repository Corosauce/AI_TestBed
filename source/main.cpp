/*This source code copyrighted by Lazy Foo' Productions (2004-2009) and may not
be redestributed without written permission.*/

//The headers

//#include "glSDL.h" //hardware accel!

#include "SDL_opengl.h"

#include "SDL.h"
#include "SDL_image.h"
#include <string>

#include "main.h"
//#include "timer.h"



bool game_start() {
	
	// GAME CODE!!!
	//World.unitNum = unitNum;
	//World.ptrfps = fps;

	if ( !World.Init(&World, &Screen) ) { return false; }
	
	return true;

}

int main( int argc, char* args[] ) {
	
	//Quit flag
    bool quit = false;

	World.useOpenGL = false;
	
	//_putenv("SDL_VIDEODRIVER=");
	_putenv("SDL_VIDEO_WINDOW_POS=50,30");
	

	//const SDL_VideoInfo* myPointer = SDL_GetVideoInfo();

	//int a = myPointer->hw_available(

    //Initialize
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) { return 1; }

    //Create a window
    myWindow.Init(&Screen,&World);

/*
	//opengl test
	glEnable( GL_TEXTURE_2D );
	
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	 
	glViewport( 0, 0, 640, 480 );
	 
	glClear( GL_COLOR_BUFFER_BIT );
	 
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	 
	glOrtho(0.0f, 640, 480, 0.0f, -1.0f, 1.0f);
		
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	SDL_Delay(5000);
	//end opengl test
*/
    //If the window failed
    if( myWindow.error() == true ) { return 1; }

	if ( !game_start() ) { return 1; }
	
	frame = 0;

    //While the user hasn't quit
    while( quit == false )
    {
		
		

        //While there's events to handle
        while( SDL_PollEvent( &(myWindow.event) ) )
        {
            //Handle window events
            myWindow.handle_events();

            //If escape was pressed
            if( ( myWindow.event.type == SDL_KEYDOWN ) && ( myWindow.event.key.keysym.sym == SDLK_ESCAPE ) )
            {
                //Quit the program
                quit = true;
            }

            //If the user has Xed out the window
            if( myWindow.event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        //If the window failed
        if( myWindow.error() == true )
        {
            return 1;
        }

		if (!World.Process()) {
			return false;
		}
		myWindow.ptrScreen->screen->h;
        //Update the screen
		if (World.useOpenGL) {
			//myWindow.ptrScreen->screen = SDL_LoadBMP("image.bmp");

			glBindTexture( GL_TEXTURE_2D, myWindow.ptrScreen->GLtexture );

			glTexImage2D( GL_TEXTURE_2D, 0, 3, myWindow.ptrScreen->screen->w, myWindow.ptrScreen->screen->h, 0, GL_BGR, GL_UNSIGNED_BYTE, myWindow.ptrScreen->screen->pixels );

			// Clear the screen before drawing
			glClear( GL_COLOR_BUFFER_BIT );
		    
			// Bind the texture to which subsequent calls refer to
			glBindTexture( GL_TEXTURE_2D, Screen.GLtexture );

			glBegin( GL_QUADS );
				// Top-left vertex (corner)
				glTexCoord2i( 0, 0 );
				glVertex3f( 0, 0, 0 );
		    
				// Bottom-left vertex (corner)
				glTexCoord2i( 1, 0 );
				glVertex3f( myWindow.ptrScreen->screen->h, 0, 0 );
				
		    
				// Bottom-right vertex (corner)
				glTexCoord2i( 1, 1 );
				glVertex3f( myWindow.ptrScreen->screen->w, myWindow.ptrScreen->screen->h, 0 );
		    
				// Top-right vertex (corner)
				glTexCoord2i( 0, 1 );
				glVertex3f( 0, myWindow.ptrScreen->screen->h, 0 );

				
			glEnd();
			
			SDL_GL_SwapBuffers();
		} else {
			if( SDL_Flip( Screen.screen ) == -1 )
			{
				return 1;
			}
		}

		//Increment the frame counter
        //frame++;

        //If we want to cap the frame rate
		
    }

    //Clean up
	myWindow.ptrScreen->clean_up();

    return 0;

}


