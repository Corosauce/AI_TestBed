#include "../ai.h"
#include "../thread.h"
#include "math.h"

#include <stdio.h>
#include <string>
#include <windows.h>

#include <cstdlib> 
#include <iostream>

#include "../phys_math.h"
#include "../physics.h"

#include "../timer.h"

// SDL_THREAD FUNCTIONS
int sdl_thread::mythread(void *data) {

	return 0;

}

int sdl_thread::_run(void* data) {

	ai_nav_thread* tptr = (ai_nav_thread*)data;
	//(ai_nav_thread*)data->Ent;
	tptr->AI;
	while(tptr->thread_active) {
		
		//Sleep(500);
		//SDL_Delay(1000);
		tptr->EntMemory->navData->inUse = true;
		if (tptr->AI->threadCount < 15) {
			tptr->AI->threadCount++;
			tptr->RunPathfindThread();
			tptr->thread_active = false;
			tptr->AI->threadCount--;
		} else {
			SDL_Delay(1);
		}
		

	}

	return 0;
}

bool sdl_thread::startThread(ai_nav_thread* Nav) {

	this->thread_active = false;

	this->NavPtr = Nav;
	
	this->thread_active = true;
	this->thread = SDL_CreateThread( _thread_runner, Nav );
	return true;
}

int _thread_runner(void* data)  {
    return reinterpret_cast<sdl_thread*>(data)->_run(data);
}

// AI_NAV_THREAD FUNCTIONS

void ai_nav_thread::PrepPathfind() {
	/*for (long i = 0; i < this->EntMemory->navData->nodeData.size(); i++) {
		delete this->EntMemory->navData->nodeData.at(i);
	}*/
	long count = 0;
	this->EntMemory->navData->nodeData.clear();
	this->EntMemory->navData->nodeOrderData.clear();
	//delete this->EntMemory->navData->Parent;
	
	for (long i = 0; i < this->ptrWorld->Map->WorldInfo.total_tiles; i++) {

		EntMemory->navData->occupyData2[i].exists = false;
		count++;
		if (count > 10) {
			count = 0;
			SDL_Delay(1);
		}

	}

	/*for (int i = 0; i < this->ptrWorld->Map->WorldInfo.tiles_count_x; i++) {
		for (int j = 0; j < this->ptrWorld->Map->WorldInfo.tiles_count_y; j++) {
			EntMemory->navData->occupyData.at(i).at(j).exists = false;
		}
		
	}*/
	SDL_Delay(1);
	EntMemory->navData->pathFound = false;

}

ai_nav_thread::ai_nav_thread() {
	//this->test2 = test;
	//this->waypointNodes = new std::vector<nodeInfo*>;
}

