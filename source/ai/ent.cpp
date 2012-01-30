#include "../ai.h"

#include "math.h"

#include <stdio.h>
#include <string>
#include <windows.h>

#include <cstdlib> 
#include <iostream>

#include "../phys_math.h"
#include "../physics.h"

#include "../timer.h"

jobInfo* ai_agent::NewJob(jobInfo *ptrCurJob, long newJobState) {

	jobInfo* temp = new jobInfo;
	temp->state_job = newJobState;
	Ent->ai_job_state = newJobState;

	temp->prevJob = ptrCurJob;
	temp->ptrJobAction = this->stateToFcn_Job.at(newJobState);
	
	ptrCurJob->noNextJob = false;
	ptrCurJob->jobGo = 10;
	temp->noNextJob = true;
	temp->jobVars.newTarget = false;


	//template idle job for safety bailout, check if frees its memory when its not needed
	jobInfo* temp2 = new jobInfo;
	temp2->noNextJob = true;
	temp2->state_job = states::AI_JOB_STATE_IDLE;
	temp2->prevJob = temp;
	temp2->ptrJobAction = this->stateToFcn_Job.at(temp2->state_job);


	//debug
	this->JobChanges++;
	ptrWorld->AILogJob(newJobState);

	

	return temp;

}

jobInfo* ai_agent::NewJob(jobInfo *ptrCurJob, long newJobState, long waitTime) {

	
	//Make the new job after wait job
	jobInfo* temp2 = new jobInfo;
	temp2->state_job = newJobState;
	temp2->ptrJobAction = this->stateToFcn_Job.at(newJobState);
	temp2->jobVars.newTarget = false;

	//Make a wait job
	jobInfo* temp = new jobInfo;
	temp->state_job = states::AI_JOB_STATE_IDLE;
	temp->prevJob = ptrCurJob;
	temp->nextJob = temp2; //IMPORTANT!
	temp->ptrJobAction = this->stateToFcn_Job.at(temp->state_job);
	temp->jobVars.SleepUntil = ptrWorld->runTime + waitTime;
	ptrCurJob->noNextJob = false;
	ptrCurJob->jobGo = 10;
	
	temp2->prevJob = temp;

	//IMPORTANT!
	ptrCurJob->nextJob = temp;

	
	return temp;
}

void ai_agent::StartNextJob() {
	AttackStop();
	if (this->EntMemory->jobData->jobGo == 10) {
		//this->JobChanges++;
		this->EntMemory->jobData = this->EntMemory->jobData->nextJob;
		this->EntMemory->state_job = this->EntMemory->jobData->state_job;
	}
}


void ai_agent::TryNextJob() {
	if (!EntMemory->jobData->jobGo == 10) {
		this->EntMemory->jobData = this->EntMemory->jobData->nextJob;
		this->EntMemory->state_job = this->EntMemory->jobData->state_job;
	} else {
		EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,EntMemory->defaultJob);
		StartNextJob();
	}
}
//AI Logic Main Function



void ai_agent::StartAI() {

	//this code might go better in addagent, rather than having to wait for a thought process first here
	if (Ent->initInfo.useInit) {
		this->SetPri1State(Ent->initInfo.state_pri1);
		this->SetPri1Target(Ent->initInfo.target_atkID);
		this->SetGuardTarget(Ent->initInfo.target_grdID);
		this->EntMemory->jobData->nextJob = NewJob(this->EntMemory->jobData,Ent->initInfo.state_job);

		if (Ent->initInfo.target_atkID > -2) {
			Nav->SetupPathfind(getSprite(Ent->initInfo.target_atkID));
		}

	} else {

		if (Ent->team == 3) {
			this->SetPri1State(states::AI_PRI1_STATE_SELF);
			//this->SetGuardTarget((phys_obj*)Player);
			//this->SetupPathfind(EntMemory->targetGuard);

			
		} else if (Ent->team == 1) {
			this->SetPri1State(states::AI_PRI1_STATE_PROTECT);

			//SESH SAVE LAST STATE HERE
			this->SetGuardTarget(-1);
			this->EntMemory->jobData->nextJob = NewJob(this->EntMemory->jobData,states::AI_JOB_STATE_FOLLOW);
			
			//Nav->SetupPathfind(getSprite(-1));
		} else {
			this->SetPri1State(states::AI_PRI1_STATE_KAMIKAZE);
			//threada::startPathfind(Ent_Target->position.x,Ent_Target->position.y,Nav);
			long targ = this->getRandEnemyID();
			//this->SetPri1Target(targ);
			Nav->SetupPathfind(getSprite(targ));
		}
		//this->EntMemory->jobData->nextJob = NewJob(this->EntMemory->jobData,states::AI_JOB_STATE_PATHFIND);
	}
	StartNextJob();
}

