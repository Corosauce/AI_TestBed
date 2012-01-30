#include "sprite.h"

#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include <windows.h>
#include "math.h"
#include "phys_math.h"
#include "states.h"

#include "content_manager.h"



sprite::sprite() {


	position.x = 0;
	position.y = 0;

	weapons[0].ammo = 0;
	weapons[0].maxammo = 0;
	weapons[0].range = 0;
	weapons[0].reloadtime = 0;
	weapons[0].lastshottime = -100000;

	items[0].count = 0;
	items[0].maxcount = 0;
	items[0].range = 0;

	//health = 0;
	//viewdistance = 0;

	width = 0;
	height = 0;

	this->typeID = 0;
	this->fullyCreated = false;
	this->declareDead = false;

}

sprite::~sprite() {

	//delete this->

}
void sprite::Init(sprite* SpritePtr, Game_Engine* worldpointer) {

	SprID = SpritePtr;
	ptrWorld = worldpointer;

	team = 1;

	health = this->type->health;
	viewdistance = this->type->viewdistance;

	position.x = 0.0f;// + (rand() % (int)this->ptrWorld->Map->WorldInfo.width);
	position.y = 0.0f;// + (rand() % (int)this->ptrWorld->Map->WorldInfo.height);

	angle.frame = 0;
	angle.max = this->type->angle.max;

	velocity = 0.0f;
	vector = 0.0f;
	targetvector = 0.0f;
	turndirection = 0.0f; //dynamic var
	turnspeed = this->type->turnspeed;
	goforce = this->type->goforce;
	stopforce = this->type->stopforce;
	acceleration = 0.0f;

	cruisespeed = this->type->cruisespeed;
	maxforcespeed = this->type->maxforcespeed;

	this->weapons[0].name = this->type->weapons[0].name;
	this->weapons[0].range = this->type->weapons[0].range;
	this->weapons[0].reloadtime = this->type->weapons[0].reloadtime;
	
	target.ID = 0;//rand() % (int)this->ptrWorld->unitNum;

	phys_state = states::PHYS_STATE_NORMAL;

	this->animation_list.Init(((phys_obj* )SpritePtr));
	
}

void sprite::Process() {
	
	if (this->ai_mm_state == states::AI_MM_ATTACK) {
		Shoot(vector);
	}
}

