#ifndef PHYS_OBJ_H
#define PHYS_OBJ_H

//#pragma message("TOP PHYS_OBJ")

struct angle_info {
	int frame;
	int max;
};

struct target_info {
	int ID;

	bool hasLOS;
	float angle;
	float distance;
	int x;
	int y;

};

struct pos_info {
	float x;
	float y;
};

class phys_obj
{

	public:

	phys_obj(){};
	phys_obj(long fakeID){ID = fakeID;};
	~phys_obj(){};
	long getState(){return this->phys_state;};
	

	int team;

	long ID;
	int typeID;

	float cruisespeed;
	float maxforcespeed;

	float stopforce;
	float goforce;
	float velocity;
	float acceleration;
	
	float vector;
	float targetvector;

	float turnspeed;
	float turndirection;

	int width;
	int height;

	pos_info				position;
	
	target_info				target;

	angle_info				angle;

	int	phys_state;
	int ai_mm_state;
	int ai_job_state;
	int health;

	long spawntime;
	long max_lifetime;

	int viewdistance;

	//recently moved
	ResourceObject			*ResID;
	Game_Engine*			ptrWorld;

	bool					fullyCreated;
	long					fullyCreatedSafety;

	bool					declareDead;
};


#endif