int ai_agent::GetState(const int state) {
	return state;
}

/* Pri1 functions */
void ai_agent::Listen() {

return;
	


	//EntMemory->safe = false;
	phys_obj* targ2 = GetClosestSpriteID((((phys_obj* )Ent)),true);

	phys_obj* targ;
	UpdateTarget();
	if (!EntMemory->hasTarget()) {
		targ = getSprite(this->getRandEnemyID());
	} else {
		targ = (phys_obj*)Ent_Target;
	}
	 //= (phys_obj*)Player;//GetClosestSpriteID((((phys_obj* )Ent)),true);
	
	//If unit found
	if (targ->ID > -2) {
		if (ptrWorld->ptrPhysics->LOSCheck((phys_obj* )Ent,targ)) {
			if (EntMemory->state_job != GetState(states::AI_JOB_STATE_HUNT)) {
				//if (EntMemory->state_job != GetState(states::AI_JOB_STATE_PATHFIND)) {
					SetPri1Target(targ->ID);
					this->EntMemory->jobData->nextJob = NewJob(this->EntMemory->jobData,states::AI_JOB_STATE_HUNT);
					StartNextJob();
				//}
			}

		} else {
			if ((EntMemory->state_job == states::AI_JOB_STATE_HUNT) || (EntMemory->state_job != GetState(states::AI_JOB_STATE_PATHFIND))) {
					//EntMemory->hasTarget = false;
					//threada::startPathfind(Ent_Target->position.x,Ent_Target->position.y,Nav);
					Nav->SetupPathfind(targ->position.x,targ->position.y);
					//this->EntMemory->jobData->nextJob = NewJob(this->EntMemory->jobData,states::AI_JOB_STATE_PATHFIND);
					StartNextJob();
			}
			//}
		}
	} else {
		if (EntMemory->state_job != states::AI_JOB_STATE_WAYPOINTS) {
			/*EntMemory->hasTarget = false;
			this->SetupPathfind(targ->position.x,targ->position.y);
			this->EntMemory->jobData->nextJob = NewJob(this->EntMemory->jobData,states::AI_JOB_STATE_PATHFIND);
			StartNewJob();*/
		}
	}
}

void ai_agent::Protect() {

	
	
	//new phys_obj(-2);
	//phys_obj* targ = new phys_obj(-2);//this->getSprite(this->getRandEnemyID());
	phys_obj* targ = GetClosestVisibleSprite((phys_obj*)Ent,true);
	

	
	if (targ->ID > -2) {
		if (this->stateToPriority_Job.at(this->EntMemory->state_job) > 2) {
			this->SetPri1Target(targ);
			EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_FIGHT);
			StartNextJob();
		}
	} else { delete targ; }

	
}
void ai_agent::Kamikaze() {
	
	phys_obj* targ;

	//targ = GetClosestVisibleSprite((phys_obj*)Ent,true);
	//phys_obj* targ = new phys_obj(-2);//this->getSprite(this->getRandEnemyID());

	
	if (!EntMemory->hasTarget()) {
		//targ = this->getSprite(this->getRandEnemyID());
		targ = GetClosestVisibleSprite((phys_obj*)Ent,true);
	} else {
		targ = new phys_obj(-2);
	}

	if (targ->ID > -2) {
		if (this->stateToPriority_Job.at(this->EntMemory->state_job) > 3) {
			this->SetPri1Target(targ);
			EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_FIGHT);
			StartNextJob();
		}
	} else { delete targ; }
}

void ai_agent::Dazed() {

	float test = rand();
	test = test/30000;
	test = 0.2;
	test = getHeading(Ent->position.x,Ent->position.y,Player->position.x,Player->position.y);
	test = Vectorize(test-Ent->targetvector,0);
	Ent->targetvector = Vectorize(Ent->targetvector,test/8);
	
	Attack();
	Forward();



}
/* Job functions */
void ai_agent::Wait() {
	if (Ent->ptrWorld->runTime > EntMemory->jobData->jobVars.SleepUntil) {
		
		//Ent->target.ID = rand() % (int)unitNum;
		//this->Wander();
		//this->SetTarget(this->Entities.at(this->getRandAliveUnitID())->ID);
		//this->SetTarget(this->getRandEnemyID());
		//this->SetTarget(GetClosestSpriteID((((phys_obj* )Ent)),true));

		if (this->EntMemory->jobData->jobVars.newTarget) {
			//SetupPathfind(this->EntMemory->jobData->jobVars.targPos.x,this->EntMemory->jobData->jobVars.targPos.y);
		}

		this->EntMemory->jobData = this->EntMemory->jobData->nextJob;

		
	}
}



