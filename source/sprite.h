#ifndef SPRITE_H
#define SPRITE_H
//#pragma message("TOP SPRITE")
//#include "image.h"
//#include "resource.h"

#include "map.h"
//#include "content_manager.h"
#include "control.h"

#include "animation.h"

#include "phys_obj.h"

/*struct angle_info {
	int frame;
	int max;
};

struct target_info {
	int ID;

	double distance;
	int x;
	int y;

};

struct pos_info {
	float x;
	float y;
};*/

struct weapon_info {

	std::string name;

	int range;
	long reloadtime;
	int maxammo;

	long lastshottime;
	int ammo;

	float angle_cur;
	float angle_target;

};

struct item_info {
	int range;
	int maxcount;
	int count;
};


/*class ResourceObject {
	public:
	int width;
	int height;
};*/

class sprite_info;

class sprite : public phys_obj {
	
	public:
	sprite ();
	~sprite ();

	sprite*			SprID;
	//Game_Engine*	ptrWorld;

	void Init (sprite* SpritePtr, Game_Engine* worldpointer);
	void Process();
	void Render();
	void RenderOutline();

	void Move(float x, float y); //Relative
	void Warp(float x, float y); //Absolute

	void Rotate(float d);

	void Shoot(float vector);

	
	

	
	
	

	/*float cruisespeed;
	float maxforcespeed;

	float stopforce;
	float goforce;
	float velocity;
	float acceleration;
	
	float vector;

	int width;
	int height;

	pos_info				position;
	target_info				target;

	angle_info				angle;*/

	weapon_info				weapons[1];
	item_info				items[1];

	sprite_info				*type;
	//ResourceObject			*ResID;

	sprite_animations		animation_list;

	std::stringstream		sstr;
	std::string				str1;

	

	

};


#endif