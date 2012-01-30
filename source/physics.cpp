#include "physics.h"
#include "control.h"
#include "sprite.h"

#include "timer.h"
#include "ai.h"

#include "agent.h"
#include "map.h"
#include "player.h"
#include "phys_obj.h"
#include "projectile.h"

#include "phys_math.h"


void Physics::Init(Game_Engine* ptrEngine) {

	this->ptrWorld = ptrEngine;
	this->Entities = ptrEngine->Units;

	this->ts = ptrWorld->Map->WorldInfo.tile_size;

	physDrag = 0.04f;
	physSpeedScale = 1.0f;

}

/* */
void Physics::unitPhysics() {

	UpdateCollideMap();

	if (this->ptrWorld->phys_process) {
		for (int i = 0;i < ptrWorld->Units.size();i++)
		{
			Ent = ((phys_obj* )Entities[i]);//[i];
			if (Ent->phys_state != states::PHYS_STATE_DEAD && Ent->fullyCreated) {
				doUnitPhysics();
			}
		}
	}
	Ent = ((phys_obj* )ptrWorld->Player);
	doUnitPhysics();
}

void Physics::prjPhysics() {
	if (this->ptrWorld->phys_process) {
		for (int i = 0; i < ptrWorld->Projectiles.size();i++) {

			//Phys = ((phys_obj*)ptrWorld->Projectiles.at(i));
			ptrPrj = ((projectile*)ptrWorld->Projectiles.at(i));
			Ent = ((phys_obj*)ptrWorld->Projectiles.at(i));

			if (this->ptrWorld->runTime > ptrPrj->spawntime+ptrPrj->max_lifetime) {
				p = ptrWorld->Projectiles.begin();
				p+= i;
				
				ptrWorld->Projectiles.erase(p);
				delete ptrPrj;
			} else {
				
				doPrjPhysics();
			}

			if (ptrPrj->phys_state == states::PHYS_STATE_COLLIDE_UNIT || ptrPrj->phys_state == states::PHYS_STATE_COLLIDE_MAP) {
				p = ptrWorld->Projectiles.begin();
				p+= i;
				ptrWorld->Projectiles.erase(p);
				delete ptrPrj;
				
			}
		}
	}

}

