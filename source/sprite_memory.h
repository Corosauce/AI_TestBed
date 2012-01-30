#ifndef SPRITE_MEMORY
#define SPRITE_MEMORY

#include <vector>
#include "phys_math.h"

class ai_agent;


//Pathfinding nodes
class nodeInfo {
public:

	nodeInfo();
	nodeInfo(ai_agent* AI);
	~nodeInfo();

	long ID;
	ai_agent* ptrAI;
	bool ptrAIValid;


	long x;
	long y;

	
	bool exists;
	
	float distance;
	
	//these 2 used for pathfinding nodes
	nodeInfo* parent;
	bool hasParent;

	//this used for waypoint nodes
	std::vector<nodeInfo*> LOSNodes;

};

//Define for pointer to nodeInfo vectors
typedef std::vector<nodeInfo*> wpDef;

/*struct waypointNodeInfo {
	
	long x;
	long y;

	long ID;
	bool exists;
	
	float distance;

	std::vector<waypointNodeInfo*> LOSNodes;

};*/

//Composed Pathfind data
struct navInfo {

	//This is used to determine if an active AI is 
	//using this already allocated memory space
	long ID;
	long uID;

	bool claimed;
	long claimCount;

	bool pf_preloaded;

	bool inUse;
	bool queueRemove;
	
	std::vector<nodeInfo*>	nodeData;
	std::vector<nodeInfo*>	nodeOrderData;

	std::vector<std::vector<nodeInfo>>	occupyData;
	nodeInfo*				occupyData2;

	nodeInfo*				Parent;
	nodeInfo				Target;

	bool pathFound;

	//Compiled results
	std::vector<nodeInfo*> Path;
	std::vector<nodeInfo*> tempPath;

	long nodeCount;
	long curPathNode;

};

struct jobVarData {

	long SleepUntil;

	bool newTarget;
	coord targPos;


};

class jobInfo {
public:

	jobInfo(){ready = false; noNextJob = true;};

	jobInfo*	nextJob;
	jobInfo*	prevJob;

	bool		ready;
	bool		noNextJob; //bad(not null) pointer will cause this to be true
	long		jobGo;

	jobVarData	jobVars;

	long priority; // ?
	int state_job;

	void (ai_agent::*ptrJobAction)(); //muscle memory
	//void (ai_agent::*ptrNextAction)();

};

struct flockInfo {

	sprite* left;
	sprite* right;
	sprite* front;
	sprite* back;

	long target_distance;

};



class sprite_memory {
public:
	sprite_memory();
	~sprite_memory() {};

	bool hasTarget();//{return true;};

	//Pathfind data, main reason for caching right here
	navInfo*		navData;

	long team;

	int state; //mm state
	int state_pri1;
	
	int state_mm;  // pointer to state
	int state_job; // pointer to jobData->JobVars.state_job;

	

	int phys_state;

	bool collision;
	bool pathing;

	int dangerlevel;
	bool safe;

	phys_obj*	targetGuard;
	bool		hasTargetToGuard;

	//std::vector<sprite*>	targetList2;
	std::list<phys_obj*>	targetList;
	//bool hasTarget;
	//sprite*	ptrTarget;
	
	//void (ai_agent::*ptrWakeAction)();

	//void (ai_agent::*defaultJob)();
	int defaultJob;

	void (ai_agent::*ptrThought_Pri1)();
	void (ai_agent::*ptrThought_MM)();

	jobInfo*	jobData; // for level 2
	
	

	//I dont think i need this, since alive unit = has nav
	//bool		hasNav;

	bool		fullPath;

	flockInfo	flockData;

	
	

};

#endif