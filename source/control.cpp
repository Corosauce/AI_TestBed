//#include <afxcoll.h>

#include "control.h"
#include "stdlib.h"
#include "stdio.h"

//#include "vld.h"

//#include <windows.h>

#include "glSDL.h"
#include "SDL_image.h"
#include "functions.h"

#include "ai.h"
#include "sprite.h"
#include "map.h"
#include "scenario.h"
#include "timer.h"
#include "physics.h"
#include "agent.h"
#include "player.h"
#include "projectile.h"
#include <vector>
#include "thread.h"



#include "content_manager.h"


//using namespace std;
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library


/*template <typename T, size_t N>
char ( &_ArraySizeHelper( T (&array)[N] ))[N];
#define countof( array ) (sizeof( _ArraySizeHelper( array ) ))*/

//map* Map = new map;
//ai_agent* AI = new ai_agent;


int ai_thread::_run(void* data) {

	ai_agent* tptr = (ai_agent*)data;
	//(ai_nav_thread*)data->Ent;
	//tptr->AI;
	
	//this->thread_active = true;
	while(tptr->thread_active) {
		
		//Sleep(500);
		tptr->Process();
		//tptr->thread_active = false;


	}

	return 0;
}

bool ai_thread::startThread(ai_agent* ptr) {

	//this->thread_active = false;

	//this->AIPtr = ptr;
	
	ptr->thread_active = true;
	this->thread = SDL_CreateThread( _thread_ai, ptr );
	return true;
}

int _thread_ai(void* data)  {
    return reinterpret_cast<ai_thread*>(data)->_run(data);
}


Game_Engine::Game_Engine() {
	//unitNum = Num;

	//laptop
	worldWidth = 1152;
	worldHeight = 720;

	//pc
	worldWidth = 1820;
	worldHeight = 980;

	//power of 2
	//worldWidth = 1024;
	//worldHeight = 1024;


	worldTileSize = 32;
	
}

Game_Engine::~Game_Engine() {
	AI->thread_active = false;
}
/*aitest::aitest(int Num) {
	unitNum = Num;
	
}*/

bool Game_Engine::Init(Game_Engine* worldpointer, Screen* screenpointer) {

	//Get our pointers to itself and screen
	ptrWorld = worldpointer;
	ptrScreen = screenpointer;

	//Init all timers
	this->fps = new Timer();
	this->fpsCount = new Timer();
	this->sim = new Timer();
	this->game_time = new Timer();
	this->anim_time = new Timer();
	this->scen_time = new Timer();
	this->phys_time = new Timer();
	this->ai_time = new Timer();

	teamCount.push_back(0);
	teamCount.push_back(0);
	teamCount.push_back(0);

	this->loader.Init(ptrWorld);


	this->loader.Update("map",0);
	//Declare & Init Map
	this->Map = new map;
	Map->Init(ptrWorld,ptrScreen,worldWidth,worldHeight,worldTileSize);

	this->loader.Update("map",100);

	this->loader.Update("units",0);
	//Load Content From Files
	this->gameData = new Content_Manager;
	this->gameData->ptrWorld = ptrWorld;
	if (!gameData->loadContent()) { return false; }
	this->loader.Update("map",100);
	
	this->Scenario = new scenario;
	Scenario->Init(ptrWorld);
	
	//InitUnits();

	//Declare & Init Physics with engine reference
	ptrPhysics = new Physics;
	ptrPhysics->Init(ptrWorld);

	

	//Misc
	Counter = 0;
	renderTime = 0;
	aiTime = 0;
	unitsVisible = 0;


	curFPS = 0.0f;
	frameDelay = ( 1000 / ptrScreen->FRAMES_PER_SECOND );
	ahh = 0;
	testCount = 0;

	//Start the timers
	this->fps->start();
	this->fpsCount->start();
	this->sim->start();
	this->game_time->start();
	this->anim_time->start();

	//this->MakeUnit(1);

	//no?
	this->AliveUnits = this->Units;

	ClearJobLog();

	this->phys_process = true;
	this->ai_process = true;

	this->thrAI = new ai_thread;
	this->thrAI->thread_active = false;

	los_checks = 0;

	this->lock_newunit = false;
	this->ask_newai = false;
	this->ask_ai_count = 0;
	this->lock_failedunits = 0;

	this->ai_proc_count = 0;

	this->graphSize = 60;
	this->graphMaxValue = 60;
	this->graphBarWidth = 2;
	this->graphPosX = 0;
	this->graphPosY = 400;

	this->fpsData = new long[graphSize];

	for (int i = 0; i < graphSize; i++) {
		fpsData[i] = 0;
	}


	//Finally Init units now that there is a world to initialize them into
	this->Scenario->Load();

	//this better activate!
	if (!AI->thread_active) {
		this->thrAI->startThread(AI);
	}

	return true;
	
}


