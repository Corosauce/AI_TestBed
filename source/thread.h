#ifndef THREAD_H
#define THREAD_H

#include "SDL.h"
#include "SDL_thread.h"

//#include "ai.h"

int _thread_runner(void* data);
int _thread_ai(void* data);

class ai_nav_thread;
class ai_agent;

//namespace test2 {
//ai_nav_thread* tptr2;
//}

class sdl_thread {
public:

	int mythread(void *data);
	int _run(void* data);
	bool startThread(ai_nav_thread* Nav);

	SDL_Thread *thread;
	sdl_thread(){ this->thread = NULL; }
	~sdl_thread(){};

	bool thread_active;

	ai_nav_thread* NavPtr;

	//int mythread(void *data);


};

class ai_thread {
public:

	//int mythread(void *data);
	int _run(void* data);
	bool startThread(ai_agent* ptr);

	SDL_Thread *thread;
	ai_thread(){ this->thread = NULL; }
	~ai_thread(){};

	bool thread_active;

	//ai_agent* AIPtr;

	//int mythread(void *data);


};

#endif