#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "map.h"
//
#include "control.h"

#include "animation.h"

#include "phys_obj.h"

class prj_info;

class projectile : public phys_obj {

	public:

	projectile();
	~projectile();

	void Init (sprite* SpritePtr, Game_Engine* worldpointer);
	void Render();

	projectile*				prjID;
	sprite*					OwnerPtr;

	long					dmg_radius;
	long					dmg_amount;

	prj_info				*type;

	std::stringstream		sstr;
	std::string				str1;

	sprite_animations		animation_list;





};

#endif