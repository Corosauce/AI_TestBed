#ifndef SCENARIO_H
#define SCENARIO_H

//#include <vector>
//#include "stdlib.h"
//#include "stdio.h"
//#include "glSDL.h"

#include "resource_manager.h"
#include "screen.h"

#include "glSDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <vector>

//#include "timer.h"

class Game_Engine;
class player;
class ent_agent;
//class Timer;

class scenario {
public:

	scenario();
	~scenario();

	void Init(Game_Engine* worldpointer);
	void Load();
	void Process();

	void SpawnUnits_Circular(std::string name, long x, long y, int team, long numUnits, long circleSize);

	long			lastExecTime;
	long			lastExecTime2;

	Game_Engine*				ptrWorld;
	player*						Player;
	std::vector<ent_agent *>	Units;
	
	std::stringstream sstr;
	std::string str1;



};

#endif