void Physics::doPrjPhysics() {
	
	//Fix vector
	if (Ent->vector < 0) { Ent->vector = (atan(1.0f)*8) + Ent->vector; }
	if (Ent->vector > (atan(1.0f)*8)) { Ent->vector -= (atan(1.0f)*8); }

	//Apply drag
	Ent->velocity -= physDrag;
	
	//Add accelleration
	Ent->velocity += Ent->acceleration;
	
	if (Ent->velocity < 0) { Ent->velocity = 0; }
	if (Ent->velocity > Ent->maxforcespeed) {
		Ent->velocity = Ent->maxforcespeed;
	}

	//Apply the velocity to position
	Ent->position.x += Ent->velocity * sin(Ent->vector) * physTimeScale;
	Ent->position.y -= Ent->velocity * cos(Ent->vector) * physTimeScale;

	//reset for evaluation below
	Ent->phys_state = states::PHYS_STATE_NORMAL;

	//collision stuff below here, to be moved

	if (Ent->position.x < 0) {
		Ent->position.x = 0;
		Ent->phys_state = states::PHYS_STATE_COLLIDE_MAP;
	}
	if (Ent->position.y < 0) {
		Ent->position.y = 0;
		Ent->phys_state = states::PHYS_STATE_COLLIDE_MAP;
	}
	if (Ent->position.x > ptrWorld->Map->WorldInfo.width-Ent->width) {
		Ent->position.x = ptrWorld->Map->WorldInfo.width-Ent->width;
		Ent->phys_state = states::PHYS_STATE_COLLIDE_MAP;
	}
	if (Ent->position.y > ptrWorld->Map->WorldInfo.height-Ent->height) {
		Ent->position.y = ptrWorld->Map->WorldInfo.height-Ent->height;
		Ent->phys_state = states::PHYS_STATE_COLLIDE_MAP;
	}

	float xFuture = Ent->position.x + Ent->velocity * sin(Ent->vector) * (physTimeScale);
	float yFuture = Ent->position.y + Ent->velocity * cos(Ent->vector) * (physTimeScale);

	short tempp = ptrWorld->Map->MapSource->MapGetTile((int)Ent->position.x/32,(int)Ent->position.y/32);
	if (tempp < 0) {
		tempp = 0;
	}
	//BLKSTR* blockTemp = this->ptrWorld->Map->Map->MapGetBlock((int)ptrWorld->Player->position.x/32,(int)ptrWorld->Player->position.y/32);
	BLKSTR* blockTemp = this->ptrWorld->Map->MapSource->MapGetBlock((int)Ent->position.x/32,(int)Ent->position.y/32);
	/*if (this->ptrWorld->Map->tile_map[tempp].Occupied) {
		Ent->position.x -= Ent->velocity * sin(Ent->vector);
		Ent->position.y += Ent->velocity * cos(Ent->vector);
	}*/

	//currently all blocks have full collision, so only need to check 1 corner
	if (blockTemp->tl) {// || this->ptrWorld->Map->tile_map[tempp].Occupied) {
		Ent->position.x -= Ent->velocity * sin(Ent->vector) * physTimeScale;
		Ent->position.y += Ent->velocity * cos(Ent->vector) * physTimeScale;
		Ent->phys_state = states::PHYS_STATE_COLLIDE_MAP;
	} else { }

	long tileNum = (((int)yFuture/32)*this->ptrWorld->Map->WorldInfo.tiles_count_x)+((int)xFuture/32);

	if (tileNum > 0 && tileNum < this->ptrWorld->Map->WorldInfo.total_tiles) {
		if (this->ptrWorld->Map->tile_map[tileNum].Occupied && Ent->typeID == -1) {
			/*s = ptrWorld->Units.begin();
			s+= this->ptrWorld->Map->tile_map[tileNum].OccupyData.at(0);
			ptrWorld->Units.erase(s);*/
			if (this->ptrWorld->Map->tile_map[tileNum].OccupyCount > 0) {
				for (int i = 0; i < this->ptrWorld->Map->tile_map[tileNum].OccupyCount; i++) {
					phys_obj* temp = ((phys_obj* )getSprite(this->ptrWorld->Map->tile_map[tileNum].OccupyData[i]));
					if (CheckCircularCollide(((phys_obj* )Ent),temp) && (((projectile* )Ent)->OwnerPtr->ID != temp->ID) && (((projectile* )Ent)->OwnerPtr->team != temp->team)) {
						int temp2 = this->ptrWorld->Map->tile_map[tileNum].OccupyData[i];
						ptrWorld->HurtUnit(temp2,ptrPrj->dmg_amount);
						this->ptrWorld->Map->tile_map[tileNum].Occupied = false;
						Ent->phys_state = states::PHYS_STATE_COLLIDE_UNIT;
					}
				}
				
			}
		}
	}
}