void ai_nav_thread::RunPathfindThread() {

	ptrWorld->ai_time->start();

	EntMemory->fullPath = false;
	
	long X = EntMemory->navData->Target.x*ts;
	long Y = EntMemory->navData->Target.y*ts;

	bool WPLOS_Ent = false;
	bool WPLOS_Targ = false;

	long distTarg_clWP = 0;
	long distTarg_Ent = 0;

	if (waypointNodes->size() < 2) {
		EntMemory->fullPath = true;
	}
	this->path_Start.clear();
	this->path_Mid.clear();
	this->path_End.clear();


	if (!EntMemory->navData->pf_preloaded) {
		AI->Nav->PreloadPathfind(EntMemory->navData);
	}

	//TEMP OVERRIDE!!!
	EntMemory->fullPath = true;

	nodeInfo* clWP_Ent;
	nodeInfo* clWP_targ;

	//if there are for sure waypoints of use
	if (!EntMemory->fullPath) {
		clWP_targ = GetClosestLOSWP((int)X/ts,(int)Y/ts);
		
		distTarg_clWP = getDistance(clWP_targ->x*ts,clWP_targ->y*ts,X,Y);
		distTarg_Ent = getDistance(Ent->position.x,Ent->position.y,X,Y);

		clWP_Ent = GetClosestLOSWP((int)Ent->position.x/ts,(int)Ent->position.y/ts);

		if (distTarg_Ent < distTarg_clWP) {
			//ie: if the enemy is right around the corner, just do a quick pathfind, but still add to waypoints
			EntMemory->fullPath = true;
		} else {
			
			//small note here, with this code, a node can be closer, but not LOS, so a very far node in a big opening
			//could have LOS but not be closest, we should make a function for accounting for LOS when chosing closest

			WPLOS_Ent = AI->LOSCheck((int)Ent->position.x/ts,(int)Ent->position.y/ts,clWP_Ent->x,clWP_Ent->y);

			WPLOS_Targ = AI->LOSCheck((int)X/ts,(int)Y/ts,clWP_targ->x,clWP_targ->y);

		}
	}
	
	/*bool derp;
	while (true) {
		derp = true;
		//SDL_Delay(1);
	}*/

	


	
	//SDL_Delay(2000);
	if (EntMemory->fullPath) {
		AI->FullPathCount++;
		PrepPathfind();
		
		nodeInfo* temp = new nodeInfo;
		temp->ID = -2;
		temp->x = (int)Ent->position.x/ts;
		temp->y = (int)Ent->position.y/ts;
		this->EntMemory->navData->nodeData.push_back(temp);
		this->EntMemory->navData->nodeOrderData.push_back(temp);

		//moved to pre thread activation
		//this->EntMemory->navData->Target.x = (int)X/ts;
		//this->EntMemory->navData->Target.y = (int)Y/ts;
		//SDL_Delay(1000);
		AI->pfCount_Map++;
		this->Map_Pathfind();
		
		//if (waypointNodes.size() > 1) {
			//ConnectPath(true);
			OptimizePath();
		//}
	} else {
		if (!WPLOS_Ent) {
			AI->NoLOSEntCount++;
			
			PrepPathfind();

			nodeInfo* temp = new nodeInfo;
			temp->ID = -2;
			temp->x = (int)Ent->position.x/ts;
			temp->y = (int)Ent->position.y/ts;
			long temp1 = abs((temp->x-clWP_Ent->x)^2 + (temp->y-clWP_Ent->y)^2);
			temp->distance = sqrt((temp1)*1.0f);
			//AddLOSNodes(temp);
			this->EntMemory->navData->nodeData.push_back(temp);
			this->EntMemory->navData->nodeOrderData.push_back(temp);

			//moved to pre thread activation
			//this->EntMemory->navData->Target.x = clWP_Ent->x;
			//this->EntMemory->navData->Target.y = clWP_Ent->y;

			AI->pfCount_Map++;
			this->Map_Pathfind();

			ConnectPath(true);
			this->path_Start = EntMemory->navData->Path;
			//connect now optimized path nodes from 0 to last minus the last(as it should be the clWP_Ent)
		} else {
			this->path_Start.push_back(clWP_Ent);
		}

		if (!WPLOS_Targ) {
			AI->NoLOSTargCount++;
			PrepPathfind();

			nodeInfo* temp = new nodeInfo;
			temp->ID = -2;
			temp->x = clWP_targ->x;
			temp->y = clWP_targ->y;
			long temp1 = abs((temp->x-(int)X/ts)^2 + (temp->y-(int)Y/ts)^2);
			temp->distance = sqrt((temp1)*1.0f);
			//AddLOSNodes(temp);
			this->EntMemory->navData->nodeData.push_back(temp);
			this->EntMemory->navData->nodeOrderData.push_back(temp);

			//moved to pre thread activation
			//this->EntMemory->navData->Target.x = (int)X/ts;
			//this->EntMemory->navData->Target.y = (int)Y/ts;

			AI->pfCount_Map++;
			this->Map_Pathfind();

			ConnectPath(false);
			this->path_End = EntMemory->navData->Path;

		} else {
			this->path_End.push_back(clWP_targ);
		}

		this->EntMemory->navData->Target.x = X/ts;
		this->EntMemory->navData->Target.y = Y/ts;

		if (BuildWPPath(clWP_Ent,clWP_targ)) {
			EntMemory->navData->Path.clear();

			for (int i = 0; i < path_Start.size(); i++) {
				EntMemory->navData->Path.push_back(path_Start.at(i));
			}
			for (int i = 0; i < path_Mid.size(); i++) {
				EntMemory->navData->Path.push_back(path_Mid.at(i));
			}
			for (int i = 0; i < path_End.size(); i++) {
				EntMemory->navData->Path.push_back(path_End.at(i));
			}

		} else {
			bool fail = true;
		}

	}

	if (procDebug) {
		if (!WPLOS_Ent) {
			this->ptrWorld->ptrPhysics->addToTileMap(this->ptrWorld->Map->wps_map,clWP_Ent->x,clWP_Ent->y,77);
		}

		if (!WPLOS_Targ) {
			this->ptrWorld->ptrPhysics->addToTileMap(this->ptrWorld->Map->wpe_map,clWP_targ->x,clWP_targ->y,99);
		}

		for (int i = 0; i < ptrWorld->Map->WorldInfo.total_tiles; i++) {

			this->ptrWorld->Map->wps_map[i].reset();
			this->ptrWorld->Map->path_map[i].reset();
			this->ptrWorld->Map->wpe_map[i].reset();
		
		}

		for (int i = 0; i < EntMemory->navData->Path.size(); i++) {
			this->ptrWorld->ptrPhysics->addToTileMap(ptrWorld->Map->path_map,EntMemory->navData->Path.at(i)->x,EntMemory->navData->Path.at(i)->y,i);
			
		}
	}

	int fsd = 0;

	//pathfinding finished, need to tell the entity, give it the data from the thread, BOOKMARK

	//SDL_Delay(1000);
	EntMemory->pathing = false;
	//EntMemory->jobData->nextJob = AI->NewJob(EntMemory->jobData,states::AI_JOB_STATE_WAYPOINTS);
	//StartNextJob();

	/*for (int i = 0; i < ptrWorld->Map->WorldInfo.total_tiles; i++) {

		this->ptrWorld->Map->waypoint_map[i].Occupied = false;
		this->ptrWorld->Map->waypoint_map[i].OccupyData.clear();
	
	}*/
	
	FinishPathfind();
	//this->EntMemory->jobData = NewJob(this->EntMemory->jobData,states::AI_JOB_STATE_PATHFIND);

	//SDL_Delay(500);

	ptrWorld->AI->ai_ms_count = ptrWorld->ai_time->get_ticks();

}

