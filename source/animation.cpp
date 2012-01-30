#include "animation.h"
#include "sprite.h"
#include "math.h"
#include "states.h"
#include "phys_obj.h"

sprite_animations::sprite_animations() {
	this->active = false;
	this->initialized = false;
}

sprite_animations::~sprite_animations() {

}

void sprite_animations::Init(phys_obj* sprID) {
	this->ptrSprite = sprID;
	this->ID = ptrSprite->ID;
	this->curAnimID = 0;
	this->curAnimFrame = 0;
	this->curAnimStartTime = 0;
	this->active = false;
	this->initialized = true;
}



void sprite_animations::LoadAnimations(int count) {

	

}

void sprite_animations::Animate() {
	//if ((this->active) && (this->curAnimID > 0)) {
	if (this->active) {
		if (this->ID == -1) {
			int safdsad = 0;
		}
		/*if (this->animations[this->curAnimID].
		this->curAnimFrame++;
		if (this->curAnimFrame > this->animations[this->curAnimID].frames) {
			this->curAnimFrame = 0;	
		}*/
		//this->curAnimFrame = (int)(fmod(this->ptrSprite->ptrWorld->runTime - (this->ptrSprite->ResID->animData[this->curAnimID].execTime*1.0f),this->ptrSprite->ResID->animData[this->curAnimID].execTime * 1.0f) / (ptrSprite->ResID->animData[this->curAnimID].execTime / ptrSprite->ResID->animData[this->curAnimID].frames));
		if ((this->ptrSprite->ptrWorld->runTime - this->curAnimStartTime) > (this->ptrSprite->ResID->stateToAnim.at(this->curAnimID)->execTime / this->ptrSprite->ResID->stateToAnim.at(this->curAnimID)->frames)) {
			this->curAnimFrame++;
			this->curAnimStartTime = this->ptrSprite->ptrWorld->runTime;
		}
		if (this->curAnimFrame >= this->ptrSprite->ResID->stateToAnim.at(this->curAnimID)->frames || this->curAnimFrame < 0) {
			if (curAnimID == nextAnimID) {
				this->curAnimFrame = 0;
			} else {
				this->curAnimFrame = 0;
				Start(nextAnimID);
			}
		}
	}
}

void sprite_animations::Stop() {
	this->curAnimFrame = 0;
	this->curAnimStartTime = 0;
	//this->active = false;
}

void sprite_animations::PlayOnce(int ID, int nextID) {
	this->curAnimID = ID;
	this->nextAnimID = nextID;
	if (this->curAnimID != ID) {
		this->curAnimStartTime = this->ptrSprite->ptrWorld->runTime;
	}
	this->active = true;
	//if (ID > 0) { this->active = true; } else { }//this->active = false; }
}

void sprite_animations::Start(int ID) {
	if (this->ID > -1) {
		this->curAnimID = ID;
		this->nextAnimID = ID;
		this->curAnimStartTime = this->ptrSprite->ptrWorld->runTime;
		this->active = true;
	}
	//if (ID > 0) { this->active = true; } else { }//this->active = false; }
}

void sprite_animations::Reset() {
	//this->curAnimID = ID;
	this->curAnimFrame = 0;
	this->curAnimStartTime = this->ptrSprite->ptrWorld->runTime;
	//this->active = false;
}