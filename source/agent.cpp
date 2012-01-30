#include "agent.h"

ent_agent::ent_agent() {
	this->initInfo.useInit = false;
	this->initInfo.state_job = 0;
	this->initInfo.state_pri1 = 0;
	this->initInfo.target_atkID = -2;
	this->initInfo.target_grdID = -2;
}

ent_agent::~ent_agent() {

}