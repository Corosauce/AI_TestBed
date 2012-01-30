#include "ai.h"
#include "math.h"

#include <stdio.h>
#include <string>
#include <windows.h>

#include <cstdlib> 
#include <iostream>

#include "phys_math.h"
#include "physics.h"

#include <tchar.h>
#include "timer.h"
//#include <windows.h>

//#include "thread.h"

ai_agent::ai_agent() {

	this->JobChanges = 0;
	this->nodeCount = 0;
	this->FullPathCount = 0;
	this->NoLOSEntCount = 0;
	this->NoLOSTargCount = 0;

	//Initializing in right order results in proper references
	this->stateToFcn_Pri1.push_back(&ai_agent::Idle);
	this->stateToFcn_Pri1.push_back(&ai_agent::Listen);
	this->stateToFcn_Pri1.push_back(&ai_agent::Protect);
	this->stateToFcn_Pri1.push_back(&ai_agent::Kamikaze);
	this->stateToFcn_Pri1.push_back(&ai_agent::Dazed);

	//Pri 1 default jobs
	stateToDefaultJob_Pri1.push_back(states::AI_JOB_STATE_IDLE);
	stateToDefaultJob_Pri1.push_back(states::AI_JOB_DEFEND_AREA);
	stateToDefaultJob_Pri1.push_back(states::AI_JOB_STATE_FOLLOW);
	stateToDefaultJob_Pri1.push_back(states::AI_JOB_STATE_HUNT);
	stateToDefaultJob_Pri1.push_back(states::AI_JOB_STATE_IDLE);

	//State # -> Job Function Pointer
	this->stateToFcn_Job.push_back(&ai_agent::Wait);
	this->stateToFcn_Job.push_back(&ai_agent::Follow);
	this->stateToFcn_Job.push_back(&ai_agent::Waypoints);
	this->stateToFcn_Job.push_back(&ai_agent::Patrol);
	this->stateToFcn_Job.push_back(&ai_agent::Hunt);
	this->stateToFcn_Job.push_back(&ai_agent::Fight);
	this->stateToFcn_Job.push_back(&ai_agent::Evade);
	this->stateToFcn_Job.push_back(&ai_agent::Pathfind);
	this->stateToFcn_Job.push_back(&ai_agent::StartAI);
	this->stateToFcn_Job.push_back(&ai_agent::DefendArea);

	//Priorities for each state/job above in relevant order
	this->stateToPriority_Job.push_back(9);
	this->stateToPriority_Job.push_back(4);
	this->stateToPriority_Job.push_back(4);
	this->stateToPriority_Job.push_back(4);
	this->stateToPriority_Job.push_back(2);
	this->stateToPriority_Job.push_back(1);
	this->stateToPriority_Job.push_back(1);
	this->stateToPriority_Job.push_back(3);
	this->stateToPriority_Job.push_back(1);
	this->stateToPriority_Job.push_back(2);

	//State # -> 'Muscle Memory' Function Pointer
	this->stateToFcn_MM.push_back(&ai_agent::Idle);
	this->stateToFcn_MM.push_back(&ai_agent::MoveTo);
	this->stateToFcn_MM.push_back(&ai_agent::Chase);
	this->stateToFcn_MM.push_back(&ai_agent::Attack);
	this->stateToFcn_MM.push_back(&ai_agent::Flee);
	this->stateToFcn_MM.push_back(&ai_agent::Dodge);


	this->Nav = new ai_nav;

	this->thread_active = false;

	//# of brains that think per game tick (keep in mind allowed thoughts per second)
	this->prc_ai_int = 1500;

	this->prc_ai_s = 0;
	this->prc_ai_e = 0;

	this->ai_ms_count = 0;
}

ai_agent::~ai_agent() {

}