bool Game_Engine::Process() {
	
	//AI->Think();

	this->procTimeTemp = this->game_time->get_ticks();
	
	if (this->anim_time->get_ticks() > 1000 / this->ptrScreen->FRAMES_PER_SECOND) {
		//this->
		this->anim_time->start();
	}
	
	Scenario->Process();

	Player->DoMoves(); //player

	//Agents - added for shooting independant of ai
	for (int z = 0; z < Units.size(); z++) {
		if (this->Units.at(z)->phys_state != states::PHYS_STATE_DEAD) {
			Units.at(z)->Process();
		}
	}

	phys_time->start();
	ptrPhysics->unitPhysics(); //runs physics on unit array + player
	
	ptrPhysics->prjPhysics();
	physTime = phys_time->get_ticks();
	
	//this is here because of debug info, needs screen blanked first, so dont put this in render function
	SDL_FillRect( ptrScreen->screen, &ptrScreen->screen->clip_rect, SDL_MapRGB( ptrScreen->screen->format, 0x00, 0x48, 0x00 ) );

	//long temp = game_time->get_ticks();

	Counter++;
	curFPS = Counter / (fpsCount->get_ticks() / 1000.0f);
	curRealFPS = 1000.0f / this->procTime;
	//hm = fps->get_ticks();
	//float temp = game_time->get_ticks();

	renderTime = this->game_time->get_ticks();
	if (!Render()) { return false; }
	//float uh = ptrScreen->FRAMES_PER_SECOND;
	
	//UHHHHHHHHH, IS THIS REALLY ACCURATE? if rendering takes no time but something else like phys is taking forever, this will add useless delays, FIX TO ACCOUNT FOR FULL GAME LOOP TIME TO EXECUTE!!!!!

	renderTime = this->game_time->get_ticks() - renderTime;
	if( fps->get_ticks() < ( 1000 / ptrScreen->FRAMES_PER_SECOND ) )
    {
        //Sleep the remaining frame time
        SDL_Delay( ( 1000 / ptrScreen->FRAMES_PER_SECOND ) - fps->get_ticks() + ahh);
    }
	SDL_Delay(1);

	//float temp = game_time->get_ticks();

	//DEBUG

#ifdef SHOW_DEBUG_TEXT

	sstr.str("");
	sstr<<"ai create request: "<<this->ask_ai_count;
	str1 = sstr.str();
	ptrScreen->printText(1250,10,str1.c_str());

	sstr.str("");
	sstr<<"AI ON: "<<this->ai_process;
	str1 = sstr.str();
	ptrScreen->printText(1450,10,str1.c_str());

	sstr.str("");
	sstr<<"active ais: "<<this->ai_count;
	str1 = sstr.str();
	ptrScreen->printText(1550,10,str1.c_str());








	sstr.str("");
	sstr<<"M-B: "<<Player->mouse.button;
	str1 = sstr.str();
	ptrScreen->printText(550,10,str1.c_str());

	sstr.str("");
	sstr<<"M-x: "<<Player->mouse.x;
	str1 = sstr.str();
	ptrScreen->printText(650,10,str1.c_str());

	sstr.str("");
	sstr<<"M-y: "<<Player->mouse.y;
	str1 = sstr.str();
	ptrScreen->printText(750,10,str1.c_str());


	sstr.str("");
	sstr<<"FPS: "<<curRealFPS;
	str1 = sstr.str();
	ptrScreen->printText(850,10,str1.c_str());

	/*sstr.str("");
	sstr<<"Full PF's: "<<this->AI->FullPathCount<< " | WP-PF-Ent's: "<<AI->NoLOSEntCount<<" | WP-PF-Targ: "<<AI->NoLOSEntCount;
	str1 = sstr.str();
	ptrScreen->printText(850,50,str1.c_str());*/

	sstr.str("");
	sstr<<"AI ask: " <<this->ptrWorld->ask_newai<< "| AI Ents: "<<AIEntCount<< " | Control Ents: "<<this->Units.size();
	str1 = sstr.str();
	ptrScreen->printText(850,50,str1.c_str());

	sstr.str("");
	sstr<< "aiTime: " << aiTime;
	//sstr<< "aiTime/physics/player: " << this->anim_time->get_ticks();
	//sstr<<(int)(Counter / ((temp) / 1000.0f));
	str1 = sstr.str();
	ptrScreen->printText(850,90,str1.c_str());

	

	sstr.str("");
	sstr<< "physTime: " << physTime<<" | subaiTime: "<<ptrWorld->AI->ai_ms_count;
	//sstr<< "aiTime/physics/player: " << this->anim_time->get_ticks();
	//sstr<<(int)(Counter / ((temp) / 1000.0f));
	str1 = sstr.str();
	ptrScreen->printText(1050,130,str1.c_str());

	
	sstr.str("");
	sstr<< "aiTimeTop: " << aiTimeTop;
	//sstr<< "aiTime/physics/player: " << this->anim_time->get_ticks();
	//sstr<<(int)(Counter / ((temp) / 1000.0f));
	str1 = sstr.str();
	ptrScreen->printText(1050,90,str1.c_str());

	sstr.str("");
	sstr<<"Job Changes: "<<AI->JobChanges;
	str1 = sstr.str();
	ptrScreen->printText(850,130,str1.c_str());

	sstr.str("");
	sstr<<"TeamCounts: "<<this->teamCount.at(1)<<" | "<<this->teamCount.at(2);
	str1 = sstr.str();
	ptrScreen->printText(850,170,str1.c_str());

	//if (Projectiles.size() > 0) {
		sstr.str("");
		sstr<<"Map pf Count: "<<AI->pfCount_Map;
		str1 = sstr.str();
		ptrScreen->printText(850,210,str1.c_str());
	//}

	sstr.str("");
	sstr<<"WP pf Count: "<<AI->pfCount_WP;
	str1 = sstr.str();
	ptrScreen->printText(850,250,str1.c_str());

	sstr.str("");
	sstr<<"navthread Count: "<<AI->threadCount;
	str1 = sstr.str();
	ptrScreen->printText(850,290,str1.c_str());
	
	sstr.str("");
	for (int z = 0; z < jobLog.size(); z++) {

		//if (this->Units.at(z)->phys_state == 0) {

			//sstr<<this->Units.at(z)->target.ID<<" | ";
			sstr<<jobLog.at(z)<<" | ";
			//sstr<<this->Units.at(z)->phys_state;
		//}
	}
	str1 = sstr.str();
	ptrScreen->printText(10,250,str1.c_str());

	if (jobLog.size() > 30) {
		ClearJobLog();
	}

#endif
	//DEBUG //
	
	this->ptrPhysics->physTimeScalePrev = this->ptrPhysics->physTimeScale;
	this->ptrPhysics->physTimeScale = (fps->get_ticks() / frameDelay) * this->ptrPhysics->physSpeedScale;

	if (this->ptrPhysics->physTimeScale-this->ptrPhysics->physTimeScalePrev > 5) {
		this->ptrPhysics->physTimeScale = this->ptrPhysics->physTimeScalePrev;
	}

	fps->start();
	if (fpsCount->get_ticks() > 10000) {
		fpsCount->start();
		Counter = 0;
	}

	runTime = game_time->get_ticks();

	procTime = game_time->get_ticks() - procTimeTemp;

	return true;

}