void Physics::doUnitPhysics() {

	//Fix targetvector       need?
	Ent->targetvector = Vectorize(Ent->targetvector,0.0f);
	//if (Ent->targetvector < 0.0f) { Ent->targetvector = (atan(1.0f)*8) + Ent->targetvector; }
	//if (Ent->targetvector > (atan(1.0f)*8)) { Ent->targetvector -= (atan(1.0f)*8); }

	//vector from target vector calc here
	if (Ent->targetvector != Ent->vector) {
		if (abs(Ent->targetvector - Ent->vector) < atan(1.0f)*4) {
			Ent->turndirection = Ent->turnspeed;
		} else {
			Ent->turndirection = Ent->turnspeed * -1.0f;
		}

		if ((Ent->targetvector - Ent->vector) < 0) {
			Ent->turndirection = Ent->turndirection * -1.0f;
		}

		if (abs(Ent->targetvector - Ent->vector) < (Ent->turnspeed * physTimeScale)) {
			Ent->vector = Ent->targetvector;
		} else {
			Ent->vector = Vectorize(Ent->vector,Ent->turndirection * physTimeScale);
		}
	}
	//Ent->vector += Ent->turndirection * physTimeScale;

	//Fix vector

	//Apply drag
	Ent->velocity -= physDrag;
	
	//Add accelleration
	Ent->velocity += Ent->acceleration;
	
	if (Ent->velocity < 0) { Ent->velocity = 0; }
	if (Ent->velocity > Ent->maxforcespeed) {
		Ent->velocity = Ent->maxforcespeed;
	}

	//reset for evaluation below
	Ent->phys_state = states::PHYS_STATE_NORMAL;

	//collision stuff below here, to be moved
	if (Ent->position.x < 0) {
		Ent->position.x = 0;
		Ent->phys_state = states::PHYS_STATE_COLLIDE_MAP;
	}
	if (Ent->position.y < 0) {
		Ent->position.y = 0;
		Ent->phys_state = states::PHYS_STATE_COLLIDE_MAP;
	}
	if (Ent->position.x > ptrWorld->Map->WorldInfo.width-Ent->width) {
		Ent->position.x = ptrWorld->Map->WorldInfo.width-Ent->width;
		Ent->phys_state = states::PHYS_STATE_COLLIDE_MAP;
	}
	if (Ent->position.y > ptrWorld->Map->WorldInfo.height-Ent->height) {
		Ent->position.y = ptrWorld->Map->WorldInfo.height-Ent->height;
		Ent->phys_state = states::PHYS_STATE_COLLIDE_MAP;
	}


	/*short tempp = ptrWorld->Map->MapSource->MapGetTile((int)Ent->position.x/32,(int)Ent->position.y/32);
	if (tempp < 0) {
		tempp = 0;
	}*/
	//BLKSTR* blockTemp = this->ptrWorld->Map->Map->MapGetBlock((int)ptrWorld->Player->position.x/32,(int)ptrWorld->Player->position.y/32);
	BLKSTR* blockTemp = this->ptrWorld->Map->MapSource->MapGetBlock((int)Ent->position.x/this->ptrWorld->Map->WorldInfo.tile_size,(int)Ent->position.y/this->ptrWorld->Map->WorldInfo.tile_size);
	/*if (this->ptrWorld->Map->tile_map[tempp].Occupied) {
		Ent->position.x -= Ent->velocity * sin(Ent->vector);
		Ent->position.y += Ent->velocity * cos(Ent->vector);
	}*/

	//currently all blocks have full collision, so only need to check 1 corner
	if (blockTemp->tl) {// || this->ptrWorld->Map->tile_map[tempp].Occupied) {
		//Ent->position.x -= Ent->velocity * sin(Ent->vector) * physTimeScale;
		//Ent->position.y += Ent->velocity * cos(Ent->vector) * physTimeScale;
		Ent->phys_state = states::PHYS_STATE_COLLIDE_MAP;
	} else { }

	long tileNum = 0;
	float dir;
	
	/*for (dir = 0.0f; dir < atan(1.0f)*2; dir+= atan(1.0f)/2) {
		tileNum = this->getFutureTile(Ent,Vectorize(Ent->vector,dir),90);
		this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
		tileNum = this->getFutureTile(Ent,Vectorize(Ent->vector,dir*-1),90);
		this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
	}*/

	/*for (dir = 0.0f; dir < atan(1.0f)*2; dir+= atan(1.0f)/2) {
		tileNum = this->getFutureTile(Ent,Vectorize(Ent->vector,dir),50);
		this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
		tileNum = this->getFutureTile(Ent,Vectorize(Ent->vector,dir*-1),50);
		this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
	}*/

	//tileNum = this->GetClosestSpriteID(Ent,false);

	
	//Apply the velocity to position
	





	//tileNum = this->getFutureTile(Ent,Vectorize(Ent->vector,(PI/2)*-1,30);

	float vec = (PI/4)*-1;
	long tileX = (int)(this->getRelX(Ent->position.x,Vectorize(Ent->vector,vec),32)/32);
	long tileY = (int)(this->getRelY(Ent->position.y,Vectorize(Ent->vector,vec),32)/32);
	//this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);

	if (this->ptrWorld->Map->MapSource->MapGetBlock(tileX,tileY)->tl) {
		Ent->vector = Vectorize(Ent->vector,(PI/4)*1);
	} else {

		vec = (PI/4)*1;
		tileX = (int)(this->getRelX(Ent->position.x,Vectorize(Ent->vector,vec),32)/32);
		tileY = (int)(this->getRelY(Ent->position.y,Vectorize(Ent->vector,vec),32)/32);

		if (this->ptrWorld->Map->MapSource->MapGetBlock(tileX,tileY)->tl) {
			Ent->vector = Vectorize(Ent->vector,(PI/4)*-1);
		}

	}

	Ent->position.x += Ent->velocity * sin(Ent->vector) * physTimeScale;
	Ent->position.y -= Ent->velocity * cos(Ent->vector) * physTimeScale;

	//tileNum = this->getFutureTile(Ent,Vectorize(Ent->vector,atan(1.0f)),30);
	//tileNum = this->getRelativeTile(Ent,Vectorize(Ent->vector,(PI/4)),32);
	//tileNum = this->getRelativeTile(Ent,Ent->vector,32);
	//this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);

	//tileNum = this->getFutureTile(Ent,Vectorize(Ent->vector,0),3);

	if (Ent->ID != -1) {
		long adjust = 5;
		long dist = 24;

		vec = (PI/8)*15;
		vec = (PI/2);
		//right
		tileNum = boundFix(this->getRelativeTile(Ent,Vectorize(Ent->vector,vec),dist));
		
		if (this->occupyAllCheck(tileNum)) {
			this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
			Ent->position.x += adjust * sin(Vectorize(Ent->vector,(PI/2)*3)) * physTimeScale;
			Ent->position.y -= adjust * cos(Vectorize(Ent->vector,(PI/2)*3)) * physTimeScale;
		}

		//left
		vec = (PI/2)*3;
		tileNum = boundFix(this->getRelativeTile(Ent,Vectorize(Ent->vector,vec),dist));
		
		if (this->occupyAllCheck(tileNum)) {
			this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
			Ent->position.x += adjust * sin(Vectorize(Ent->vector,(PI/2))) * physTimeScale;
			Ent->position.y -= adjust * cos(Vectorize(Ent->vector,(PI/2))) * physTimeScale;
		}

		//front
		vec = 0;
		tileNum = boundFix(this->getRelativeTile(Ent,Vectorize(Ent->vector,vec),dist));
		
		if (this->occupyAllCheck(tileNum)) {
			this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
			Ent->position.x += adjust/2 * sin(Vectorize(Ent->vector,(PI))) * physTimeScale;
			Ent->position.y -= adjust/2 * cos(Vectorize(Ent->vector,(PI))) * physTimeScale;
		}

		//back
		vec = (PI)*1;
		tileNum = boundFix(this->getRelativeTile(Ent,Vectorize(Ent->vector,vec),dist));
		
		if (this->occupyAllCheck(tileNum)) {
			this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
			Ent->position.x += adjust * sin(Vectorize(Ent->vector,0)) * physTimeScale;
			Ent->position.y -= adjust * cos(Vectorize(Ent->vector,0)) * physTimeScale;
		}
	}
	
	//REWRITE THIS CODE, IT ITERATES THROUGH ALL TILES, JUST USE FUTURE PREDICTIONS FOR NOW ON!!!!!!!!!!!!!!

	/*
	if (tileNum > 0 && tileNum < this->ptrWorld->Map->WorldInfo.total_tiles) {
		//this->ptrWorld->Map->pred_map[tileNum].Occupied = true;
		//this->ptrWorld->Map->pred_map[tileNum].OccupyData.push_back(Ent->ID);
		if (this->ptrWorld->Map->tile_map[tileNum].Occupied) {
			//s = ptrWorld->Units.begin();
			//s+= this->ptrWorld->Map->tile_map[tileNum].OccupyData.at(0);
			//ptrWorld->Units.erase(s);
			if (this->ptrWorld->Map->tile_map[tileNum].OccupyData.size() > 0) {
				for (int i = 0; i < this->ptrWorld->Map->tile_map[tileNum].OccupyData.size(); i++) {
					phys_obj* temp = ((phys_obj* )getSprite(this->ptrWorld->Map->tile_map[tileNum].OccupyData.at(i)));
					if (CheckCircularCollide(((phys_obj* )Ent),temp) && ((projectile* )Ent)->ID != temp->ID) {
						//int temp2 = this->ptrWorld->Map->tile_map[tileNum].OccupyData.at(i);
						//ptrWorld->KillUnit(temp2);
						//this->ptrWorld->Map->tile_map[tileNum].Occupied = false;
						
						//Ent->phys_state = states::PHYS_STATE_COLLIDE_UNIT;
					}
				}
				
			}
		}
	}*/

	if (Ent->phys_state == states::PHYS_STATE_COLLIDE_UNIT || Ent->phys_state == states::PHYS_STATE_COLLIDE_MAP) {
		Ent->position.x -= (Ent->velocity*1.0 * sin(Ent->vector) * physTimeScale)*2;
		Ent->position.y += (Ent->velocity*1.0 * cos(Ent->vector) * physTimeScale)*2;

		
	}

	if (Ent->phys_state == states::PHYS_STATE_COLLIDE_MAP) {
		Ent->velocity = 0.0;
	}
}
/* */
bool Physics::LOSCheck(phys_obj* physPtr1, phys_obj* physPtr2) {
	//this->ptrWorld->los_checks++;
	float heading = getHeading(physPtr1->position.x,physPtr1->position.y,physPtr2->position.x,physPtr2->position.y);
	float distance = getDistance(physPtr1->position.x,physPtr1->position.y,physPtr2->position.x,physPtr2->position.y);

	for (float tempDist = 0; tempDist < distance; tempDist += ptrWorld->Map->WorldInfo.tile_size/2) {
		//long tile = getRelX(physPtr1,heading,tempDist);
		BLKSTR* blockTemp = this->ptrWorld->Map->MapSource->MapGetBlock(getRelX(physPtr1->position.x,heading,tempDist)/ptrWorld->Map->WorldInfo.tile_size,getRelY(physPtr1->position.y,heading,tempDist)/ptrWorld->Map->WorldInfo.tile_size);
		if (blockTemp->tl) {
			return false;
		}
	}
	return true;
}