void ai_agent::Init(Game_Engine* worldpointer) {

	ptrWorld = worldpointer;
	Entities = ptrWorld->Units;
	Player = ptrWorld->Player;

	ts = this->ptrWorld->Map->WorldInfo.tile_size;

	Nav->AI = this;
	Nav->ptrWorld = ptrWorld;
	Nav->ts = ts;

	Nav->PreloadNavs(120);

	tCount = 0;

	pfCount_Map = 0;
	pfCount_WP = 0;
	threadCount = 0;

	int lol = this->ptrWorld->Map->MapSource->MapChangeLayer(1);

	//this->ptrWorld->Map->MapSource->MapG

	if (lol != -1) {
		for (int i = 0; i < this->ptrWorld->Map->WorldInfo.tiles_count_x; i++) {
			for (int j = 0; j < this->ptrWorld->Map->WorldInfo.tiles_count_y; j++) {
				BLKSTR* blockTemp = this->ptrWorld->Map->MapSource->MapGetBlock(i,j);
				if (blockTemp->tl) {
					nodeInfo* tempNode = new nodeInfo;
					tempNode->exists = true;
					tempNode->x = i;
					tempNode->y = j;
					//AddWPNode(tempNode);
					Nav->waypointNodes.push_back(tempNode);
				}
			}
		}
		Nav->FixLOSNodes();
	}

			//AddWPNode(this->ptrWorld->Map->WorldInfo.tiles_count_x

	this->ptrWorld->Map->MapSource->MapChangeLayer(0);

	prc_ai_e += prc_ai_int;
	if (prc_ai_e > Entities.size()) {
		prc_ai_e = Entities.size();
	}

	ptrWorld->ai_count = 0;

}


void ai_agent::Process() {

	long temptime;

	

	//ISSUE!!!!: this method has a small desync issue, eventually causes an untracable crash, been crashing since queue manager was added

	//queueing issue insight: Entities list isnt matching up as anticipated, doesnt update till end of line is hit? wtf?


	//make a safe copy that doesnt get changed from main thread
	this->new_ai_count = this->ptrWorld->ask_ai_count;

	if (this->new_ai_count > 0) {
		
		//NO CODE CAN GO HERE

		//Instantly set false incase main thread gets a new unit call and wants this to be true
		this->ptrWorld->ask_newai = false;
		this->Entities = ptrWorld->Units;

		//using this var to replace --\/
		ptrWorld->AIEntCount = this->Entities.size();

		if (ptrWorld->AIEntCount != ptrWorld->Units.size()) {
			bool hmm = true;
		}
		
		//make a safe copy that doesnt get changed from main thread
		//this->new_ai_count = this->ptrWorld->ask_ai_count;
		
		ptrWorld->ai_count = 0;
		//ptrWorld->AI->ai_ms_count = 0;
		//this code, VERY BAD!!! FIX!!!     ----- it works fine if NAVDATA IS PRECACHED DERP, otherwise this queue stalls all ai processing
		for (long i = ptrWorld->AIEntCount - this->new_ai_count; i < ptrWorld->AIEntCount; i++) {
			ptrWorld->ai_count++;
			Ent = Entities.at(i);

			if (!Ent->fullyCreated) {
				
				
				AddAgent();
				

				this->ptrWorld->ask_ai_count--;
			} else {
				//if this happens, fix code
				bool bad = true;
			}
		}

		

		
	}

	/*while(true) {
		nodeInfo* blah = new nodeInfo[999999];
	}*/

	//if( ptrWorld->sim->get_ticks() > 1000 / THOUGHTS_PER_SECOND ) {

		temptime = ptrWorld->game_time->get_ticks();	
		if (ptrWorld->ai_process) {
			//Thread 
			Think();
		}
		ptrWorld->aiTime = ptrWorld->game_time->get_ticks() - temptime;
		if (ptrWorld->aiTime > ptrWorld->aiTimeTop) {
			ptrWorld->aiTimeTop = ptrWorld->aiTime;
		}

		ptrWorld->sim->start();
		//if (Counter > THOUGHTS_PER_SECOND) { Counter = 0; }

		//this->MakeRandUnit();
    //}


}