bool Game_Engine::Render() {
	
	
	Map->RenderWorld();
	
	this->RenderLife();

	Map->renderMiniMap();

	this->renderGraph();
	
	//return false;
	return true;
}

void Game_Engine::RenderLife() {

	unitsVisible = 0;
	int dff = Units.size();
	int i;
	for (i = 0;i < Units.size();i++) {
		if (Units[i]->phys_state != states::PHYS_STATE_DEAD) {
			if (Units.at(i)->animation_list.ID < 0) {
				Unit = Units.at(i);
				int dfsdfsdf = 0;
			}
			Units.at(i)->animation_list.ptrSprite = Units.at(i);
			//Units[i]->animation_list.ptrSprite = Units[i];  //turn this on if you use vector.erase()
			Units[i]->Render();
			
		}
		//if (i > 0) { break; }
	}
	
	if (Player->phys_state != states::PHYS_STATE_DEAD) {
		//Player->animation_list.ptrSprite = Player;
		Player->Render();
	}

	for (i = 0; i < Projectiles.size(); i++) {
		Projectiles[i]->Render();
	}
}

void Game_Engine::renderGraph() {

	SDL_Rect clip;
	SDL_Rect clip2;
	clip2.w = this->graphSize*this->graphBarWidth;
	clip2.h = this->graphMaxValue;
	clip2.x = this->graphPosX+4;
	clip2.y = this->graphPosY;
	clip.w = this->graphBarWidth;

	//shift data over 1
	for (int i = this->graphSize-2; i >= 0; i--) {
		fpsData[i+1] = fpsData[i];
	}

	//add new fps count
	fpsData[0] = this->curRealFPS;

	SDL_FillRect( this->ptrScreen->screen, &clip2, SDL_MapRGB( ptrScreen->screen->format, 0x00, 0x00, 0x00 ) );

	for (int i = this->graphSize-2; i >= 0; i--) {
		clip.h = fpsData[i];
		clip.x = this->graphPosX + (this->graphSize*this->graphBarWidth) - (i*this->graphBarWidth);
		clip.y = this->graphPosY + this->graphMaxValue - fpsData[i];
		SDL_FillRect( this->ptrScreen->screen, &clip, SDL_MapRGB( ptrScreen->screen->format, 0x00, 0x00, 255-(i*4) ) );
	}
	

	
}