void ai_nav_thread::FinishPathfind() {

	EntMemory->navData->inUse = false;

	//if entity latched to it has died, declare navData open for re-use as this is the end of the line for the old entity
	if (EntMemory->navData->queueRemove) {
		this->EntMemory->navData->claimed = false;
	}
}

bool ai_nav_thread::BuildWPPath(nodeInfo* startWP, nodeInfo* endWP) {
	nodeInfo* curWP;
	nodeInfo* bestWP;
	nodeInfo* prevWP;
	float bestDist = 0;
	float curDist = 0;
	this->path_Mid.clear();

	curWP = startWP;
	prevWP = curWP;

	PrepPathfind();
	
	//TEMP DISABLED - reenabled
	FixLOSNodes(this->waypointNodes);

	nodeInfo* temp = new nodeInfo;
	temp->ID = -2;
	temp->hasParent = false;
	temp->x = startWP->x;
	temp->y = startWP->y;

	startWP->hasParent = false;
	endWP->hasParent = false;

	this->EntMemory->navData->nodeData.push_back(startWP);
	this->EntMemory->navData->nodeOrderData.push_back(startWP);

	this->EntMemory->navData->Target.x = endWP->x;
	this->EntMemory->navData->Target.y = endWP->y;

	AI->pfCount_WP++;
	this->WP_Pathfind();

	ConnectPath(true);
	this->path_Mid = EntMemory->navData->Path;

	
	return true;
	//prevWP = startWP;
	while (curWP->x != endWP->x && curWP->y != endWP->y) {
		if (curWP->LOSNodes.size() < 1) {
			break;
		} else if (curWP->LOSNodes.size() < 2) {
			//only one node
			curWP = curWP->LOSNodes.at(0);
			this->path_Mid.push_back(curWP);
		} else {
			bestWP = curWP->LOSNodes.at(0);
			bestDist = getDistance(bestWP->x,bestWP->y,endWP->x,endWP->y);
			//bestDist = getDistance(bestWP->x,bestWP->y,EntMemory->navData->Target.x,EntMemory->navData->Target.y);
			//distance comparison to target to select next node
			for (int i = 1; i < curWP->LOSNodes.size(); i++) {
				
				curDist = getDistance(curWP->LOSNodes.at(i)->x,curWP->LOSNodes.at(i)->y,endWP->x,endWP->y);
				//curDist = getDistance(bestWP->x,bestWP->y,EntMemory->navData->Target.x,EntMemory->navData->Target.y);
				if (bestDist > curDist) {
					
					bestWP = curWP->LOSNodes.at(i);
					
					bestDist = curDist;
				}

			}
			curWP = bestWP;
			this->path_Mid.push_back(curWP);

			if (bestWP->x == prevWP->x && bestWP->y == prevWP->y) {
				bool loop = true;
				return false;
			}
			prevWP = bestWP;
			
			
		}

		
	}
	return true;
}

void ai_nav_thread::ConnectPath(bool startOfPath) {
	EntMemory->navData->tempPath.clear();
	long curNode = 0, i = 0;
	if (startOfPath) {
		EntMemory->navData->tempPath.push_back(EntMemory->navData->Path.at(curNode));
	}
	//AddWPNode(EntMemory->navData->Path.at(curNode));
	for (i = 1; i < EntMemory->navData->Path.size(); i++) {
		if (!AI->LOSCheck(EntMemory->navData->Path.at(curNode),EntMemory->navData->Path.at(i))) {
			curNode = i-1;
			EntMemory->navData->tempPath.push_back(EntMemory->navData->Path.at(curNode));
			
			//AddWPNode(EntMemory->navData->Path.at(curNode),waypointNodes.at(waypointNodes.size()-1));
		}
	}
	if (!startOfPath) {
		EntMemory->navData->tempPath.push_back(EntMemory->navData->Path.at(i-1));
	}
	//AddWPNode(EntMemory->navData->Path.at(i-1),waypointNodes.at(waypointNodes.size()-1));
	EntMemory->navData->Path.clear();
	EntMemory->navData->Path = EntMemory->navData->tempPath;

	//TEMP DISABLED - on?
	FixLOSNodes(this->waypointNodes);
}