void ai_agent::Think() {
	
	//return;

	ptrWorld->ai_count = 0;
	//ai_ms_count = 0;
	

	//__int64 ctr1 = 0, ctr2 = 0, freq = 0;
	//int acc = 0, i = 0;
	
	

	for (int i = 0;i < ptrWorld->AIEntCount;i++)
	{
		//Setup for all functions to refer to current unit using Ent
		Ent = Entities[i];
		EntMemory = Ent->ptrBrain;
		ptrWorld->ai_count = 2000+rand();
		
		//Nav->Ent = Ent; removed for threading
		if (Ent->declareDead) {
			//setting false just in case of ressurection? and to prevent multikill
			Ent->declareDead = false;
			KillAgent();
		}

		if (Ent->phys_state != states::PHYS_STATE_DEAD && Ent->fullyCreated) {

			
			Ent->acceleration = 0.0f;
			EntMemory = Ent->ptrBrain;
			//EntMemory = brainMemory[i];
			//Nav->EntMemory = EntMemory;
			
			(this->*EntMemory->ptrThought_Pri1)();
			
			//EntMemory = brainMemory[i];
			if (EntMemory->safe) {
				
				(this->*EntMemory->jobData->ptrJobAction)();
				
			}
			(this->*EntMemory->ptrThought_MM)();
			//AttackStart();
			//TEMP DISABLED FOR DEBUGGING AI SLOWDOWN FOR 80+ UNITS
			CheckCollision();
			SDL_Delay(1);


			
		}
		
		
	}
	
	/*if (prc_ai_e == Entities.size()) {
		prc_ai_s = 0;
		prc_ai_e = prc_ai_int;
		if (prc_ai_e > Entities.size()) {
			prc_ai_e = Entities.size();
			prc_ai_s = prc_ai_e-prc_ai_int;
			if (prc_ai_s < 0) {
				prc_ai_s = 0;
			}
		}
	} else {
		prc_ai_s += prc_ai_int;
		prc_ai_e += prc_ai_int;
		if (prc_ai_e > Entities.size()) {
			prc_ai_e = Entities.size();
			prc_ai_s = prc_ai_e-prc_ai_int;
			if (prc_ai_s < 0) {
				prc_ai_s = 0;
			}
		}

		
	}*/

	

}

void ai_agent::AddAgent() {

	
	tempBrain = new sprite_memory;

	brainMemory.push_back(tempBrain);
	EntMemory = brainMemory.at(brainMemory.size()-1);
	this->unitNum++;
	//this->Entities = ptrWorld->Units;
	//Ent = Entities.at(Entities.size()-1);

	//Ent ptr is now set in ai_agent::Process() before addagent is called
	Ent->ptrBrain = EntMemory;

	//State Inits
	SetMMState(states::AI_MM_IDLE);
	tempBrain->jobData = new jobInfo();
	tempBrain->jobData->nextJob = NewJob(tempBrain->jobData,states::AI_JOB_STATE_BIRTH);
	SetPri1State(states::AI_PRI1_STATE_SELF);

	//unused?
	tempBrain->collision = false;
	
	//tempBrain->hasNav = false;

	//Get some nav memory, GetNav returns recycled navdata or new if all else used up
	
	tempBrain->navData = Nav->GetNav();
	
	
	tempBrain->safe = true;
	tempBrain->dangerlevel = 0;
	tempBrain->defaultJob = this->stateToDefaultJob_Pri1.at(tempBrain->state_pri1);

	
	StartNextJob();
	

	tempBrain->hasTargetToGuard = false;

	//this->ptrWorld->lock_newunit = false;
	//this->ptrWorld->ask_newai = false;
	Ent->fullyCreated = true;

	//not used
	Ent->fullyCreatedSafety = 44;
	

	//this->SetTarget(this->ptrWorld->ptrPhysics->GetClosestSpriteID((((phys_obj* )Ent)),true));
	//this->SetTarget(this->getRandEnemyID());
	
	

	//int dsdfsd = this->Entities.size();
}

