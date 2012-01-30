#ifndef AGENT_H
#define AGENT_H

#include "sprite.h"
#include "sprite_memory.h"

struct InitData {

	bool	useInit;

	//values that use the states:: namespace
	long	state_pri1;
	long	state_job;

	long	target_grdID;
	long	target_atkID;
};

class ent_agent : public sprite {
public:

	ent_agent();
	~ent_agent();
	sprite_memory*			ptrBrain;

	InitData				initInfo;

};

#endif