void ai_nav_thread::OptimizePath() {
	EntMemory->navData->tempPath.clear();
	long curNode = 0, i = 0;

	EntMemory->navData->tempPath.push_back(EntMemory->navData->Path.at(curNode));

	//AddWPNode(EntMemory->navData->Path.at(curNode));

	for (i = 1; i < EntMemory->navData->Path.size(); i++) {
		if (!AI->LOSCheck(EntMemory->navData->Path.at(curNode),EntMemory->navData->Path.at(i))) {
			curNode = i-1;
			EntMemory->navData->tempPath.push_back(EntMemory->navData->Path.at(curNode));
			
			//add new node to WPs, giving new node reference to last node (which is of course LOS)
			//AddWPNode(EntMemory->navData->Path.at(curNode),waypointNodes.at(waypointNodes.size()-1));

			//now give that last node a reference to this new node, for 2 way navigation by LOS
			//waypointNodes.at(waypointNodes.size()-2)->LOSNodes.push_back(waypointNodes.at(waypointNodes.size()-1));
		}
		SDL_Delay(1);
	}
	EntMemory->navData->tempPath.push_back(EntMemory->navData->Path.at(i-1));

	//AddWPNode(EntMemory->navData->Path.at(i-1),waypointNodes.at(waypointNodes.size()-1));
	//waypointNodes.at(waypointNodes.size()-2)->LOSNodes.push_back(waypointNodes.at(waypointNodes.size()-1));

	EntMemory->navData->Path.clear();
	EntMemory->navData->Path = EntMemory->navData->tempPath;

	FixLOSNodes(&EntMemory->navData->Path);
}

void ai_nav_thread::AddWPNode(nodeInfo* WP) {
	//make a new node, so its not pointing to node from some units memory
	nodeInfo* tn = new nodeInfo;
	tn->ID = this->waypointNodes->size();
	tn->exists = true;
	tn->x = WP->x;
	tn->y = WP->y;

	//AddLOSNodes(tn);

	//this->waypointNodes.push_back(tn);
	this->waypointNodes->push_back(tn);

}

void ai_nav_thread::AddWPNode(nodeInfo* WP, nodeInfo* connWP) {
	//make a new node, so its not pointing to node from some units memory
	nodeInfo* tn = new nodeInfo;
	tn->ID = this->waypointNodes->size();
	tn->exists = true;
	tn->x = WP->x;
	tn->y = WP->y;

	//tn->LOSNodes.push_back(connWP);
	AddLOSNodes(tn);

	this->waypointNodes->push_back(tn);

}


nodeInfo* ai_nav_thread::ClosestNode() {
	float tempDist, bestDist;
	nodeInfo* tempNode = new nodeInfo;

	tempNode = EntMemory->navData->nodeData.at(0);
	
	
	bestDist = sqrt(((abs(tempNode->x-this->EntMemory->navData->Target.x))^2 + (abs(tempNode->y-this->EntMemory->navData->Target.y))^2)*1.0f);
	tempDist = bestDist;
	
	for (long i = 1; i < EntMemory->navData->nodeData.size(); i++) {
		tempDist = sqrt( ((abs(EntMemory->navData->nodeData.at(i)->x-this->EntMemory->navData->Target.x))^2 + (abs(EntMemory->navData->nodeData.at(i)->y-this->EntMemory->navData->Target.y))^2 )*1.0f);
		if (tempDist < bestDist) {
			bestDist = tempDist;
			tempNode = EntMemory->navData->nodeData.at(i);
		}
		SDL_Delay(1);
	}

	return tempNode;
}

nodeInfo* ai_nav_thread::GetClosestWP(long tileX, long tileY) {
	float tempDist, bestDist;
	nodeInfo* tempNode;

	if (waypointNodes->size() > 0) {

		//tempNode = EntMemory->navData->nodeData.at(0);
		tempNode = waypointNodes->at(0);
		
		bestDist = getDistance(tempNode->x,tileX,tempNode->y,tileY);
		//bestDist = sqrt(((abs(tempNode->x-tileX))^2 + (abs(tempNode->y-tileY))^2)*1.0f);
		tempDist = bestDist;
		
		for (long i = 1; i < waypointNodes->size(); i++) {
			tempDist = getDistance(waypointNodes->at(i)->x,tileX,waypointNodes->at(i)->y,tileY);
			//tempDist = sqrt( ((abs(waypointNodes.at(i)->x-tileX))^2 + (abs(waypointNodes.at(i)->y-tileY))^2 )*1.0f);
			if (tempDist < bestDist) {
				bestDist = tempDist;
				tempNode = waypointNodes->at(i);
			}
		}

		return tempNode;
	} else {
		tempNode = new nodeInfo;
		tempNode->exists = false;
		return tempNode;
	}
}