void ai_agent::KillAgent() {

	//Ent = ptrWorld->Units[agentID];
	//EntMemory = Ent->ptrBrain;//AI->brainMemory[agentID];
	SetMMState(states::AI_MM_DEAD);
	Nav->ReleaseNav(EntMemory->navData);
}

void ai_agent::KillAgent(long agentID) {

	Ent = ptrWorld->Units[agentID];
	EntMemory = Ent->ptrBrain;//AI->brainMemory[agentID];
	SetMMState(states::AI_MM_DEAD);

	Nav->ReleaseNav(EntMemory->navData);
}

void ai_agent::RemoveAgent(long agentID) {

	//not used, units never deleted to keep vectors order, fix and use if memory becomes issue over time

	int i;
	for (i = 0; i < brainMemory.size(); i++) {
		if (agentID == Entities.at(i)->ID) {
			break;
		}
	}

	tempBrain = brainMemory.at(i);

	itB = brainMemory.begin();
	itB += agentID;
	brainMemory.erase(itB);

	delete tempBrain;

	this->unitNum--;
	
}


/* Job Init functions */

/* INSTANT FUNCTIONS */

/* */


void ai_agent::SetPri1State(int state) {
	EntMemory->state_pri1 = state;
	EntMemory->ptrThought_Pri1 = this->stateToFcn_Pri1.at(state);
	EntMemory->defaultJob = this->stateToDefaultJob_Pri1.at(state);
}
void ai_agent::SetMMState(int state) {
	if (EntMemory->state != state) {
		EntMemory->state = state;
		EntMemory->state_mm = EntMemory->state;
		Ent->ai_mm_state = state;
		
		EntMemory->ptrThought_MM = this->stateToFcn_MM.at(state);
		this->ptrWorld->testCount++;
		//Ent->animation_list.curAnimID = 0;
		
		/*switch(state) {
			case 0:
				if (Ent->animation_list.initialized) {
					Ent->animation_list.Start(ANIM_STATE_IDLE);
				}
				break;
			case 1: //move
				Ent->animation_list.Start(ANIM_STATE_MOVE);
				break;
			case 2:
				Ent->animation_list.Start(ANIM_STATE_MOVE);
				break;
			case 3:
				Ent->animation_list.Start(ANIM_STATE_MOVE);
				break;
			case 4:	//attack
				Ent->animation_list.PlayOnce(ANIM_STATE_ATTACK,ANIM_STATE_MOVE);
				//Ent->animation_list.Stop();
				break;
			case 10: //death
				Ent->animation_list.Start(ANIM_STATE_DEATH);
				break;
			default:
				//Ent->animation_list.Start(state);
				break;
		}*/
	}
}

void ai_agent::SetAnimState(int state) {
	if (state != Ent->animation_list.curAnimID) {
		Ent->animation_list.Stop();
		Ent->animation_list.Start(state);
	}
}


/* */
void ai_agent::Wander() {
	
	GotoPoint((rand() % (int)this->ptrWorld->Map->WorldInfo.width),(rand() % (int)this->ptrWorld->Map->WorldInfo.height));
}

void ai_agent::GotoPoint(int x, int y) {
	this->Ent->target.ID = -2;
	this->Ent->target.x = x;
	this->Ent->target.y = y;

	//position.x = 0.0f + (rand() % (int)Ent->ptrWorld->Map->WorldInfo.width);
	//position.y = 0.0f + (rand() % (int)Ent->ptrWorld->Map->WorldInfo.height);
}

void ai_agent::SetPri1Target(phys_obj* targ) {
	if (targ->ID > -2) {
		if ((targ->phys_state != states::PHYS_STATE_DEAD) || (Ent->ID != Ent_Target->ID)) {
			Ent_Target = targ;
			Ent->target.ID = targ->ID;
			EntMemory->targetList.push_front(targ);//this->Wander();
		}
	} else { delete targ; }
}