void ai_agent::Hunt() {
	

	//Update all calculations for target
	UpdateTarget();
	if (EntMemory->hasTarget()) {
		Ent->targetvector = Ent->target.angle;

		if (Ent->targetvector == 0) {
			if (Ent->position.x > Ent->target.x) {
				Ent->targetvector = Ent->velocity*-1;
			} else { Ent->targetvector = Ent->velocity; }
		}

		//Move towards if not close
		if (ptrWorld->ptrPhysics->LOSCheck((phys_obj* )Ent,(phys_obj* )Ent_Target)) {
			if (Ent->target.distance > Ent->weapons[0].range) {

				if (Ent->velocity < Ent->cruisespeed) {
					Forward();
				}

			} else {

				//this->ptrAction = &ai_agent::Stop;
				if (this->Ent_Target->phys_state == states::PHYS_STATE_DEAD) {
					Stop();
					//Attack();
					//EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_HUNT);
					//StartNextJob();
					AttackStop();
					UpdateTarget();
					//EntMemory->hasTarget = false;
					//doCurrentAction() = &Wander();
					//doCurrentAction();
				} else {
					if (Ent->ID != Ent_Target->ID) {
						AttackStart();
					}
				}
			}
		} else {
			if (EntMemory->hasTarget()) {
				//EntMemory->hasTarget = false;
				Nav->SetupPathfind(EntMemory->targetList.front());
				//EntMemory->pathing = true;
				//threada::startPathfind(EntMemory->targetList.front()->position.x,EntMemory->targetList.front()->position.y,Nav);
				//EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_PATHFIND);
				StartNextJob();
			}
		}
	} else {
		//threada::startPathfind(getSprite(this->getRandEnemyID())->position.x,getSprite(this->getRandEnemyID())->position.y,Nav);
		Nav->SetupPathfind(getSprite(this->getRandEnemyID()));
		//EntMemory->pathing = true;
		//EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_PATHFIND);
		//EntMemory->jobData->nextJob = AI->NewJob(EntMemory->jobData,states::AI_JOB_STATE_IDLE);
		StartNextJob();
	}
	
}


void ai_agent::Waypoints() {
	
	//TEMP OFF
	if (EntMemory->hasTarget()) {
		UpdateTarget();
		if (LOSCheck((phys_obj*) Ent,(phys_obj*) EntMemory->targetList.front())) {
			//if we see prime target code goes here

			//this code is kinda useless if pri1 job is kamikaze, aka will see closest target
			EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_FIGHT);
			StartNextJob();
		}
	} else if (EntMemory->hasTargetToGuard) {
		if (LOSCheck((phys_obj*) Ent,(phys_obj*) EntMemory->targetGuard)) {
			//if we see prime target code goes here

			//this code is kinda useless if pri1 job is kamikaze, aka will see closest target
			EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_FOLLOW);
			StartNextJob();
		}
	}

	EntMemory->phys_state = Ent->getState();

	if (EntMemory->navData->curPathNode < EntMemory->navData->Path.size()) {

		//1 node ahead LOS check to prevent pointless backtracking (note: hopefully doesnt break wallhit backtracks)
		if (EntMemory->phys_state != states::PHYS_STATE_COLLIDE_MAP) {
			if (EntMemory->navData->curPathNode+1 < EntMemory->navData->Path.size()) {
				long x2 = EntMemory->navData->Path.at(EntMemory->navData->curPathNode+1)->x;
				long y2 = EntMemory->navData->Path.at(EntMemory->navData->curPathNode+1)->y;
				
				if (LOSCheck((int)(Ent->position.x/32),(int)(Ent->position.y/32),x2,y2)) {
					EntMemory->navData->curPathNode++;
					return;
				}
			}
		}

		//Proximity check to target node
		long x = (EntMemory->navData->Path.at(EntMemory->navData->curPathNode)->x*32)+16;
		long y = (EntMemory->navData->Path.at(EntMemory->navData->curPathNode)->y*32)+16;
		float dist = getDistance(Ent->position.x,Ent->position.y,x,y);
		if (dist < 64) {
			EntMemory->navData->curPathNode++;
			return;
		}

		//Vector Adjustment
		Ent->targetvector = getHeading(Ent->position.x,Ent->position.y,x,y);
		if (Ent->targetvector == 0) {
			if (Ent->position.x > x) {
				Ent->targetvector = Ent->velocity*-1;
			} else { Ent->targetvector = Ent->velocity; }
		}
			
	} else {
		
		//The next pre-setup job is then called once we found end of path.
		TryNextJob();
		
	}


	if (Ent->velocity < Ent->cruisespeed) {
		Forward();
	}

	
	if (EntMemory->phys_state == states::PHYS_STATE_COLLIDE_MAP) {
		//if (Ent->ai_job_state == states::AI_JOB_STATE_WAYPOINTS) {
			EntMemory->navData->curPathNode-=1;
			if (EntMemory->navData->curPathNode < 0) {
				EntMemory->navData->curPathNode = 0;	
			} else {
				int fdgdfg = 0;
			}
		//}
	}
}
void ai_agent::Patrol() {
	//UpdateTarget();
	//EntMemory->jobData->jobVars.SleepUntil = Ent->ptrWorld->runTime + 1000;
	//EntMemory->jobData->ptrMM = &ai_agent::Wait;
	int aa = 1;
}