nodeInfo* ai_nav_thread::GetClosestLOSWP(long tileX, long tileY) {
	float tempDist, bestDist, bestLOSDist = 999999;
	nodeInfo* bestLOSNode;
	nodeInfo* tempNode;

	if (waypointNodes->size() > 0) {

		//tempNode = EntMemory->navData->nodeData.at(0);
		tempNode = waypointNodes->at(0);
		
		bestDist = getDistance(tempNode->x,tileX,tempNode->y,tileY);
		//bestDist = sqrt( ((abs(tempNode->x-tileX))^2 + (abs(tempNode->y-tileY))^2 )*1.0f);
		tempDist = bestDist;

		

		if (AI->LOSCheck(tileX,tileY,tempNode->x,tempNode->y)) {
			bestLOSDist = bestDist;
			bestLOSNode = tempNode;
		}
		
		for (long i = 1; i < waypointNodes->size(); i++) {
			tempDist = getDistance(waypointNodes->at(i)->x,tileX,waypointNodes->at(i)->y,tileY);
			//tempDist = sqrt( ((abs(waypointNodes.at(i)->x-tileX))^2 + (abs(waypointNodes.at(i)->y-tileY))^2 )*1.0f);
			if (AI->LOSCheck(tileX,tileY,waypointNodes->at(i)->x,waypointNodes->at(i)->y)) {
				if (tempDist < bestLOSDist) {
					bestLOSDist = tempDist;
					bestLOSNode = waypointNodes->at(i);
				}
			}
			if (tempDist < bestDist) {
				bestDist = tempDist;
				tempNode = waypointNodes->at(i); 
			}
		}

		if (bestLOSDist != 999999) {
			return bestLOSNode;
		} else {
			return tempNode;
		}
	} else {
		tempNode = new nodeInfo;
		tempNode->exists = false;
		return tempNode;
	}
}

void ai_nav_thread::GetBestPath(nodeInfo *node, bool reachedGoal) {
	long pathdistance;
	nodeInfo* lastnode;

	this->EntMemory->navData->Path.clear();
	this->EntMemory->navData->tempPath.clear();

	lastnode = node;
	while (true) {
		EntMemory->navData->tempPath.push_back(lastnode);
		if (lastnode->ID == -2) {
			break;
		}
		if (lastnode->hasParent) {
			lastnode = lastnode->parent;
			if (lastnode->ID == -2) {
				break;
			}
		} else { break; }
		SDL_Delay(1);
	}

	for (int i = EntMemory->navData->tempPath.size()-1; i >= 0; i--) {
		EntMemory->navData->Path.push_back(EntMemory->navData->tempPath.at(i));
		SDL_Delay(1);
	}

	if (reachedGoal) {
		EntMemory->navData->pathFound = true;
		EntMemory->navData->Path.push_back(&EntMemory->navData->Target);
	} else { EntMemory->navData->pathFound = false; }

	if (EntMemory->navData->Path.size() > 0) {
		//for sure good to go
	} else {
		//path needs a retry? do this else where
	}

	if (EntMemory->navData->Path.size() > 2) {
		if (EntMemory->fullPath) {
			if (waypointNodes->size() < 1) {
				//OptimizePath();
			}
		}
	}

	if (procDebug) {

		for (int i = 0; i < EntMemory->navData->Path.size(); i++) {
			this->ptrWorld->ptrPhysics->addToTileMap(ptrWorld->Map->path_map,EntMemory->navData->Path.at(i)->x,EntMemory->navData->Path.at(i)->y,Ent->ID);
		
		}

		for (int i = 0; i < ptrWorld->Map->WorldInfo.total_tiles; i++) {

			this->ptrWorld->Map->waypoint_map[i].reset();
		
		}

		for (int i = 0; i < waypointNodes->size(); i++) {
			this->ptrWorld->ptrPhysics->addToTileMap(ptrWorld->Map->waypoint_map,waypointNodes->at(i)->x,waypointNodes->at(i)->y,waypointNodes->at(i)->LOSNodes.size());
			
		}
	}

	//EntMemory->jobData->nextJob = AI->NewJob(EntMemory->jobData,states::AI_JOB_STATE_WAYPOINTS);
	//AI->StartNextJob();
	
	EntMemory->navData->curPathNode = 0;
	//Waypoints();

}

