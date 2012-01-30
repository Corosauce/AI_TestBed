#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

#include "states.h"

class Game_Engine;
class sprite;
class ent_agent;
class phys_obj;
class projectile;
class Tile;

struct coord;

class Physics {
public:

	Game_Engine*	ptrWorld;
	//ent_agent*		Entities;
	std::vector<ent_agent *> Entities;

	phys_obj*		Ent;
	phys_obj*		TempEnt;
	sprite*			ptrSprite;
	projectile*		ptrPrj;

	std::vector<projectile*>::iterator p;
	
	//phys_obj*		Phys;


	void Init(Game_Engine* ptrEngine);


	phys_obj*		getSprite(long ID);

	//void doPhysics();

	void unitPhysics();
	void doUnitPhysics();
	void prjPhysics();
	void doPrjPhysics();

	void UpdateCollideMap();
	bool CheckCircularCollide(phys_obj* physPtr, phys_obj* physPtr2);

	phys_obj* GetClosestSpriteID(phys_obj* physPtr, bool findEnemy);
	phys_obj* GetClosestVisibleSprite(phys_obj* physPtr, bool findEnemy);
	bool LOSCheck(phys_obj* physPtr1, phys_obj* physPtr2);

	long getRelX(long x, float newVec, long pixels);
	long getRelY(long y, float newVec, long pixels);
	coord CalcTrajectory(coord curPos, float vector, long pixels);

	long getFutureTile(phys_obj* Ent, float testVec, long futureTicks);
	long getRelativeTile(phys_obj *Ent, float newVec, long pixels);
	//Tile* getRelativeTile(phys_obj *Ent, float newVec, long pixels);
	void addToTileMap(Tile* map, long tileNum, long sprID);
	void addToTileMap(Tile* map, long x, long y, long sprID);

	long tiletoX(long tilenum);
	long tiletoY(long tilenum);

	//3 functions return if tile is occupied by any but (Ent*)
	bool occupyMapCheck(long tilenum);
	bool occupyUnitCheck(long tilenum);
	bool occupyAllCheck(long tilenum);

	//false is out of bounds
	bool boundCheck(long tilenum);
	long boundFix(long tilenum);

	float physDrag;

	float physTimeScale;
	float physTimeScalePrev;
	float physSpeedScale;

	float vDiff;

	long  ts;

	
};


#endif