void ai_agent::SetPri1Target(long targID) {
	if (targID > -2) { 
		SetPri1Target(this->ptrWorld->ptrPhysics->getSprite(targID));
	}
}
void ai_agent::UnSetGuardTarget() {
	EntMemory->hasTargetToGuard = false;
}
void ai_agent::SetGuardTarget(phys_obj* targ) {
	if (targ->ID > -2) {
		if ((targ->phys_state != states::PHYS_STATE_DEAD) || (Ent->ID != Ent_Target->ID)) {
			EntMemory->targetGuard = targ;
			EntMemory->hasTargetToGuard = true;
		}
	} else { 
		EntMemory->hasTargetToGuard = false;
		delete targ;
	}
}
void ai_agent::SetGuardTarget(long targID) {
	if (targID > -2) { 
		SetGuardTarget(this->ptrWorld->ptrPhysics->getSprite(targID));
	}
}

void ai_agent::UpdateTarget() {
	//this->ptrWorld->los_checks++;
	if (EntMemory->targetList.size() > 0) {

		Ent_Target = EntMemory->targetList.front();

		if (Ent_Target->ID > -2) {
			if ((Ent_Target->phys_state == -1) || (Ent->ID == Ent_Target->ID)) {
				AttackStop();
				EntMemory->targetList.pop_front();
			} else {
				//Ent->target.x = EntMemory->ptrTarget->position.x;
				//Ent->target.y = EntMemory->ptrTarget->position.y;
				Ent->target.x = Ent_Target->position.x;
				Ent->target.y = Ent_Target->position.y;
				Ent->target.distance = getDistance(Ent->position.x,Ent->position.y,Ent->target.x,Ent->target.y);
				Ent->target.angle = getHeading(Ent->position.x,Ent->position.y,Ent->target.x,Ent->target.y);
			}
		} else {
			EntMemory->targetList.pop_front();
			
		}
	} else {
		
		if (EntMemory->hasTargetToGuard) {
			
			Ent_Target = EntMemory->targetGuard;

			if (Ent_Target->ID > -2) {
				if ((Ent_Target->phys_state == -1) || (Ent->ID == Ent_Target->ID)) {		
					AttackStop();
					//EntMemory->targetList.pop_front();
				} else {
					//Ent->target.x = EntMemory->ptrTarget->position.x;
					//Ent->target.y = EntMemory->ptrTarget->position.y;
					Ent->target.x = Ent_Target->position.x;
					Ent->target.y = Ent_Target->position.y;
					Ent->target.distance = getDistance(Ent->position.x,Ent->position.y,Ent->target.x,Ent->target.y);
					Ent->target.angle = getHeading(Ent->position.x,Ent->position.y,Ent->target.x,Ent->target.y);
				}
			}
		}
	}


}

void ai_agent::SetGlobalTarget(long targID) {
	for (int i = 0;i < Entities.size();i++)
	{
		Ent = Entities[i];
		if (Ent->phys_state != states::PHYS_STATE_DEAD) {
			EntMemory = brainMemory[i];
			SetPri1Target(targID);
			//threada::startPathfind(Ent_Target->position.x,Ent_Target->position.y,Nav);
			Nav->SetupPathfind(Ent_Target->position.x,Ent_Target->position.y);
		}
	}
}