coord Physics::CalcTrajectory(coord curPos, float vector, long pixels) {
	//coord temp = ;
	curPos.x += pixels*sin(vector);
	curPos.y -= pixels*cos(vector);
	return curPos;
}

phys_obj* Physics::GetClosestSpriteID(phys_obj* physPtr, bool findEnemy = false) {
	long tileNum = 0;
	float dir = 0.0f;
	int range = (physPtr->viewdistance/3);

	for (range; range < physPtr->viewdistance; range+=physPtr->viewdistance/3) {
		for (dir = 0; dir < atan(1.0f)*4; dir+= atan(1.0f)/2) {
			for (long lr = 1; lr >= -1; lr-=2) {
				tileNum = this->getRelativeTile(physPtr,Vectorize(physPtr->vector,dir*lr),range);
				if (tileNum > 0 && tileNum < this->ptrWorld->Map->WorldInfo.total_tiles) {
				//this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
				//this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);

					if (this->ptrWorld->Map->tile_map[tileNum].Occupied) {
						for (int i = 0; i < this->ptrWorld->Map->tile_map[tileNum].OccupyCount; i++) {
							TempEnt = getSprite(this->ptrWorld->Map->tile_map[tileNum].OccupyData[i]);
							if (physPtr->team == TempEnt->team) {
								if (!findEnemy) {
									return TempEnt;
								}
							} else {
								if (findEnemy) {
									return TempEnt;
								}
							}
						}
					}
				}
			}
			
		}
	}
	

	return new phys_obj(-2);

}

