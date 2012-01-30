#include "player.h"
#include "physics.h"
#include "ai.h"

#include "phys_math.h"

player::player() {
	for (int i = 0; i < 323; i++) {
		this->keys[i] = false;
	}

	this->position.x = 300;
	this->position.y = 300;

	this->mouse.button = 0;
}

int player::DoMoves() {
	if (keys[SDLK_UP]) {
		acceleration = goforce;
	} else {
		acceleration = 0.0f;
	}

	if (keys[SDLK_DOWN]) {
		if (acceleration > -2) { acceleration -= stopforce; }
		else { acceleration = -2.0f; }
	}
	if (keys[SDLK_LEFT]) {
		targetvector = Vectorize(targetvector,this->turnspeed * (this->ptrWorld->ptrPhysics->physTimeScale/4)*-1.0f);
		//targetvector -= this->turnspeed * (this->ptrWorld->ptrPhysics->physTimeScale/4);
		//Units[0].position.x -= Units[0].acceleration*20;
	}
	if (keys[SDLK_RIGHT]) {
		targetvector = Vectorize(targetvector,this->turnspeed * (this->ptrWorld->ptrPhysics->physTimeScale/4)*1.0f);
		//targetvector += this->turnspeed * (this->ptrWorld->ptrPhysics->physTimeScale/4);
		//Units[0].position.x += Units[0].acceleration*20;
	}

	if (keys[SDLK_w]) {
		this->ptrWorld->Map->ViewportInfo.y-=(5*this->ptrWorld->ptrPhysics->physTimeScale);
	}
	if (keys[SDLK_a]) {
		this->ptrWorld->Map->ViewportInfo.x-=(5*this->ptrWorld->ptrPhysics->physTimeScale);
	}
	if (keys[SDLK_s]) {
		this->ptrWorld->Map->ViewportInfo.y+=(5*this->ptrWorld->ptrPhysics->physTimeScale);
	}
	if (keys[SDLK_d]) {
		this->ptrWorld->Map->ViewportInfo.x+=(5*this->ptrWorld->ptrPhysics->physTimeScale);
	}
	if (keys[SDLK_KP1]) {
		ptrWorld->scen_process = true;
		//this->animation_list.Start(1);
	}
	if (keys[SDLK_KP2]) {
		//this->animation_list.Start(2);
	}
	if (keys[SDLK_KP3]) {
		//this->animation_list.Start(10);
	}
	if (keys[SDLK_KP0]) {
		//this->animation_list.Start(0);
		ptrWorld->scen_process = false;
	}
	if (keys[SDLK_KP4]) {
		//this->animation_list.Start(4);
	}

	if (keys[SDLK_SPACE]) {
		
		this->ptrWorld->AI->SetGlobalTarget(-1);
	}

	if (keys[SDLK_DELETE]) {
		this->ptrWorld->phys_process = !this->ptrWorld->phys_process;
		//Units[0].position.y += Units[0].acceleration*20;
	}

	if (keys[SDLK_END]) {
		this->ptrWorld->ai_process = !this->ptrWorld->ai_process;
	}

	if (keys[SDLK_KP_PLUS]) {
		//this->ptrWorld->MakeRandUnit();
		this->ptrWorld->MakeUnit("vulture",1200,1800,2);
		this->ptrWorld->MakeNewUnitsAI();
	}

	if (keys[SDLK_KP_MINUS]) {
		this->ptrWorld->KillAllUnits();
	}

	if (keys[SDLK_e]) {
		this->Shoot(this->vector);
	}

	if (velocity > 0) {
		if (!animation_list.active) {
			animation_list.Start(1);
		}
	} else { }//animation_list.Stop(); }

	if (this->ptrWorld->phys_process) {
		this->ptrWorld->Map->ViewportInfo.x = position.x+(width/2)-(this->ptrWorld->Map->ViewportInfo.width/2);
		this->ptrWorld->Map->ViewportInfo.y = position.y+(height/2)-(this->ptrWorld->Map->ViewportInfo.height/2);
	}
	
	if (this->ptrWorld->Map->ViewportInfo.x < 0) {
		this->ptrWorld->Map->ViewportInfo.x = 0;
	}
	if (this->ptrWorld->Map->ViewportInfo.y < 0) {
		this->ptrWorld->Map->ViewportInfo.y = 0;
	}
	if (this->ptrWorld->Map->ViewportInfo.x > this->ptrWorld->Map->WorldInfo.width-this->ptrWorld->Map->ViewportInfo.width) {
		this->ptrWorld->Map->ViewportInfo.x = this->ptrWorld->Map->WorldInfo.width-this->ptrWorld->Map->ViewportInfo.width;
	}
	if (this->ptrWorld->Map->ViewportInfo.y > this->ptrWorld->Map->WorldInfo.height-this->ptrWorld->Map->ViewportInfo.height) {
		this->ptrWorld->Map->ViewportInfo.y = this->ptrWorld->Map->WorldInfo.height-this->ptrWorld->Map->ViewportInfo.height;
	}


	return 1;
}

void player::InitPlayer() {
	this->position.x = 300;
	this->position.y = 300;

	this->fullyCreated = true;

}

/*void player::Init(sprite* SpritePtr, Game_Engine* worldpointer) {

	SprID = SpritePtr;
	ptrWorld = worldpointer;

	

	health = 100;
	viewdistance = 20;

	position.x = 50;
	position.y = 50;

	angle.frame = 0;
	angle.max = 15;

	velocity = 0.0f;
	vector = 0.0f;
	targetvector = 0.0f;
	turndirection = 0.0f; //dynamic var
	turnspeed = 0.1f;

	goforce = 0.05f;
	stopforce = 0.05f;
	acceleration = 0.0f;
	

	cruisespeed = 5.5f;
	maxforcespeed = 8.0f;
	
	target.ID = 0;//rand() % (int)this->ptrWorld->unitNum;

	animation_list.Init(SpritePtr);
	
}*/