long ai_agent::getRandAliveUnitID() {
	bool found = false;
	long temp = -2;
	long attempts = 0;
	while (!found) {
		temp = rand() % (this->ptrWorld->Units.size()-1);
		if (this->Entities.at(temp)) {
			if ((this->Entities.at(temp)->phys_state != states::PHYS_STATE_DEAD)) {
				found = true;
			}
		}
		attempts++;
		if (attempts > 100) { 
			return Ent->ID;
		}
	}
	return temp;
}
// includes alive only
long ai_agent::getRandEnemyID() {
	bool found = false;
	long temp = -2;
	long attempts = 0;
	phys_obj* tempEnt;
	while (!found) {
		if (this->ptrWorld->Units.size() > 0) {
			temp = (rand() % (this->Entities.size()))-1;
			if (temp > -1) {
				tempEnt = (phys_obj*)this->Entities.at(temp);
			} else if (temp == -1) {
				tempEnt = (phys_obj*)this->Player;
			}

			if (tempEnt) {
				if ((tempEnt->phys_state != states::PHYS_STATE_DEAD) && (this->Ent->team != tempEnt->team)) {
					found = true;
				}
			}
			attempts++;
			if (attempts > 100) { 
				return Ent->ID;
			}
		} else { break; }
	}
	return temp;
}
void ai_agent::CheckCollision() {
	EntMemory->phys_state = Ent->getState();

	if (EntMemory->phys_state == states::PHYS_STATE_COLLIDE_MAP) {

		//Stop();
		
		/*EntMemory->jobData->jobVars.SleepUntil = Ent->ptrWorld->runTime + 30;
		EntMemory->jobData->ptrMM = &ai_agent::Wait;
		EntMemory->jobData->nextJob = NewJob(EntMemory->jobData);
		EntMemory->jobData->nextJob->ptrMM = &ai_agent::Listen;*/

		//EntMemory->hasTarget = false;

		//WHAT?! FIX MEEEEEEEEEEEEEEE
		if (EntMemory->state_job == states::AI_JOB_STATE_WAYPOINTS) {
			if (EntMemory->hasTarget()) {
				//threada::startPathfind(Ent_Target->position.x,Ent_Target->position.y,Nav);
				/*if (LOSCheck(Ent->position.x/ts,Ent->position.y/ts,X/ts,Y/ts)) {
					bool weee = true;
					EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_WAYPOINTS);
					StartNextJob();
					//dont pathfind, move to target
				}*/
				Nav->SetupPathfind(Ent_Target->position.x,Ent_Target->position.y);
				//EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_PATHFIND);
				StartNextJob();
			}
		} 

	} else if (EntMemory->phys_state == states::PHYS_STATE_COLLIDE_UNIT) {
		//Stop();

		//EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_EVADE);
		//StartNewJob();
	}
}
bool ai_agent::IsEnemy(phys_obj* targ) {
	return (Ent->team != targ->team);
}

bool ai_agent::IsEnemy(long targ) {
	return (Ent->team != Ent_Target->ID);
	//return (Ent->team != targ->team);
}

bool ai_agent::LOSCheck(nodeInfo* node1, nodeInfo* node2) {
	//float ts = ptrWorld->Map->WorldInfo.tile_size;
	
	float heading = getHeading(node1->x*ts+(ts/2),node1->y*ts+(ts/2),node2->x*ts+(ts/2),node2->y*ts+(ts/2));
	float distance = getDistance(node1->x*ts+(ts/2),node1->y*ts+(ts/2),node2->x*ts+(ts/2),node2->y*ts+(ts/2));

	//heading += 0.00000001f;

	if (distance <= ts) {
		return true;
	}

	for (float tempDist = 0; tempDist < distance; tempDist += ts/4) {
		//long tile = getRelX(physPtr1,heading,tempDist);
		long x2 = (node1->x*ts)+(ts/2);
		long y2 = (node1->y*ts)+(ts/2);
		int x1 = (int)(this->ptrWorld->ptrPhysics->getRelX(x2,heading,tempDist)/ts);
		int y1 = (int)(this->ptrWorld->ptrPhysics->getRelY(y2,heading,tempDist)/ts);
		BLKSTR* blockTemp = this->ptrWorld->Map->MapSource->MapGetBlock(x1,y1);
		if (blockTemp->tl) {
			return false;
		}
	}
	return true;
}

