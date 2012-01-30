#include "scenario.h"

#include "control.h"
#include "player.h"
#include "agent.h"
#include "ai.h"
#include "physics.h"
#include "phys_math.h"

#include "content_manager.h"


scenario::scenario() {

}

scenario::~scenario() {

}

void scenario::Init(Game_Engine *worldpointer) {

	this->ptrWorld = worldpointer;
	
	ptrWorld->unitNum = 0;
	ptrWorld->unitMax = 1;
	//ptrWorld->unitNum;
	//Declares and Buffers unit space
	//Units = new ent_agent[this->unitMax]; //unitNum should become unitMax here

	//Init Player and AI Units
	ptrWorld->Player = new player;
	for (int i = 0; i < ptrWorld->unitNum; i++) {
		ptrWorld->Unit = new ent_agent;
		ptrWorld->Units.push_back(ptrWorld->Unit);
	}

	//long lol = Units.size();

	ptrWorld->unitNum = ptrWorld->Units.size();

	//Declare & Init AI, needs reference to declared Unit array
	ptrWorld->AI = new ai_agent;
	ptrWorld->AI->unitNum = ptrWorld->unitNum;
	ptrWorld->AI->Init(ptrWorld);

	this->Player = ptrWorld->Player;
	this->Units = ptrWorld->Units;

	Player->ID = -1;
	Player->type = ptrWorld->gameData->LoadUnitName("vulture");
	//Player->type->
	//Player->ResID = Resources.NewResource(6);
	Player->ResID = Player->type->ResID;
	Player->width = Player->ResID->resInfo.xTileSize;
	Player->height = Player->ResID->resInfo.yTileSize;
	// |
	//Player->ID = -1;
	Player->Init(Player,ptrWorld);
	Player->InitPlayer();

	int ii = 0;
	
	
	return;



	for (int i = 0;i < ptrWorld->unitNum;i++)
	{
		Units[i]->ID = i;
		//Units[i]->ResID = Resources.NewResource(6);
		Units[i]->type = ptrWorld->gameData->LoadUnitName("vulture");
		Units[i]->ResID = Units[i]->type->ResID;
		Units[i]->width = Units[i]->ResID->resInfo.xTileSize;
		Units[i]->height = Units[i]->ResID->resInfo.yTileSize;
		// |
		//Units[i]->ID = i;
		Units[i]->Init(Units[i],ptrWorld);
		int ii = 0;
	}

}


void scenario::Load() {
	
	ptrWorld->Player->position.x = 1100;
	ptrWorld->Player->position.y = 100;

	ptrWorld->Player->team = 1;

	ptrWorld->Player->health = 990001;

	/*for (int i = 0; i < 15; i++) {

		this->ptrWorld->MakeUnit("pcannon");
		this->ptrWorld->Unit->position.x = 390+(i*90);
		this->ptrWorld->Unit->position.y = 190;

		this->ptrWorld->Unit->team = 2;
	}*/
	

	/*for (int i = 0; i < ptrWorld->unitMax; i++) {

		this->ptrWorld->MakeUnit("vulture",900,1200,2);
		this->ptrWorld->Unit->position.x = 700+(i*90);
		this->ptrWorld->Unit->position.y = 1650+i;
		this->ptrWorld->MakeNewUnitsAI();
	}*/

	/*for (int i = 0; i < 5; i++) {


		ptrWorld->MakeUnit("vulture",1200+(i*9),40+i,1);
		this->ptrWorld->Unit->health = 30000;
		this->ptrWorld->MakeNewUnitsAI();

	}*/

	//ptrWorld->MakeUnit("pcannon",1100,160,1);
	//this->ptrWorld->Unit->health = 999999;
	//this->ptrWorld->MakeNewUnitsAI();
	//ptrWorld->MakeUnit("pcannon",1500,160,1);
	//this->ptrWorld->MakeNewUnitsAI();

	/*ptrWorld->MakeUnit("pcannon",800,1700,2);
	this->ptrWorld->MakeNewUnitsAI();
	ptrWorld->MakeUnit("pcannon",1000,1700,2);
	this->ptrWorld->MakeNewUnitsAI();
	ptrWorld->MakeUnit("pcannon",1200,1700,2);
	this->ptrWorld->MakeNewUnitsAI();*/
	
	//ptrWorld->MakeUnit("pcannon",1400,1700,2);
	//this->ptrWorld->Unit->health = 999999;
	//this->ptrWorld->MakeNewUnitsAI();
	//ptrWorld->MakeUnit("pcannon",1600,1500,2);
	//this->ptrWorld->MakeNewUnitsAI();

	//this->SpawnUnits_Circular("dragoon",1200,300,1,10,100);
	//this->SpawnUnits_Circular("dragoon",1200,1800,2,10,100);

}

void scenario::Process() {
	//return;
	if (this->ptrWorld->Units.size() < 9000 && ptrWorld->scen_process) {

		if (this->ptrWorld->teamCount.at(1) < 60) {
			if (this->ptrWorld->runTime > this->lastExecTime2 + (this->ptrWorld->teamCount.at(1)*1) ) {
				this->lastExecTime2 = this->ptrWorld->runTime;
				if (this->ptrWorld->MakeUnit("vulture",1200+(rand()/15*1),270,1)) {

					//how to init ai from world class, coming soon: templates!
					this->ptrWorld->Unit->initInfo.useInit = true;
					//this->ptrWorld->Unit->initInfo.state_pri1 = states::AI_PRI1_STATE_DAZED;
					this->ptrWorld->Unit->initInfo.state_pri1 = states::AI_PRI1_STATE_PROTECT;
					this->ptrWorld->Unit->initInfo.state_job = states::AI_JOB_STATE_FOLLOW;
					//this->ptrWorld->Unit->initInfo.target_atkID = -1;
					this->ptrWorld->Unit->initInfo.target_grdID = -1;
					//this->ptrWorld->MakeNewUnitsAI();
				}
			}
		}

		//SDL_Delay(50);

		if (this->ptrWorld->teamCount.at(2) < 60) {
			if (this->ptrWorld->runTime > this->lastExecTime + (this->ptrWorld->teamCount.at(2)*1) ) {
				this->lastExecTime = this->ptrWorld->runTime;
				if (this->ptrWorld->MakeUnit("dragoon",1700+(rand()/15*1),1650+(rand()/15*1),2)) {
					this->ptrWorld->Unit->initInfo.useInit = true;
					//this->ptrWorld->Unit->initInfo.state_pri1 = states::AI_PRI1_STATE_DAZED;
					this->ptrWorld->Unit->initInfo.state_pri1 = states::AI_PRI1_STATE_KAMIKAZE;
					this->ptrWorld->Unit->initInfo.state_job = states::AI_JOB_STATE_HUNT;
					//this->ptrWorld->Unit->initInfo.target_atkID = -1;
					//this->ptrWorld->MakeNewUnitsAI();
				}
			}
		}
	}
}

void scenario::SpawnUnits_Circular(std::string name, long x, long y, int team, long numUnits, long circleSize) {
	float angle = 0.0f;
	float incr = PI*2/numUnits;
	long curUnit;
	coord temp;
	temp.x = x;
	temp.y = y;

	for (curUnit = 0; curUnit <= numUnits; curUnit++) {
		angle += Vectorize(angle,incr*curUnit);
		ptrWorld->MakeUnit(name,this->ptrWorld->ptrPhysics->CalcTrajectory(temp,angle,circleSize),team);
		this->ptrWorld->MakeNewUnitsAI();
	}

}