void Game_Engine::SetupMap() {
	//Map = MapPtr;

	//scaleInfo.x = -7.5*2;
	//scaleInfo.y = 5.5*2;
	//scaleInfo.z = -16*2;


	//Ok heres the logic of the scaling,
	//OpenGL by default has a matrix size of x: -1.0 to 1.0, y: 1.0 to -1.0 - totalling 2 by 2 size

	//
	

	
}


 
bool Game_Engine::MakeRandUnit() {

	int temp = rand() % (int)(this->gameData->UnitTypes.size());
	//MakeUnit(temp);
	return MakeUnit(temp);
}

bool Game_Engine::MakeUnit(long unitType) {
	
	//if (!this->lock_newunit) {

		this->lock_newunit = true;

		this->Unit = new ent_agent;

		Unit->ID = Units.size();
		Unit->type = this->gameData->LoadUnitID(unitType);

		Unit->ResID = Unit->type->ResID;

		//really? size is 1 tile in size? might... need adjusting
		Unit->width = Unit->ResID->resInfo.xTileSize;
		Unit->height = Unit->ResID->resInfo.yTileSize;

		Unit->Init(Unit,ptrWorld);
		//states::
		Unit->initInfo.state_pri1 = states::AI_PRI1_STATE_KAMIKAZE;

		//unitNum = Unit->ID;

		this->Units.push_back(Unit);

		this->ask_ai_count++;
		this->ask_newai = true;

		//Gotta update the pointer to the updated vector for same thread vectors
		this->ptrPhysics->Entities = Units;

		return true;

		
	//} else {
		//this->lock_failedunits++;
		//return false;
	//}
}