bool ai_agent::LOSCheck(long x1, long y1, long x2, long y2) {
	//Must supply tile coordinates
	if (x1 == x2) {
		int aaaaaa = 0;
	}

	float heading = getHeading(x1*ts+(ts/2),y1*ts+(ts/2),x2*ts+(ts/2),y2*ts+(ts/2));
	float distance = getDistance(x1*ts+(ts/2),y1*ts+(ts/2),x2*ts+(ts/2),y2*ts+(ts/2));

	//heading += 0.00000001f;

	

	if (distance <= ts) {
		return true;
	}

	for (float tempDist = 0; tempDist < distance; tempDist += ts/4) {
		//long tile = getRelX(physPtr1,heading,tempDist);
		long xx2 = (x1*ts)+(ts/2);
		long yy2 = (y1*ts)+(ts/2);
		int xx1 = (int)(this->ptrWorld->ptrPhysics->getRelX(xx2,heading,tempDist)/ts);
		int yy1 = (int)(this->ptrWorld->ptrPhysics->getRelY(yy2,heading,tempDist)/ts);
		BLKSTR* blockTemp = this->ptrWorld->Map->MapSource->MapGetBlock(xx1,yy1);
		if (blockTemp->tl) {
			return false;
		}
	}
	return true;
}







bool ai_agent::LOSCheck(phys_obj* physPtr1, phys_obj* physPtr2) {
	//this->ptrWorld->los_checks++;
	float heading = getHeading(physPtr1->position.x,physPtr1->position.y,physPtr2->position.x,physPtr2->position.y);
	float distance = getDistance(physPtr1->position.x,physPtr1->position.y,physPtr2->position.x,physPtr2->position.y);

	for (float tempDist = 0; tempDist < distance; tempDist += ptrWorld->Map->WorldInfo.tile_size/2) {
		//long tile = getRelX(physPtr1,heading,tempDist);
		BLKSTR* blockTemp = this->ptrWorld->Map->MapSource->MapGetBlock(this->ptrWorld->ptrPhysics->getRelX(physPtr1->position.x,heading,tempDist)/ptrWorld->Map->WorldInfo.tile_size,this->ptrWorld->ptrPhysics->getRelY(physPtr1->position.y,heading,tempDist)/ptrWorld->Map->WorldInfo.tile_size);
		if (blockTemp->tl) {
			return false;
		}
	}
	return true;
}

phys_obj* ai_agent::GetClosestSpriteID(phys_obj* physPtr, bool findEnemy = false) {
	long tileNum = 0;
	float dir = 0.0f;
	int range = (physPtr->viewdistance/3);

	for (range; range < physPtr->viewdistance; range+=physPtr->viewdistance/3) {
		for (dir = 0; dir < atan(1.0f)*4; dir+= atan(1.0f)/2) {
			for (long lr = 1; lr >= -1; lr-=2) {
				tileNum = ptrWorld->ptrPhysics->getRelativeTile(physPtr,Vectorize(physPtr->vector,dir*lr),range);
				if (tileNum > 0 && tileNum < this->ptrWorld->Map->WorldInfo.total_tiles) {
				//ptrWorld->ptrPhysics->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
				//this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);

					if (this->ptrWorld->Map->tile_map[tileNum].Occupied) {
						for (int i = 0; i < this->ptrWorld->Map->OccupyMax; i++) {
							TempEnt = getSprite(this->ptrWorld->Map->tile_map[tileNum].OccupyData[i]);
							if (physPtr->team == TempEnt->team) {
								if (!findEnemy) {
									return TempEnt;
								}
							} else {
								if (findEnemy) {
									return TempEnt;
								}
							}
						}
					}
				}
			}
			
		}
	}
	

	return new phys_obj(-2);
}