phys_obj* Physics::GetClosestVisibleSprite(phys_obj* physPtr, bool findEnemy = false) {
	long tileNum = 0;
	float dir = 0.0f;
	int range = (physPtr->viewdistance/3);

	

	for (range; range < physPtr->viewdistance; range+=physPtr->viewdistance/3) {
		for (dir = 0; dir < atan(1.0f)*4; dir+= atan(1.0f)/2) {
			for (long lr = 1; lr >= -1; lr-=2) {

				while(this->ptrWorld->lock_tile_map) { }

				tileNum = this->getRelativeTile(physPtr,Vectorize(physPtr->vector,dir*lr),range);
				if (tileNum > 0 && tileNum < this->ptrWorld->Map->WorldInfo.total_tiles) {
				//this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);
				//this->addToTileMap(this->ptrWorld->Map->pred_map,tileNum,Ent->ID);

					if (this->ptrWorld->Map->tile_map[tileNum].Occupied) {
						//this->ptrWorld->los_checks++;
						for (int i = 0; i < this->ptrWorld->Map->tile_map[tileNum].OccupyCount; i++) {
							ptrWorld->ai_time->start();

							while(this->ptrWorld->lock_tile_map) { }

							TempEnt = getSprite(this->ptrWorld->Map->tile_map[tileNum].OccupyData[i]);

							ptrWorld->AI->ai_ms_count += ptrWorld->ai_time->get_ticks();
							if (physPtr->team == TempEnt->team) {
								if (!findEnemy) {
									if (this->LOSCheck(physPtr,TempEnt)) {
										
										return TempEnt;
									}
								}
							} else {
								if (findEnemy) {
									if (this->LOSCheck(physPtr,TempEnt)) {
										
										return TempEnt;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	return new phys_obj(-2);

}

bool Physics::CheckCircularCollide(phys_obj* physPtr, phys_obj* physPtr2) {
	int x1, x2, y1, y2;
	if (physPtr2->ID >= -1) {
		x1 = physPtr->position.x;
		y1 = physPtr->position.y;
		
		x2 = physPtr2->position.x;
		y2 = physPtr2->position.y;
	

		//this needs to be more accurate, this is best for square, need one that uses sin, cos with width and height
		if (getDistance(x1,y1,x2,y2)-((physPtr->width/2)+(physPtr2->width/2)) < -8) {
			return true;
		} else { return false; }
	} else {
		return false;
	}
}


void Physics::UpdateCollideMap() {

	int i, k, j, l;
	long xLow, xHigh, yLow, yHigh;
	//TileOccupyInfo* TOI = new TileOccupyInfo;


	this->ptrWorld->lock_tile_map = true;
	//Unit location map
	//this->ptrWorld->Map->tile_map

	//this->ptrWorld->Map->tile_map_swap = this->ptrWorld->Map->tile_map_temp;
	//this->ptrWorld->Map->tile_map_temp
	//this->ptrWorld->Map->tile_map = this->ptrWorld->Map->tile_map_swap;

	for (i = 0; i < ptrWorld->Map->WorldInfo.total_tiles; i++) {

		this->ptrWorld->Map->tile_map_swap[i].Occupied = this->ptrWorld->Map->tile_map[i].Occupied;
		//this->ptrWorld->Map->tile_map_swap[i].OccupyData = this->ptrWorld->Map->tile_map[i].OccupyData;
		if (this->ptrWorld->Map->tile_map[i].OccupyCount > 0) {
			for (int j = 0; j < this->ptrWorld->Map->tile_map[i].OccupyCount; j++) {
				this->ptrWorld->Map->tile_map_swap[i].OccupyData[j] = this->ptrWorld->Map->tile_map[i].OccupyData[j];
				
			}
		}
		//this->ptrWorld->Map->tile_map_swap[i] = this->ptrWorld->Map->tile_map[i];
		//this->ptrWorld->Map->tile_map_temp[i].Occupied = false;
		//this->ptrWorld->Map->tile_map_temp[i].OccupyData.clear();
	
	}

	this->ptrWorld->Map->tile_map_temp = this->ptrWorld->Map->tile_map;

	this->ptrWorld->Map->tile_map = this->ptrWorld->Map->tile_map_swap;

	for (i = 0; i < ptrWorld->Map->WorldInfo.total_tiles; i++) {
		this->ptrWorld->Map->tile_map_temp[i].reset();
	}

	//Prediction map
	for (i = 0; i < ptrWorld->Map->WorldInfo.total_tiles; i++) {
		this->ptrWorld->Map->pred_map[i].reset();
	}
	/*for (i = 0; i < ptrWorld->Map->WorldInfo.total_tiles; i++) {

		this->ptrWorld->Map->path_map[i].Occupied = false;
		this->ptrWorld->Map->path_map[i].OccupyData.clear();
	
	}
	for (i = 0; i < ptrWorld->Map->WorldInfo.total_tiles; i++) {

		//this->ptrWorld->Map->node_map[i].Occupied = false;
		//this->ptrWorld->Map->node_map[i].OccupyData.clear();
	
	}*/
	
	//BOOKMARK, TEMP DISABLED!!
	/*for (int i = 0; i < this->Entities[0]->ptrBrain->navData.nodeOrderData.size(); i++) {
		this->ptrWorld->ptrPhysics->addToTileMap(ptrWorld->Map->node_map,this->Entities[0]->ptrBrain->navData.nodeOrderData.at(i)->x,this->Entities[0]->ptrBrain->navData.nodeOrderData.at(i)->y,this->Entities[0]->ptrBrain->navData.nodeOrderData.at(i)->ID);
		//this->ptrWorld->ptrPhysics->addToTileMap(ptrWorld->Map->node_map,this->Entities[0]->ptrBrain->navData.nodeOrderData.at(i)->x,this->Entities[0]->ptrBrain->navData.nodeOrderData.at(i)->y,this->Entities[0]->ptrBrain->navData.nodeOrderData.at(i)->ID);
	}*/

	int tX = this->ptrWorld->Map->WorldInfo.tiles_count_x;
	//this->ptrWorld->Map->tile_map_temp[0].Occupied = true;
	//this->ptrWorld->Map->tile_map_temp[0].OccupyData.push_back(99);
	for (k = 0;k <= ptrWorld->Units.size();k++)
	{
		if (k == ptrWorld->Units.size()) {
			ptrSprite = this->ptrWorld->Player;
		} else {
			ptrSprite = Entities.at(k);
		}
		
		if (ptrSprite->phys_state != states::PHYS_STATE_DEAD && ptrSprite->fullyCreated) {

			xLow = (int)((ptrSprite->position.x-(ptrSprite->width/2))/32);
			xHigh = (int)((ptrSprite->position.x+(ptrSprite->width/2))/32);
			yLow = (int)((ptrSprite->position.y-(ptrSprite->height/2))/32);
			yHigh = (int)((ptrSprite->position.y+(ptrSprite->height/2))/32);
			
			
			for (i = xLow; i <= xHigh; i++) {
				for (j = yLow; j <= yHigh; j++) {
					
					//TOI->when = ptrWorld->runTime;
					//TOI->who = Ent;
					l = i+(j*tX);
					if (l >= 0 && l < ptrWorld->Map->WorldInfo.total_tiles) {
						this->ptrWorld->Map->tile_map_temp[l].Occupied = true;
						this->ptrWorld->Map->tile_map_temp[l].OccupyData[this->ptrWorld->Map->tile_map_temp[l].OccupyCount] = ptrSprite->ID;
						this->ptrWorld->Map->tile_map_temp[l].OccupyCount++;
						this->ptrWorld->Map->tile_map_temp[l].team = ptrSprite->team;
					}
				}
			}


		}
	}

	//this->ptrWorld->Map->tile_map[6].OccupyData.push_back(66);
	//this->ptrWorld->Map->tile_map_temp[5].OccupyData.push_back(55);

	this->ptrWorld->Map->tile_map_swap = this->ptrWorld->Map->tile_map;

	this->ptrWorld->Map->tile_map = this->ptrWorld->Map->tile_map_temp;

	this->ptrWorld->Map->tile_map_temp = this->ptrWorld->Map->tile_map_swap;

	//long temppp = this->ptrWorld->Map->tile_map[5].OccupyData.at(0);
	//long temppp2 = this->ptrWorld->Map->tile_map_temp[5].OccupyData.front();

	this->ptrWorld->lock_tile_map = false;

	//return;
	
}

phys_obj* Physics::getSprite(long ID) {

	//if player
	if (ID == -1) {
		return this->ptrWorld->Player;
	} else if (ID == -2) {
		return new phys_obj(-2);
	}

	return Entities[ID];
	//return this->ptrWorld->AI->Entities[ID];
	//return Entities.at(ID);

	//caused REALLY SLOW issues with 80+ entities called thru get closest sprite
	/*for (int i = 0; i < this->Entities.size(); i++) {
		if (ID == Entities.at(i)->ID) {
			return Entities.at(i);
		}
	}*/
	return new phys_obj(-2);
}

long Physics::getFutureTile(phys_obj *Ent, float newVec, long futureTicks) {
	float xFuture;
	float yFuture;

	xFuture = Ent->position.x;
	yFuture = Ent->position.y;
	
	for (int fCount = 0; fCount < futureTicks; fCount++) {
		xFuture += Ent->velocity * sin(newVec) * (physTimeScale);
		yFuture -= Ent->velocity * cos(newVec) * (physTimeScale);
	}

	int uh = this->ptrWorld->Map->WorldInfo.tile_size;

	return (((int)yFuture/uh)*this->ptrWorld->Map->WorldInfo.tiles_count_x)+((int)xFuture/uh);

}

long Physics::getRelativeTile(phys_obj *Ent, float newVec, long pixels) {
	float xFuture;
	float yFuture;

	xFuture = Ent->position.x;
	yFuture = Ent->position.y;
	
	xFuture += pixels * sin(newVec);
	yFuture -= pixels * cos(newVec);

	//int uh = this->ptrWorld->Map->WorldInfo.tile_size;

	return (((int)yFuture/ts)*this->ptrWorld->Map->WorldInfo.tiles_count_x)+((int)xFuture/ts);

}

long Physics::getRelX(long x, float newVec, long pixels) {
	float xFuture;

	xFuture = x;

	xFuture += pixels * sin(newVec);
	
	xFuture = (long)xFuture;
	if (xFuture < 0) {
		xFuture = 0;
	}

	return xFuture;
	

}

long Physics::getRelY(long y, float newVec, long pixels) {
	float yFuture;

	yFuture = y;
	
	yFuture -= pixels * cos(newVec);

	yFuture = (long)yFuture;
	if (yFuture < 0) {
		yFuture = 0;
	}

	return yFuture;

}

long Physics::tiletoX(long tilenum) {

	long temp = (int)(tilenum / this->ptrWorld->Map->WorldInfo.tiles_count_x);

	return tilenum-(temp*this->ptrWorld->Map->WorldInfo.tiles_count_x);
	
	//return (((int)yFuture/uh)*this->ptrWorld->Map->WorldInfo.tiles_count_x)+((int)xFuture/uh);

}



long Physics::tiletoY(long tilenum) {

	return (int)(tilenum / this->ptrWorld->Map->WorldInfo.tiles_count_x);

	
	
}

bool Physics::occupyMapCheck(long tilenum) {
	return this->ptrWorld->Map->MapSource->MapGetBlock(tiletoX(tilenum),tiletoY(tilenum))->tl;
}

bool Physics::occupyUnitCheck(long tilenum) {
	//return this->ptrWorld->Map->tile_map[tilenum].Occupied;

	if (this->ptrWorld->Map->tile_map[tilenum].Occupied) {
		if (this->ptrWorld->Map->tile_map[tilenum].OccupyCount > 1) {
			//return true since at least 2 units are in the way, other cant be you
			return true;
		} else {
			if (Ent->ID != this->ptrWorld->Map->tile_map[tilenum].OccupyData[0]) {
				return true;
			}
		}
	}

	return false;
}

bool Physics::occupyAllCheck(long tilenum) {
	if (occupyUnitCheck(tilenum)) {
		return true;
	} else {
		return this->ptrWorld->Map->MapSource->MapGetBlock(tiletoX(tilenum),tiletoY(tilenum))->tl;
	}

}

bool Physics::boundCheck(long tilenum) {
	if (tilenum > 0 && tilenum < this->ptrWorld->Map->WorldInfo.total_tiles) {
		return true;
	}
	return false;
}

long Physics::boundFix(long tilenum) {
	if (tilenum > 0 && tilenum < this->ptrWorld->Map->WorldInfo.total_tiles) {
		return tilenum;
	}
	return 0;
}

void Physics::addToTileMap(Tile* map, long tileNum, long sprID) {
	if (tileNum > 0 && tileNum < this->ptrWorld->Map->WorldInfo.total_tiles) {
		map[tileNum].Occupied = true;
		map[tileNum].OccupyData[map[tileNum].OccupyCount] = sprID;
		//map[tileNum].OccupyData.push_back(sprID);
	}
}

void Physics::addToTileMap(Tile* map, long x, long y, long sprID) {
	long tileNum = (y*this->ptrWorld->Map->WorldInfo.tiles_count_x)+x;
	if (tileNum > 0 && tileNum < this->ptrWorld->Map->WorldInfo.total_tiles) {
		map[tileNum].Occupied = true;
		map[tileNum].OccupyData[map[tileNum].OccupyCount] = sprID;
	}
}