bool Game_Engine::MakeUnit(std::string name) {
	
	return MakeUnit(this->gameData->UnitNameToID(name));

	/*this->Unit = new ent_agent;

	Unit->ID = Units.size();
	Unit->type = this->gameData->LoadUnitName(name);

	Unit->ResID = Unit->type->ResID;
	Unit->width = Unit->ResID->resInfo.xTileSize;
	Unit->height = Unit->ResID->resInfo.yTileSize;

	Unit->Init(Unit,ptrWorld);

	//unitNum = Unit->ID;

	this->Units.push_back(Unit);
	AI->AddAgent();

	Units[Units.size()-1]->ptrBrain = AI->brainMemory[Units.size()-1];

	//Gotta update the pointer to the updated vector
	this->ptrPhysics->Entities = Units;*/
}

bool Game_Engine::MakeUnit(std::string name, long x, long y) {
	
	if (MakeUnit(name)) {

		Unit->position.x = x;
		Unit->position.y = y;

		return true;
	} else { return false; }

	//this->Unit = new ent_agent;

	/*Unit->ID = Units.size();
	Unit->type = this->gameData->LoadUnitName(name);

	Unit->ResID = Unit->type->ResID;
	Unit->width = Unit->ResID->resInfo.xTileSize;
	Unit->height = Unit->ResID->resInfo.yTileSize;

	Unit->Init(Unit,ptrWorld);*/

		

	//unitNum = Unit->ID;

	//this->Units.push_back(Unit);
	//AI->AddAgent();

	//WARNING, ONCE YOU START REMOVING UNITS IN VECTOR, THIS WILL BREAK, USE A LOOKUP FUNCTION FOR ITS ID
	//Units[Units.size()-1]->ptrBrain = AI->brainMemory[Units.size()-1];

	//Gotta update the pointer to the updated vector
	//this->ptrPhysics->Entities = Units;
}

bool Game_Engine::MakeUnit(std::string name, coord pos) {
	return MakeUnit(name,pos.x,pos.y);
}

bool Game_Engine::MakeUnit(std::string name, coord pos, int team) {
	return MakeUnit(name,pos.x,pos.y,team);
}

bool Game_Engine::MakeUnit(std::string name, long x, long y, int team) {
	if (MakeUnit(name,x,y)) {
		Unit->team = team;
		teamCount.at(team)++;
		return true;
	} else { return false; }
}
void Game_Engine::MakeNewUnitsAI() {
	
	//Tells AI thread to make a new ai once it checks this
	//this->ask_newai = true;
	
	//Units[Units.size()-1]->ptrBrain = AI->brainMemory[Units.size()-1];
}
/*void Game_Engine::MakeUnit(long unitType, ent_agent* ptrUnit) {
	
	this->Unit = new ent_agent;

	Unit->ID = Units.size();
	Unit->type = this->gameData->LoadUnitID(unitType);

	Unit->ResID = Unit->type->ResID;
	Unit->width = Unit->ResID->resInfo.xTileSize;
	Unit->height = Unit->ResID->resInfo.yTileSize;

	Unit->Init(Unit,ptrWorld);

	//unitNum = Unit->ID;

	this->Units.push_back(Unit);
	AI->AddAgent();

	Units[Units.size()-1]->ptrBrain = AI->brainMemory[Units.size()-1];

	//Gotta update the pointer to the updated vector
	this->ptrPhysics->Entities = Units;

	
}*/

void Game_Engine::KillUnit(long unitID) {

	if (unitID == -1) {
		Player->phys_state = states::PHYS_STATE_DEAD;
	} else {
		if (Units.at(unitID)->phys_state != states::PHYS_STATE_DEAD) {
			Units.at(unitID)->phys_state = states::PHYS_STATE_DEAD;


			//replacing AI->KillAgent(unitID); with thread friendly boolean for ai next 
			Units.at(unitID)->declareDead = true;

			teamCount.at(Units.at(unitID)->team)--;
			//DeleteUnit(unitID);

			//Add code here for releasing the AI memory for reuse.

			//SESH SAVE
		}

	}

}

