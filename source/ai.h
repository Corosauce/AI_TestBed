#ifndef AI_H
#define AI_H

#include "agent.h"
#include "sprite_memory.h"

#include "ai\navigation.h"

#include "control.h"
#include "player.h"

#include "states.h"
#include "physics.h"
//#include "thread.h"


class ai_agent {
public:

	ai_agent();
	~ai_agent();

	void Init(Game_Engine* worldpointer);

	void AddAgent();
	void KillAgent(long agentID);
	void KillAgent();
	void RemoveAgent(long agentID);

	//jobInfo* NewJob(jobInfo* ptrCurJob);
	//jobInfo* NewJob(jobInfo *ptrCurJob, void (ai_agent::*ptrNewAction)());jobInfo* NewJob(jobInfo *ptrCurJob, void (ai_agent::*ptrNewAction)());
	jobInfo* NewJob(jobInfo *ptrCurJob, long newJobState);
	jobInfo* NewJob(jobInfo *ptrCurJob, long newJobState, long waitTime);
	void StartNextJob();
	void TryNextJob();

	int GetState(const int state);

	//dont use?
	bool IsEnemy(phys_obj* targ);

	//
	bool IsEnemy(long targ);

	void SetPri1State(int state);

	//this one cant exist, because you must change through job changes
	//void SetJobState(int state);

	void SetMMState(int state);
	void SetAnimState(int state);

	void Process(); //added for threading and TPS free ai code
	void Think(); //iterates through agents
	
	void CheckCollision();

	/* VIRTUAL THOUGHT FUNCTION POINTERS START */

	//can be used for any below
	void Idle(){};
	
	//Pri1
	void Listen();
	void Protect();
	void Kamikaze();
	void Dazed();
	
	//Job Thought
	void Wait();
	void Follow();
	void Waypoints();
	void Patrol();
	void Hunt();
	void Fight();
	void Evade();
	void DefendArea();

	void Pathfind(); //make this do something action wise quick while it calculates nodes on the side
	//void WP_Pathfind();
	
	//MM
	void MoveTo();
	void Chase();
	void Attack();
	void Flee();
	void Dodge();
	/* VIRTUAL THOUGHT FUNCTION POINTERS END */

	//Action functions
	void Forward();
	void Stop();
	void AttackStart();
	void AttackStop();
	
	//fun temp functions
	void StartAI();

	/* target related */

	//these 3 use SetTarget - or not!
	void SetGlobalTarget(long targID);

	void GotoPoint(int x, int y);
	void Wander();

	void SetGuardTarget(long targID);
	void SetGuardTarget(phys_obj* targ);

	void UnSetGuardTarget();

	void SetPri1Target(long targID);
	void SetPri1Target(phys_obj* targ);
	void AddTarget(long targID);
	void AddTarget(phys_obj* targ);

	void UpdateTarget();
	
	long getRandAliveUnitID();
	long getRandEnemyID();

	/* */

	bool LOSCheck(nodeInfo* node1, nodeInfo* node2);

	//Must supply tile coordinates
	bool LOSCheck(long x1, long y1, long x2, long y2);

	bool LOSCheck(phys_obj* physPtr1, phys_obj* physPtr2);
	
	phys_obj* GetClosestSpriteID(phys_obj* physPtr, bool findEnemy);
	phys_obj* GetClosestVisibleSprite(phys_obj* physPtr, bool findEnemy);
	phys_obj* GetClosestVisibleSprite2(phys_obj* physPtr, bool findEnemy);
	phys_obj* getSprite(long ID);
	
	/* */

	
	int state; //should be in sprite class?

	int tCount;
	int unitNum;

	long JobChanges;

	long ts;

	Game_Engine*			ptrWorld;
	//ent_agent*				Entities;
	std::vector<ent_agent *> Entities;
	//std::vector<ent_agent *>::iterator itE;
	ent_agent*				Ent;
	phys_obj*				TempEnt;
	phys_obj*				Ent_Target;
	player*					Player;

	//sprite_memory*			brainMemory; //all units
	std::vector<sprite_memory*> brainMemory;
	std::vector<sprite_memory*>::iterator itB;
	sprite_memory*			tempBrain;
	sprite_memory*			EntMemory; //current unit

	
	std::vector<void (ai_agent::*)()> stateToFcn_Pri1;
	std::vector<void (ai_agent::*)()> stateToFcn_Job;
	std::vector<void (ai_agent::*)()> stateToFcn_MM;

	std::vector<long> stateToDefaultJob_Pri1;
	std::vector<long> stateToPriority_Job;


	ai_nav* Nav;
	
	
	

	//ptrAction getFcnPtr(std::vector<ptrAction*>, int state);

	std::stringstream sstr;
	std::string str1;

	long nodeCount;

	long FullPathCount;
	long NoLOSEntCount;
	long NoLOSTargCount;


	long pfCount_Map;
	long pfCount_WP;

	long threadCount;

	bool thread_active;


	long prc_ai_s;
	long prc_ai_e;
	long prc_ai_int;

	long new_ai_count;
	long ai_ms_count;

	

	

	// 0 arg ptr
	//void (ai_agent::*ptrAction)();

	// 1 arg ptr
	//void (ai_agent::*ptrAction2)(int);

};




#endif