#ifndef AI_NAV
#define AI_NAV
#include "../thread.h"

class ai_nav;
//class sdl_thread;



class ai_nav_thread : public sdl_thread {
public:
	
	ai_nav_thread();
	~ai_nav_thread(){};

	void Map_Pathfind();
	void WP_Pathfind();

	/* Pathfinding functions */
	bool AddNode(long X, long Y);
	bool AddNode(nodeInfo* WP);
	void RunPathfindThread();
	void FinishPathfind();
	//void SetupPathfind(long X, long Y);
	//void SetupPathfind(phys_obj* targ);
	void GetBestPath(nodeInfo* node, bool reachedGoal);

	void OptimizePath();
	void ConnectPath(bool startOfPath);
	bool BuildWPPath(nodeInfo* startWP, nodeInfo* endWP);

	void AddWPNode(nodeInfo* WP);
	void AddWPNode(nodeInfo* WP, nodeInfo* connWP);

	nodeInfo* ClosestNode();
	nodeInfo* GetClosestWP(long tileX, long tileY);
	nodeInfo* GetClosestLOSWP(long tileX, long tileY); //will return closest non LOS WP if there are no LOS WPs

	
	void PrepPathfind();

	typedef std::vector<nodeInfo*> NIVec;

	void AddLOSNodes(nodeInfo* WP);
	void FixLOSNodes(NIVec* WPs);

	long ts;
	
	ai_nav*					ptrNav;
	Game_Engine*			ptrWorld;

	ai_agent*				AI;

	ent_agent*				Ent;
	sprite_memory*			EntMemory;

	std::vector<nodeInfo*>	path_Start;
	std::vector<nodeInfo*>	path_Mid;
	std::vector<nodeInfo*>	path_End;

	
	std::vector<nodeInfo*>	WPN;

	int test1;

	
	
	//int &test2;

	//Copy of the global waypoint node vector for locked use in thread
	std::vector<nodeInfo*>	wpNodesCopy;

	NIVec*	waypointNodes;// = new std::vector<nodeInfo*>;

	//(std::vector<nodeInfo*>)*	ptrWPNodes;

};

class ai_nav {
public:
	ai_nav(){};
	~ai_nav(){};

	//these 2 functions copied over to be changed into initializers of the threads
	void SetupPathfind(long X, long Y);
	void SetupPathfind(phys_obj* targ);

	void PreloadPathfind(navInfo* ni);

	//For preloading ahead of time to prevent stutters
	void PreloadNavs(long navsCount);


	void ReleaseNav(navInfo* ni);
	navInfo* GetNav();
	

	//hmmm
	void FixLOSNodes();

	//Created threads
	std::vector<ai_nav_thread*> NavThread;

	//Global waypoint node vector
	std::vector<nodeInfo*>	waypointNodes;

	//Navigation Data vector, size = alive units
	std::vector<navInfo*>	navDataArray;
	

	ai_agent*				AI;
	Game_Engine*			ptrWorld;

	long ts;

};


#endif