phys_obj* ai_agent::GetClosestVisibleSprite(phys_obj* physPtr, bool findEnemy = false) {

	//1: try a sprite to sprite los comparison for every sprite, will probably be really slow with 80+ units
	//result: at just 15 seconds in, aitime hit 1000ms process time, hilariously slow method

	//2: try varied awareness tile checks, ones for long distance checking and ones for accurate close distance checking
	//result: ???

	long dist;
	long ID = -2;
	long closest = 999999;
	long size = Entities.size();
	for (int i = -1; i < size; i++) {
		//if (Entities.size()
		TempEnt = getSprite(i);
		if (TempEnt->phys_state != states::PHYS_STATE_DEAD) {
			if (physPtr->team == TempEnt->team) {
				if (!findEnemy) {
					if (LOSCheck(physPtr, TempEnt)) {
						dist = getDistance(physPtr->position.x,physPtr->position.y,TempEnt->position.x,TempEnt->position.y);
						if (dist < closest) { closest = dist; ID = TempEnt->ID; }

					}
				}
			} else {
				if (findEnemy) {
					if (LOSCheck(physPtr, TempEnt)) {
						dist = getDistance(physPtr->position.x,physPtr->position.y,TempEnt->position.x,TempEnt->position.y);
						if (dist < closest) { closest = dist; ID = TempEnt->ID; }

					}
				}
			}
		}
		//SDL_Delay(1);
	}

	if (ID != -2) {
		return getSprite(ID);
	} else {
		return new phys_obj(-2);
	}
	
}

phys_obj* ai_agent::GetClosestVisibleSprite2(phys_obj* physPtr, bool findEnemy = false) {

	//because of the order of tile checking, the first entity found is basicly the closest, so it can instantly return and not
	//waste time calculating and comparing the distance of every entitity, fairly efficient, but has holes put close, more resoltuion needed



	//TEMP FUNCTION DISABLER
	//return new phys_obj(-2);





	long tileNum = 0;
	Tile* ptrTile;
	//Tile* ptrTile2;
	//Tile* ptrTile3;
	long tempnum = 0;
	float dir = 0.0f;
	int range = (physPtr->viewdistance/3);

	

	for (range; range < physPtr->viewdistance; range+=physPtr->viewdistance/3) {
		for (dir = 0; dir < atan(1.0f)*4; dir+= atan(1.0f)/2) {
			for (long lr = 1; lr >= -1; lr-=2) {

				

				tileNum = ptrWorld->ptrPhysics->getRelativeTile(physPtr,Vectorize(physPtr->vector,dir*lr),range);
				
				
				

				if (tileNum > 0 && tileNum < this->ptrWorld->Map->WorldInfo.total_tiles) {

					while(this->ptrWorld->lock_tile_map) { }
					
					//ptrTile2 = &this->ptrWorld->Map->tile_map_temp[tileNum];
					//ptrTile3 = &this->ptrWorld->Map->tile_map_swap[tileNum];
					//this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
					//this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
					ptrTile = &this->ptrWorld->Map->tile_map[tileNum];
					if (ptrTile->Occupied) {
						//long od = ptrTile->OccupyData;
						//this->ptrWorld->los_checks++;
						//while(this->ptrWorld->lock_tile_map) { }

						
						
						//int tempp = od.size();
						for (int i = 0; i < ptrTile->OccupyCount; i++) {
							tempnum = ptrTile->OccupyData[i];
							if (tempnum != -2) {
								
								TempEnt = getSprite(tempnum);

								if (physPtr->team == TempEnt->team) {
									if (!findEnemy) {
										if (this->LOSCheck(physPtr,TempEnt)) {
											
											return TempEnt;
										}
									}
								} else {
									if (findEnemy) {
										if (this->LOSCheck(physPtr,TempEnt)) {
											
											return TempEnt;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	return new phys_obj(-2);
}

phys_obj* ai_agent::getSprite(long ID) {

	//if player
	if (ID == -1) {
		return this->ptrWorld->Player;
	}

	return this->Entities[ID];
	//return this->ptrWorld->AI->Entities[ID];
	//return Entities.at(ID);

	//caused REALLY SLOW issues with 80+ entities called thru get closest sprite
	/*for (int i = 0; i < this->Entities.size(); i++) {
		if (ID == Entities.at(i)->ID) {
			return Entities.at(i);
		}
	}*/
	return new phys_obj(-2);
}