void sprite::Render() {


	//Updates animations;
	if (this->animation_list.active) {
		if (this->animation_list.ID < 0) {
			int sdfsdfsdf = 0;
		}
		this->animation_list.Animate();
	}

	float temp = (atan(1.0f)*8) / (angle.max+1);
	float temp2 = this->vector / temp;

	//MOVE THIS TO THE ANIMATION DATA
	int curFrame = 0;//ResID->animData[this->animation_list.curAnimID].xStart+(this->animation_list.curAnimFrame*ResID->resInfo.xTileSize);

	//temp2 += (temp/2); // offset the angle by half a frame, more accurate visual
	angle.frame = (int)(temp2+0.5f); // this actually rounds it up or down (c++ method)

	if (angle.frame > angle.max) { angle.frame = 0; }
	if (angle.frame < 0) { angle.frame = angle.max; }

	if (angle.frame == 1) {
		int sdf = 0;
	}

	SDL_Rect pClip;
	SDL_Rect pClip2;
	
	SDL_Rect frameClip = this->ResID->stateToAnim.at(this->animation_list.curAnimID)->clipArray[this->animation_list.curAnimFrame].angles[angle.frame].clip;
	
	/*frameClip.x = curFrame;
	frameClip.y = angle.frame*ResID->resInfo.yTileSize;
	frameClip.w = ResID->resInfo.xTileSize;//ResID->resInfo.width;
	frameClip.h = ResID->resInfo.xTileSize;//ResID->resInfo.height;*/

	
	//SDL_Rect frameClip
	/*frameClip.x = 0;
	frameClip.y = 0;
	frameClip.w = ResID->surface->w;//ResID->resInfo.width;
	frameClip.h = ResID->surface->h;//ResID->resInfo.height;*/


	int xOffset;
	int yOffset;
	xOffset = position.x + this->ResID->stateToAnim.at(this->animation_list.curAnimID)->clipArray[this->animation_list.curAnimFrame].angles[angle.frame].xOffset;
	yOffset = position.y + this->ResID->stateToAnim.at(this->animation_list.curAnimID)->clipArray[this->animation_list.curAnimFrame].angles[angle.frame].yOffset;

	xOffset -= (this->ResID->resInfo.xTileSize/2);
	yOffset -= (this->ResID->resInfo.yTileSize/2);

	
	if (ptrWorld->Map->inViewport(xOffset,yOffset,frameClip.w,frameClip.h) || 0) {
		this->ptrWorld->unitsVisible++;
		pClip = ptrWorld->Map->clipToViewport(xOffset,yOffset,frameClip);
		//pClip = frameClip;
		xOffset = (pClip.x - frameClip.x) + ptrWorld->Map->ViewportInfo.gui_x + xOffset - ptrWorld->Map->ViewportInfo.x;
		yOffset = (pClip.y - frameClip.y) + ptrWorld->Map->ViewportInfo.gui_y + yOffset - ptrWorld->Map->ViewportInfo.y;
		
		pClip2.x = xOffset;
		pClip2.y = yOffset;
		pClip2.w = pClip.w;
		pClip2.h = pClip.h;


		//this->ID;
		//SDL_FillRect( ptrWorld->ptrScreen->screen, &pClip2, SDL_MapRGB( ResID->surface->format, 0x00, 0x00, 0x48 ) );
		ptrWorld->ptrScreen->apply_surface(xOffset, yOffset, ResID->surface, ptrWorld->ptrScreen->screen, &pClip );

#ifdef SHOW_DEBUG_TEXT
		if (showUnitInfo) {
			sstr.str("");
			sstr<<"HP: "<<this->health<<" | ID: "<<this->ID<<" | JOB: "<<this->ai_job_state<<" | FC: "<<this->fullyCreated;
			str1 = sstr.str();

			ptrWorld->ptrScreen->printText(xOffset,yOffset+30,str1.c_str());
		}
#endif
		//ptrWorld->ptrScreen->apply_surface(xOffset, yOffset, ResID->test, ptrWorld->ptrScreen->screen);//, &pClip );
	}

#ifdef SHOW_DEBUG_TEXT
	//Render Text
	if (ID == -1) {
		sstr.str("");
		//sstr.str("speed: ");
		//sstr<<this->vector<<" - "<<this->targetvector;
		//sstr<<this->position.x/this->ptrWorld->Map->WorldInfo.tile_size<<" - "<<this->position.y/this->ptrWorld->Map->WorldInfo.tile_size;
		
		sstr<<temp2<<" - "<<this->targetvector<<" - "<<this->vector;
		str1 = sstr.str();
		ptrWorld->ptrScreen->printText(10,10,str1.c_str());
	}
#endif
	
	//message = TTF_RenderText_Solid( font, str1.c_str(), *textColor );
	// |
	//pt2apply_surface(Units[i].position.x+40, Units[i].position.y+10, message, screen, NULL );

}

void sprite::Move(float x, float y)
{
	position.x += x;
	position.y += y;
}

void sprite::Shoot(float vector)
{
	if (this->ptrWorld->runTime > this->weapons[0].lastshottime+this->weapons[0].reloadtime) {
		if (AccuracyCheck(this->vector,this->targetvector,0.3f)) {
			this->weapons[0].lastshottime = this->ptrWorld->runTime;
			ptrWorld->MakePrj(this->SprID,vector);
		}
	}
}

void sprite::Warp(float x, float y)
{
	position.x = x;
	position.y = y;

	
}