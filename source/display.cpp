#include "display.h"
#include "screen.h"
#include "control.h"

#include "glSDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
//#include "main.h"

#include "SDL_opengl.h"




void Window::Init(Screen* pointerscreen, Game_Engine* worldpointer)
{
	ptrScreen = pointerscreen;
	ptrWorld = worldpointer;

	

	if (!ptrScreen->Init()) { windowOK = false; }

	//opengl test
	//SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // *new*

	if (ptrWorld->useOpenGL) {
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // *new*

		ptrScreen->screen = SDL_SetVideoMode( ptrWorld->worldWidth, ptrWorld->worldHeight, 0, SDL_OPENGL);
		ptrScreen->screen = SDL_LoadBMP("image.bmp");

		if ( (ptrScreen->screen->w & (ptrScreen->screen->w - 1)) != 0 ) {
            printf("warning: image.bmp's width is not a power of 2\n");
        }
    
        // Also check if the height is a power of 2
        if ( (ptrScreen->screen->h & (ptrScreen->screen->h - 1)) != 0 ) {
            printf("warning: image.bmp's height is not a power of 2\n");
        }

	} else {
		ptrScreen->screen = SDL_SetVideoMode( ptrWorld->worldWidth, ptrWorld->worldHeight, 0, SDL_GLSDL | SDL_HWSURFACE);
	}
	//ptrScreen->screen = SDL_SetVideoMode( ptrWorld->worldWidth, ptrWorld->worldHeight, 0, SDL_OPENGL);


	


    //If there's an error
    if( ptrScreen->screen == NULL )
    {
        windowOK = false;
        return;
    }
    else
    {
        windowOK = true;
    }


	if (ptrWorld->useOpenGL) {
		glClearColor( 0, 0, 0, 0 );
		
		glEnable( GL_TEXTURE_2D ); // Need this to display a texture

		glViewport( 0, 0, ptrWorld->worldWidth, ptrWorld->worldHeight );

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		glOrtho( 0, ptrWorld->worldWidth, ptrWorld->worldHeight, 0, 1, -1 );
	    
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
	    
		// Load the OpenGL texture

		//GLuint texture; // Texture object handle

		// Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &ptrScreen->GLtexture );
    
        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, ptrScreen->GLtexture );

		// Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}

	
	ptrScreen->hwSurfaceUsed = true;
	if (0 == ptrScreen->screen->flags & SDL_HWSURFACE) {
		ptrScreen->hwSurfaceUsed = false;
	}
	
    //Set the window caption
    SDL_WM_SetCaption( "Window Event Test", NULL );

    //Set window flag
    windowed = true;
}

void Window::handle_events()
{
    //If there's something wrong with the window
    if( windowOK == false )
    {
        return;
    }

    //If the window resized
	if (event.type == SDL_MOUSEMOTION) {
		ptrWorld->MouseMove(event.motion.x,event.motion.y);

	}
	else if (event.type == SDL_MOUSEBUTTONDOWN) {
		ptrWorld->MousePress(event.button.button);
	}
	else if (event.type == SDL_MOUSEBUTTONUP) {
		ptrWorld->MousePress(event.button.button);
	}
    else if( event.type == SDL_VIDEORESIZE )
    {
        //Resize the screen
        ptrScreen->screen = SDL_SetVideoMode( event.resize.w, event.resize.h, ptrScreen->SCREEN_BPP, SDL_HWSURFACE | SDL_RESIZABLE );
		//ptrScreen->screen = SDL_SetVideoMode( ptrWorld->worldWidth, ptrWorld->worldHeight, 0, SDL_OPENGL);

        //If there's an error
        if( ptrScreen->screen == NULL )
        {
            windowOK = false;
            return;
        }
    }
    //If enter was pressed
    else if(event.type == SDL_KEYDOWN)
    {
		ptrWorld->KeyPress((int)event.key.keysym.sym,true);
    }
	else if(event.type == SDL_KEYUP)
    {
		ptrWorld->KeyPress((int)event.key.keysym.sym,false);
    }
    //If the window focus changed
    else if( event.type == SDL_ACTIVEEVENT )
    {
        //If the window was iconified or restored
        if( event.active.state & SDL_APPACTIVE )
        {
            //If the application is no longer active
            if( event.active.gain == 0 )
            {
                SDL_WM_SetCaption( "Window Event Test: Iconified", NULL );
            }
            else
            {
                SDL_WM_SetCaption( "Window Event Test", NULL );
            }
        }
        //If something happened to the keyboard focus
        else if( event.active.state & SDL_APPINPUTFOCUS )
        {
            //If the application lost keyboard focus
            if( event.active.gain == 0 )
            {
                SDL_WM_SetCaption( "Window Event Test: Keyboard focus lost", NULL );
            }
            else
            {
                SDL_WM_SetCaption( "Window Event Test", NULL );
            }
        }
        //If something happened to the mouse focus
        else if( event.active.state & SDL_APPMOUSEFOCUS )
        {
            //If the application lost mouse focus
            if( event.active.gain == 0 )
            {
                SDL_WM_SetCaption( "Window Event Test: Mouse Focus Lost", NULL );
            }
            else
            {
                SDL_WM_SetCaption( "Window Event Test", NULL );
            }
        }
    }
    //If the window's screen has been altered
    else if( event.type == SDL_VIDEOEXPOSE )
    {
        //Update the screen
        if( SDL_Flip( ptrScreen->screen ) == -1 )
        {
            //If there's an error
            windowOK = false;
            return;
        }
    }
}

void Window::toggle_fullscreen()
{
    //If the screen is windowed
    if( windowed == true )
    {
        //Set the screen to fullscreen
        ptrScreen->screen = SDL_SetVideoMode( ptrWorld->worldWidth, ptrWorld->worldHeight, ptrScreen->SCREEN_BPP, SDL_HWSURFACE | SDL_RESIZABLE | SDL_FULLSCREEN );

        //If there's an error
        if( ptrScreen->screen == NULL )
        {
            windowOK = false;
            return;
        }

        //Set the window state flag
        windowed = false;
    }
    //If the screen is fullscreen
    else if( windowed == false )
    {
        //Window the screen
        ptrScreen->screen = SDL_SetVideoMode( ptrWorld->worldWidth, ptrWorld->worldHeight, ptrScreen->SCREEN_BPP, SDL_HWSURFACE | SDL_RESIZABLE );

        //If there's an error
        if( ptrScreen->screen == NULL )
        {
            windowOK = false;
            return;
        }

        //Set the window state flag
        windowed = true;
    }
}

bool Window::error()
{
    return !windowOK;
}