void ai_agent::Follow() {

	if (EntMemory->hasTargetToGuard) {
		if (!EntMemory->hasTarget()) {
			//this->SetPri1Target(EntMemory->targetGuard);
		}

		//if (true) {
		if (LOSCheck((phys_obj*) Ent,(phys_obj*) EntMemory->targetGuard)) {
			UpdateTarget();

			Ent->targetvector = Ent->target.angle;

			if (Ent->targetvector == 0) {
				if (Ent->position.x > Ent->target.x) {
					Ent->targetvector = Ent->velocity*-1;
				} else { Ent->targetvector = Ent->velocity; }
			}

			if (Ent->velocity < Ent->maxforcespeed) {
				if (Ent->target.distance > 200) {
					Forward();
				} else { Stop(); }
			}

		} else {
			//Stop();

			Nav->SetupPathfind(EntMemory->targetGuard);
			//this->EntMemory->jobData->nextJob = NewJob(this->EntMemory->jobData,states::AI_JOB_STATE_PATHFIND);
			StartNextJob();
		}

	}

}
void ai_agent::Fight() {
	UpdateTarget();
	if (EntMemory->hasTarget()) {
		if (LOSCheck((phys_obj*) Ent,(phys_obj*) Ent_Target)) {
			if (IsEnemy(Ent_Target)) {
				Ent->targetvector = Ent->target.angle;

				if (Ent->targetvector == 0) {
					if (Ent->position.x > Ent->target.x) {
						Ent->targetvector = Ent->velocity*-1;
					} else { Ent->targetvector = Ent->velocity; }
				}

				
				if (Ent->target.distance > 200) {
					if (Ent->velocity < Ent->maxforcespeed) {
						Forward();
					}
				} else { Stop(); }
				

				if (EntMemory->state_mm != states::AI_MM_ATTACK) {
					AttackStart();
				}
			}
		} else {
			//THIS DEPENDS ON HOSTILITY MOOD

			//this removes target, so it will move to next targ/job
			EntMemory->targetList.pop_front();
		}
	} else {
		AttackStop();
		TryNextJob();
	}
}
void ai_agent::Evade() {
	
}



void ai_agent::DefendArea() {
	phys_obj* targ2 = GetClosestVisibleSprite((((phys_obj* )Ent)),true);

	if (targ2->ID > -2) {
		
	}
}
void ai_agent::Pathfind() {

	//Once multithreading is coded, this will listen for completion and basicly maintain previous job till then
	if (!EntMemory->pathing) {
		EntMemory->jobData->nextJob = NewJob(EntMemory->jobData,states::AI_JOB_STATE_WAYPOINTS);
		StartNextJob();
	}
}

/* Muscle Memory functions */
void ai_agent::MoveTo() {

}
void ai_agent::Chase() {

}
void ai_agent::Attack() {
	AttackStart();
}
void ai_agent::Flee() {

}
void ai_agent::Dodge() {

}
/* Action Enabler functions */
void ai_agent::Forward() {
	SetAnimState(states::ANIM_STATE_MOVE);
	Ent->acceleration = Ent->goforce;
}

void ai_agent::Stop() {
	SetAnimState(states::ANIM_STATE_IDLE);
	Ent->acceleration = Ent->stopforce * -1;
}

void ai_agent::AttackStart() {
	UpdateTarget();
	this->SetMMState(states::AI_MM_ATTACK);
	this->SetAnimState(states::ANIM_STATE_ATTACK);
	//this->Ent->Shoot(Ent->vector);
}
void ai_agent::AttackStop() {
	this->SetMMState(states::AI_MM_IDLE);
	this->SetAnimState(states::ANIM_STATE_IDLE);
}

