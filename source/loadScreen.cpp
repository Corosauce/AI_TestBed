#include "loadScreen.h"

#include "control.h"

//#include "functions.h"



void loadScreen::Init(Game_Engine* worldpointer) {

	this->ptrWorld = worldpointer;


	this->loadAI.caption = "AI Precache";
	this->loadUnits.caption = "Units Data";
	this->loadMap.caption = "Map Data";

	this->loadAI.maxValue = 100;
	this->loadMap.maxValue = 100;
	this->loadUnits.maxValue = 10;

	this->loadAI.curValue = 0;
	this->loadMap.curValue = 0;
	this->loadUnits.curValue = 0;

	this->loadMap.posY = 60;
	this->loadUnits.posY = 120;
	this->loadAI.posY = 180;

	this->loadMap.color.r = 0; this->loadMap.color.g = 128; this->loadMap.color.b = 0;
	this->loadUnits.color.r = 0; this->loadUnits.color.g = 0; this->loadUnits.color.b = 128;
	this->loadAI.color.r = 128; this->loadAI.color.g = 0; this->loadAI.color.b = 0;
	
	this->caption = "Loading...";

	this->section["ai"] = &loadAI;
	this->section["units"] = &loadUnits;
	this->section["map"] = &loadMap;

	//assuming a loading screen size
	this->width = 600;
	this->height = 600;
	this->posX = (ptrWorld->worldWidth/2)-(this->width/2);
	this->posY = (ptrWorld->worldHeight/2)-(this->height/2);

	this->barSize = 500;

}

void loadScreen::Update(std::string type, long val) {

	//Update current loading bar
	section[type]->curValue = val;

	SDL_Rect scrClip;

	scrClip.x = posX;
	scrClip.y = posY;
	scrClip.w = this->width;
	scrClip.h = this->height;

	SDL_FillRect(this->ptrWorld->ptrScreen->screen,&scrClip,SDL_MapRGB( this->ptrWorld->ptrScreen->screen->format, 55, 55, 55 ));
	ptrWorld->ptrScreen->printText(posX+(this->width/2)-40,posY+30,this->caption.c_str());
	
	this->Render("map");
	this->Render("units");
	this->Render("ai");

	SDL_Delay(1);
}

void loadScreen::Render(std::string type) {

	float		temp;
	float		temp2;
	float		temp3;
	long		barY;
	SDL_Rect	barClip;

	barY = section[type]->posY;
	temp2 = section[type]->curValue;
	temp3 = section[type]->maxValue;
	temp = (temp2 / temp3)*1.0f;

	barClip.x = posX+50;
	barClip.y = posY+barY;
	barClip.w = temp*this->barSize;
	barClip.h = 40;

	SDL_FillRect(this->ptrWorld->ptrScreen->screen,&barClip,SDL_MapRGB( this->ptrWorld->ptrScreen->screen->format, section[type]->color.r, section[type]->color.g, section[type]->color.b ));

	SDL_Flip(this->ptrWorld->ptrScreen->screen);

}

//void loadS