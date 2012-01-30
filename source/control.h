#ifndef CONTROL_H
#define CONTROL_H

#pragma warning( disable : 4244 4018 )

// DEBUG VARS //

#define SHOW_DEBUG_TEXT
#define SHOW_DEBUG_INFO

const bool showUnitInfo = true;

const bool procDebug = false;

// DEBUG VARS //

#include "resource_manager.h"
#include "screen.h"

#include "glSDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <vector>
#include "phys_math.h"
#include "loadScreen.h"
//using namespace std;
const int THOUGHTS_PER_SECOND = 30;

//my extra functions

class sprite;
class ai_agent;
class map;
class Timer;
class Physics;
class ent_agent;
class player;
class projectile;
class scenario;
class ai_thread;

class Content_Manager;

class loadScreen;


typedef void GLvoid;

class Game_Engine {
public:

	Game_Engine();
	//aitest(int Num);
	~Game_Engine();

	//Initializers
	bool Init(Game_Engine* worldpointer, Screen* screenpointer);
	void SetupMap();
	//void InitUnits();

	bool MakeRandUnit();

	void AILogJob(float val);
	void ClearJobLog();
	std::vector<float> jobLog;


	bool MakeUnit(long unitType);
	bool MakeUnit(std::string name);
	bool MakeUnit(std::string name, long x, long y);
	bool MakeUnit(std::string name, long x, long y, int team);
	bool MakeUnit(std::string name, coord pos);
	bool MakeUnit(std::string name, coord pos, int team);

	//only works for not yet initialized ai
	void PrepAIRole(ent_agent* Ent, long role);
	void PrepAIJob(ent_agent* Ent, long job);

	//no longer needed
	void MakeNewUnitsAI();
	//void MakeUnit(long unitType, ent_agent* ptrUnit);

	void MakePrj(sprite* SprPtr, float vector);

	void HurtUnit(long unitID, long dmg);
	void KillUnit(long unitID);
	void DeleteUnit(long unitID);

	void KillAllUnits();

	void KeyPress(int key, bool isPressed);
	void MouseMove(long x, long y);
	void MousePress(long button);

	//Game Control Procedures
	bool Process();
	
	//int CheckInput(SDLKey key,bool isDown);
	bool Render();

	void RenderLife();

	long unitNum;
	long unitMax;

	long testCount;


	int worldWidth;
	int worldHeight;
	int worldTileSize;

	Game_Engine*		ptrWorld;
	Screen*				ptrScreen;
	Physics*			ptrPhysics;
	
	Content_Manager*	gameData;

	loadScreen			loader;

	

	//Text Display Stuff
	std::stringstream sstr;
	std::string str1;
	
	player*						Player;
	//ent_agent*				Units;
	std::vector<ent_agent *>	Units;
	std::vector<ent_agent *>	AliveUnits;
	std::vector<ent_agent *>::iterator s;
	std::vector<projectile *>	Projectiles;
	ent_agent					*Unit;
	projectile					*Prj;
	map*						Map;
	scenario*					Scenario;
	ai_agent*					AI; //its really more of an engine itself than an agent, it controls agents

	ai_thread*					thrAI;
	

	//ResourceObject* ResObj;

	SDL_Rect clip[ 4 ];

	Timer*				fps;
	Timer*				fpsCount;
	Timer*				sim;
	Timer*				game_time;
	Timer*				anim_time;
	Timer*				scen_time;
	Timer*				ai_time;

	Timer*				phys_time;

	long				physTime;

	long				runTime;
	long				renderTime;

	long				procTime;
	long				procTimeTemp;

	long				aiTime;
	long				aiTimeTop;
	long				unitsVisible;

	long				Counter;
	
	float				curFPS;
	float				curRealFPS;
	float				frameDelay;
	float				ahh;

	long				ai_count;
	long				ai_proc_count;
	bool				scen_process;
	bool				phys_process;
	bool				ai_process;

	std::vector<long>	teamCount;

	long				los_checks;


	//Multithreading locks

	//New Unit lock
	bool				lock_newunit;
	bool				ask_newai;
	long				ask_ai_count;

	long				lock_failedunits;

	//Unit location vector lock
	bool				lock_tile_map;

	bool				useOpenGL;

	//Debug graph declares
	void				renderGraph();

	long*				fpsData;
	long				graphMaxValue;
	long				graphBarWidth;
	long				graphSize;

	long				graphPosX;
	long				graphPosY;


	long				AIEntCount;


};

#endif