bool ai_nav_thread::AddNode(nodeInfo* WP) {

	long X = WP->x;
	long Y = WP->y;

	

	//this->EntMemory->navData->occupyData.at(X).at(Y).exists = true;
	long temp = (Y*this->ptrWorld->Map->WorldInfo.tiles_count_x+X);
	this->EntMemory->navData->occupyData2[temp].exists = true;
	

	long temp1;

	//Points = Sqr((a - TargetXTile) ^ 2 + (b - TargetYTile) ^ 2)
	//this->EntMemory->navData->occupyData.at(X).at(Y).distance = ;

	nodeInfo* tempNode = new nodeInfo(AI);
	tempNode->ID = EntMemory->navData->nodeData.size();
	tempNode->parent = this->EntMemory->navData->Parent;
	tempNode->hasParent = true;
	tempNode->exists = true;
	tempNode->x = X;
	tempNode->y = Y;
	tempNode->LOSNodes = WP->LOSNodes;
	temp1 = abs((X-this->EntMemory->navData->Target.x)^2 + (Y-this->EntMemory->navData->Target.y)^2);
	tempNode->distance = sqrt((temp1)*1.0f);
	this->EntMemory->navData->nodeData.push_back(tempNode);
	//this->EntMemory->navData->nodeData.at(this->EntMemory->navData->nodeData.size());

	std::vector<nodeInfo*>::iterator nodeIT;
	for (long pos = 0; pos < this->EntMemory->navData->nodeOrderData.size(); pos++) {
		if (tempNode->distance < this->EntMemory->navData->nodeOrderData.at(pos)->distance) {
			nodeIT = this->EntMemory->navData->nodeOrderData.begin();
			nodeIT += pos;
			this->EntMemory->navData->nodeOrderData.insert(nodeIT,tempNode);
			return true;
		}
	}
	this->EntMemory->navData->nodeOrderData.push_back(tempNode);
	return false;

}

bool ai_nav_thread::AddNode(long X, long Y) {
	
	//this->EntMemory->navData->occupyData.at(X).at(Y).exists = true;

	long temp = (Y*this->ptrWorld->Map->WorldInfo.tiles_count_x+X);
	this->EntMemory->navData->occupyData2[temp].exists = true;

	long temp1;

	//Points = Sqr((a - TargetXTile) ^ 2 + (b - TargetYTile) ^ 2)
	//this->EntMemory->navData->occupyData.at(X).at(Y).distance = ;

	nodeInfo* tempNode = new nodeInfo(AI);
	tempNode->ID = EntMemory->navData->nodeData.size();
	tempNode->parent = this->EntMemory->navData->Parent;
	tempNode->hasParent = true;
	tempNode->exists = true;
	tempNode->x = X;
	tempNode->y = Y;
	temp1 = abs((X-this->EntMemory->navData->Target.x)^2 + (Y-this->EntMemory->navData->Target.y)^2);
	tempNode->distance = sqrt((temp1)*1.0f);
	this->EntMemory->navData->nodeData.push_back(tempNode);
	//this->EntMemory->navData->nodeData.at(this->EntMemory->navData->nodeData.size());

	std::vector<nodeInfo*>::iterator nodeIT;
	for (long pos = 0; pos < this->EntMemory->navData->nodeOrderData.size(); pos++) {
		if (tempNode->distance < this->EntMemory->navData->nodeOrderData.at(pos)->distance) {
			nodeIT = this->EntMemory->navData->nodeOrderData.begin();
			nodeIT += pos;
			this->EntMemory->navData->nodeOrderData.insert(nodeIT,tempNode);
			return true;
		}
	}
	this->EntMemory->navData->nodeOrderData.push_back(tempNode);
	return false;
}

void ai_nav_thread::WP_Pathfind() {

	bool stop = false;
	long ccount = 0;
	while ((stop == false) && (ccount < 10000)) {
		ccount++;
		if (this->EntMemory->navData->nodeOrderData.size() == 0) {

			//dead end - for waypoints this shouldnt happen
			stop = true;
			long hmm = 0;
			GetBestPath(ClosestNode(),false);

		} else {
			this->EntMemory->navData->Parent = this->EntMemory->navData->nodeOrderData.at(0);
			
			std::vector<nodeInfo*>::iterator nodeIT;
			nodeIT = this->EntMemory->navData->nodeOrderData.begin();
			this->EntMemory->navData->nodeOrderData.erase(nodeIT);
		}

		long tileNum = 0;
		for (long i = 0; i < this->EntMemory->navData->Parent->LOSNodes.size(); i++) {
			if (this->EntMemory->navData->Target.x == this->EntMemory->navData->Parent->LOSNodes.at(i)->x && this->EntMemory->navData->Target.y == this->EntMemory->navData->Parent->LOSNodes.at(i)->y) {
				//AddNode(this->EntMemory->navData->Parent->LOSNodes.at(i));
				GetBestPath(EntMemory->navData->Parent,true);
				stop = true;
				return;
			} else {
				long temp = (this->EntMemory->navData->Parent->LOSNodes.at(i)->y*this->ptrWorld->Map->WorldInfo.tiles_count_x+this->EntMemory->navData->Parent->LOSNodes.at(i)->x);
				if (!EntMemory->navData->occupyData2[temp].exists) {
					//AddNode(this->EntMemory->navData->Parent->LOSNodes.at(i)->x,this->EntMemory->navData->Parent->LOSNodes.at(i)->y);
					AddNode(this->EntMemory->navData->Parent->LOSNodes.at(i));
				}
			}
		}
		
	}
}

