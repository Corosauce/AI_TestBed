#include "projectile.h"

#include "content_manager.h"

#include "math.h"

projectile::projectile() {
	velocity = 0.0f;
	angle.frame = 0;
	acceleration = 0.0f;
	vector = 0.0f;
}

projectile::~projectile()
{
	//delete this->prjID;
	
}

void projectile::Init(sprite* SpritePtr, Game_Engine* worldpointer)
{
	this->OwnerPtr = SpritePtr;
	ptrWorld = worldpointer;

	//position.x = 0.0f + (rand() % (int)this->ptrWorld->Map->WorldInfo.width);
	//position.y = 0.0f + (rand() % (int)this->ptrWorld->Map->WorldInfo.height);

	
	angle.max = this->type->angle.max; // <- THIS FIXED PROJECTILE ANIMATION CRASHING

	velocity = SpritePtr->velocity;
	
	targetvector = 0.0f;
	turndirection = 0.0f; //dynamic var
	turnspeed = this->type->turnspeed;
	goforce = this->type->goforce;
	stopforce = this->type->stopforce;
	acceleration = 0.0f;

	cruisespeed = this->type->cruisespeed;
	maxforcespeed = this->type->maxforcespeed;

	dmg_radius = this->type->dmg_radius;
	dmg_amount = this->type->dmg_amount;

	this->typeID = -1;

	//this->animation_list.Init(this->prjID);
	this->animation_list.Init(((phys_obj* )this->prjID));
}

void projectile::Render()
{

	if (this->animation_list.active) {
		this->animation_list.Animate();
	}


	
	float temp = (atan(1.0f)*8) / angle.max;
	float temp2 = this->vector / temp;

	
	//MOVE THIS TO THE ANIMATION DATA
	//int curFrame = 0;//ResID->animData[this->animation_list.curAnimID].xStart+(this->animation_list.curAnimFrame*ResID->resInfo.xTileSize);

	temp2 += (temp/2); // offset the angle by half a frame, more accurate visual
	angle.frame = (int)(temp2+0.5f); // this actually rounds it up or down (c++ method)

	if (angle.frame > angle.max) { angle.frame--; }
	if (angle.frame < 0) { angle.frame = angle.max; }

	SDL_Rect pClip;

	//int fdgfdg = this->ResID->animData->
	SDL_Rect frameClip = this->ResID->stateToAnim.at(this->animation_list.curAnimID)->clipArray[this->animation_list.curAnimFrame].angles[angle.frame].clip;
	//SDL_Rect frameClip = this->ResID->animData[this->animation_list.curAnimID].clipArray[this->animation_list.curAnimFrame].angles[angle.frame].clip;
	/*frameClip.x = curFrame;
	frameClip.y = angle.frame*ResID->resInfo.yTileSize;
	frameClip.w = ResID->resInfo.xTileSize;//ResID->resInfo.width;
	frameClip.h = ResID->resInfo.xTileSize;//ResID->resInfo.height;*/

	/*SDL_Rect frameClip;
	frameClip.x = 0;
	frameClip.y = 0;
	frameClip.w = ResID->surface->w;//ResID->resInfo.width;
	frameClip.h = ResID->surface->h;//ResID->resInfo.height;
*/

	int xOffset;
	int yOffset;
	xOffset = position.x + this->ResID->stateToAnim.at(this->animation_list.curAnimID)->clipArray[this->animation_list.curAnimFrame].angles[angle.frame].xOffset;
	yOffset = position.y + this->ResID->stateToAnim.at(this->animation_list.curAnimID)->clipArray[this->animation_list.curAnimFrame].angles[angle.frame].yOffset;

	xOffset -= (this->ResID->resInfo.xTileSize/2);
	yOffset -= (this->ResID->resInfo.yTileSize/2);



	if (ptrWorld->Map->inViewport(xOffset,yOffset,frameClip.w,frameClip.h) || 0) {
		//this->ptrWorld->unitsVisible++;
		pClip = ptrWorld->Map->clipToViewport(xOffset,yOffset,frameClip);

		xOffset = (pClip.x - frameClip.x) + ptrWorld->Map->ViewportInfo.gui_x + xOffset - ptrWorld->Map->ViewportInfo.x;
		yOffset = (pClip.y - frameClip.y) + ptrWorld->Map->ViewportInfo.gui_y + yOffset - ptrWorld->Map->ViewportInfo.y;
		
		
		ptrWorld->ptrScreen->apply_surface(xOffset, yOffset, ResID->surface, ptrWorld->ptrScreen->screen, &pClip );
	}

	//Render Text
	if (this->ID == -1) {
		sstr.str("");
		//sstr.str("speed: ");
		sstr<<this->animation_list.curAnimFrame<<" - "<<this->angle.frame;
		str1 = sstr.str();
		ptrWorld->ptrScreen->printText(10,10,str1.c_str());
	}
	//message = TTF_RenderText_Solid( font, str1.c_str(), *textColor );
	// |
	//pt2apply_surface(Units[i].position.x+40, Units[i].position.y+10, message, screen, NULL );


}