void Game_Engine::KillAllUnits() {
	for (int z = 0; z < Units.size(); z++) {
		if (this->Units.at(z)->phys_state != states::PHYS_STATE_DEAD) {
			KillUnit(this->Units.at(z)->ID);
		}
	}
}

void Game_Engine::DeleteUnit(long unitID) {


	//not used, units never deleted to keep vectors order, fix and use if memory becomes issue over time

	ent_agent* temp;
	
	for (int i = 0; i < Units.size();i++) {
		s = Units.begin();
		s+= i;
		if (Units.at(i)->ID == unitID) {
			temp = Units.at(i);
			this->AI->RemoveAgent(unitID);
			Units.erase(s);
			delete temp;
			this->AI->Entities = Units;
			break;
		}
		
		
	}

	for (int i = 0;i < Units.size();i++) {
		//if (Units[i]->phys_state != states::PHYS_STATE_DEAD) {
			Units.at(i)->animation_list.ptrSprite = Units.at(i);
			//Units[i]->animation_list.ptrSprite = Units[i];  //turn this on if you use vector.erase()
			//Units[i]->Render();
		//}
	}

}

void Game_Engine::HurtUnit(long unitID, long dmg) {

	if (unitID == -1) {
		Player->health -= dmg;
		if (Player->health < 1) {
			KillUnit(unitID);
		}
	} else {
		Units.at(unitID)->health -= dmg;
		if (Units.at(unitID)->health < 1) {
			KillUnit(unitID);
		}
	}

}

void Game_Engine::MakePrj(sprite* SprPtr, float vector)
{
	Prj = new projectile();

	Prj->type = this->gameData->LoadPrjName(SprPtr->weapons[0].name);

	//Prj->ResID = Resources.NewResource(11);
	//Prj->ResID = Prj->type->ResID;
	Prj->ResID = Prj->type->ResID;
	
	Prj->width = Prj->ResID->resInfo.xTileSize;
	Prj->height = Prj->ResID->resInfo.yTileSize;

	Prj->prjID = Prj;
	Prj->Init(SprPtr,this->ptrWorld);

	Prj->position = SprPtr->position;
	//Prj->vector = this->AI->getHeading();
	Prj->vector = vector;
	Prj->acceleration = Prj->goforce;

	Prj->spawntime = this->runTime;
	Prj->max_lifetime = 9000;


	Prj->animation_list.Start(0);
	Prj->animation_list.active = true;

	this->Projectiles.push_back(Prj);
}




//Some input functions, eventually to be moved into a player manager of some kind?
void Game_Engine::KeyPress(int key, bool isPressed) {

	this->Player->keys[key] = isPressed;

}

void Game_Engine::MouseMove(long x, long y) {
	this->Player->mouse.x = x;
	this->Player->mouse.y = y;
}

void Game_Engine::MousePress(long button) {
	this->Player->mouse.button = button;
}

/*int aitest::CheckInput(SDLKey key, bool isDown) {

	
	//int a = 1;
	//Units[0].position.y -= 1;
	this->keys[(int)key] = (bool)isDown;

	return 1;
}*/


void Game_Engine::AILogJob(float val) {
	
	jobLog.push_back(val);

}

void Game_Engine::ClearJobLog() {
	jobLog.clear();
}

/*void drawcircle(int x, int y, int r, int c)
{
  int i, j;
  for (i = 0; i < 2 * r; i++)
  {
    int len = (int)(sqrt(cos(0.5f * PI * (i - r) / r)) * r * 2);
    int ofs = (y - r + i) * PITCH + x - len / 2;
    for (j = 0; j < len; j++)
      ((unsigned int*)screen->pixels)[ofs + j] = c;
  }
}*/