void ai_nav_thread::Map_Pathfind() {
	bool stop = false;
	long ccount = 0;
	/*while ((ccount < 500000000)) {
		ccount++;
		
	}*/
	ccount = 0;
	while ((stop == false) && (ccount < 5000)) {
		ccount++;
		
		if (ccount > 0) {
			ccount = 0;
			SDL_Delay(1);
		}
			
		if (this->EntMemory->navData->nodeOrderData.size() == 0) {

			//dead end
			stop = true;
			long hmm = 0;
			GetBestPath(ClosestNode(),false);

		} else {
			this->EntMemory->navData->Parent = this->EntMemory->navData->nodeOrderData.at(0);
			
			std::vector<nodeInfo*>::iterator nodeIT;
			nodeIT = this->EntMemory->navData->nodeOrderData.begin();
			this->EntMemory->navData->nodeOrderData.erase(nodeIT);
		}

		long tileNum = 0;
		for (long curX = this->EntMemory->navData->Parent->x-1;curX <= this->EntMemory->navData->Parent->x+1; curX++) {
			for (long curY = this->EntMemory->navData->Parent->y-1;curY <= this->EntMemory->navData->Parent->y+1; curY++) {
				tileNum = (curY*this->ptrWorld->Map->WorldInfo.tiles_count_x)+curX;
				if (!((curX == this->EntMemory->navData->Parent->x-1) && (curY == this->EntMemory->navData->Parent->y-1))) {
					if (!((curX == this->EntMemory->navData->Parent->x+1) && (curY == this->EntMemory->navData->Parent->y-1))) {
						if (!((curX == this->EntMemory->navData->Parent->x-1) && (curY == this->EntMemory->navData->Parent->y+1))) {
							if (!((curX == this->EntMemory->navData->Parent->x+1) && (curY == this->EntMemory->navData->Parent->y+1))) {
								if ((curX > 0) && (curY > 0) && (curX < this->ptrWorld->Map->WorldInfo.tiles_count_x) && (curY < this->ptrWorld->Map->WorldInfo.tiles_count_y)) {

									if (curX == this->EntMemory->navData->Target.x && curY == this->EntMemory->navData->Target.y) {
										//we hit target
										int lol = 0;
										GetBestPath(EntMemory->navData->Parent,true);
										stop = true;
										return;
									}
									long temp = (curY*this->ptrWorld->Map->WorldInfo.tiles_count_x+curX);
									if (!EntMemory->navData->occupyData2[temp].exists) {
										//if ((!this->ptrWorld->Map->tile_map[tileNum].Occupied) || (this->ptrWorld->Map->tile_map[tileNum].OccupyData.at(0) == Ent->ID)) {
											BLKSTR* blockTemp = this->ptrWorld->Map->MapSource->MapGetBlock(curX,curY);
											if (!blockTemp->tl) {
												AddNode(curX,curY);
											}
											//add a node
											
										//}
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

// AI_NAV FUNCTIONS

void ai_nav::SetupPathfind(phys_obj* targ) {
	if (targ->ID > -2) {
		SetupPathfind(targ->position.x,targ->position.y);
	}
}

void ai_nav::SetupPathfind(long X, long Y) {

	if (AI->LOSCheck(AI->Ent->position.x/ts,AI->Ent->position.y/ts,X/ts,Y/ts)) {
		bool weee = true;
		//dont pathfind, move to target, this will change once a more complex job manager is coded
		AI->EntMemory->jobData->nextJob = AI->NewJob(AI->EntMemory->jobData,states::AI_JOB_STATE_WAYPOINTS);
		AI->StartNextJob();
		
	} else if (!AI->EntMemory->pathing) {

		
		
		AI->EntMemory->jobData->nextJob = AI->NewJob(AI->EntMemory->jobData,states::AI_JOB_STATE_PATHFIND);
		AI->EntMemory->pathing = true;

		ai_nav_thread* tptr = new ai_nav_thread();
		tptr->Ent = AI->Ent;
		tptr->EntMemory = AI->EntMemory;
		//AI->EntMemory = AI->brainMemory[1];

		//this needs changing for waypoints cloning
		tptr->waypointNodes = &this->waypointNodes;


		tptr->AI = this->AI;
		tptr->ptrWorld = this->ptrWorld;
		tptr->ts = ts;

		tptr->EntMemory->navData->Target.x = (int)X/ts;
		tptr->EntMemory->navData->Target.y = (int)Y/ts;

		this->NavThread.push_back(tptr);
		
		//AI->threadCount = NavThread.size();

		
		//Threaded
		tptr->startThread(tptr);
		
		//temp
		//AI->EntMemory->pathing = false;
		
		//Non-Threaded
		//tptr->RunPathfindThread();


	}
}

void ai_nav::PreloadPathfind(navInfo* ni) {

	ni->occupyData2 = new nodeInfo[this->ptrWorld->Map->WorldInfo.total_tiles];

	for (long i = 0; i < this->ptrWorld->Map->WorldInfo.total_tiles; i++) {

		ni->occupyData2[i].exists = false;

	}

	/*
	//Function will cause small stall when called without delays
	for (int i = 0; i < this->ptrWorld->Map->WorldInfo.tiles_count_x; i++) {
		std::vector<nodeInfo> nodeVector;
		nodeVector.clear();
		for (int j = 0; j < this->ptrWorld->Map->WorldInfo.tiles_count_y; j++) {
			nodeInfo tempNode;
			tempNode.exists = false;
			nodeVector.push_back(tempNode);
			
		}
		//SDL_Delay(2);
		ni->occupyData.push_back(nodeVector);
		

	}*/
	ni->pf_preloaded = true;
}

void ai_nav::PreloadNavs(long navsCount) {

	this->ptrWorld->loader.loadAI.maxValue = navsCount;

	for (long i = 0; i < navsCount;i++) {
		navInfo* navTemp = new navInfo;
		navTemp->claimed = false;
		navTemp->claimCount = 0;
		navTemp->queueRemove = false;
		navTemp->inUse = false;

		navTemp->ID = navDataArray.size();
		navTemp->uID = rand();
		
		PreloadPathfind(navTemp);
		this->ptrWorld->loader.Update("ai",i);

		navDataArray.push_back(navTemp);
	}
}

navInfo* ai_nav::GetNav() {
	long i;
	long navSize = navDataArray.size();
	for (i = 0; i < navSize; i++) {
		if (!navDataArray.at(i)->claimed) {
			if (!navDataArray.at(i)->inUse) {
				navDataArray.at(i)->claimed = true;
				navDataArray.at(i)->claimCount++;
				if (navDataArray.at(i)->claimCount > 1) {
					bool fucked = true;
				}

				navDataArray.at(i)->queueRemove = false;
				return navDataArray.at(i);
			}
		}

	}

	//Make new if hit end of vector
	//if (i == navSize) {
		navInfo* navTemp = new navInfo;
		navTemp->queueRemove = false;
		navTemp->claimed = true;
		navTemp->inUse = false;
		navTemp->claimCount = 1;

		//Remove this line to prevent small stall upon new unit creation (if the buffer is full)
		//Which will cause small stall upon pathfind request instead
		PreloadPathfind(navTemp);

		navDataArray.push_back(navTemp);


		
		return navTemp;
	//}
}

void ai_nav::ReleaseNav(navInfo* ni) {




	if (!ni->inUse) {

		

		ni->claimed = false;
		ni->claimCount--;


	} else {
		ni->queueRemove = true;
	}
}

void ai_nav_thread::AddLOSNodes(nodeInfo* WP) {
	WP->LOSNodes.clear();
	for (int i = 0; i < waypointNodes->size(); i++) {
		if (AI->LOSCheck(WP,waypointNodes->at(i))) {
			if (WP->x != waypointNodes->at(i)->x && WP->y != waypointNodes->at(i)->y) {
				WP->LOSNodes.push_back(waypointNodes->at(i));
			}
		}
	}
}

void ai_nav_thread::FixLOSNodes(NIVec* WPs) {
	nodeInfo* WP;
	for (int i = 0; i < WPs->size(); i++) {
		WP = WPs->at(i);
		WP->LOSNodes.clear();
		for (int j = 0; j < WPs->size(); j++) {
			if (AI->LOSCheck(WP,WPs->at(j))) {
				//if (WP->x != waypointNodes.at(j)->x && WP->y != waypointNodes.at(j)->y) {
					WP->LOSNodes.push_back(WPs->at(j));
				//}
			}
		}
	}
}

void ai_nav::FixLOSNodes() {
	nodeInfo* WP;
	for (int i = 0; i < waypointNodes.size(); i++) {
		WP = waypointNodes.at(i);
		WP->LOSNodes.clear();
		for (int j = 0; j < waypointNodes.size(); j++) {
			if (AI->LOSCheck(WP,waypointNodes.at(j))) {
				//if (WP->x != waypointNodes.at(j)->x && WP->y != waypointNodes.at(j)->y) {
					WP->LOSNodes.push_back(waypointNodes.at(j));
				//}
			}
		}
	}
}