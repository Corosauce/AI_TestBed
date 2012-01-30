#include "ai.h"
#include "sprite_memory.h"



nodeInfo::nodeInfo() {
	ptrAIValid = false;
}

nodeInfo::nodeInfo(ai_agent* AI) {
	ptrAI = AI;
	ptrAIValid = true;
	//ptrAI->nodeCount++;
}

nodeInfo::~nodeInfo() {
	if (ptrAIValid) {
		//ptrAI->nodeCount--;
	}
}

sprite_memory::sprite_memory() {
	this->pathing = false;
	
}

bool sprite_memory::hasTarget() {
	if (this->targetList.size() > 0) {
		return true;
	} else { return false; }
}