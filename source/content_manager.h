#ifndef CONTENT_MANAGER
#define CONTENT_MANAGER

//#pragma message("TOP CONTENT_MANAGER")

#include "resource.h"
#include "animation.h"

//#include "phys_obj.h"

#include "sprite.h"
#include "projectile.h"



//class sprite;
//class projectile;
//class phys_obj;
class Game_Engine;

//#include "inc/ini_manage.h"

class Content_Object {
public:
	//ResourceObject			*ResID;

	long					ID;
	long					physType;

	std::string				name;
	
	std::stringstream		sstr;
	std::string				str1;
};

class map_info : public Content_Object {
	public:
	//this class might be depreciated because of mappy
};

class prj_info : public Content_Object, public projectile {
public:
};

class sprite_info : public Content_Object, public sprite {
public:
	

};

class Content_Manager {
public:
	Content_Manager() {};
	~Content_Manager() {};

	bool lsdir(const char *path, std::vector<std::string> &results);

	//start getting a listing of files in phys / sprite / prj / map?

	//open each folder inside those then get .dat data, resources

	bool loadContent();

	bool loadSprite(std::string file, sprite_info* ptrObj);
	bool loadPrj(std::string file, prj_info* ptrObj);

	sprite_info* LoadUnitName(std::string name);
	prj_info* LoadPrjName(std::string name);

	sprite_info* LoadUnitID(long ID);
	long UnitNameToID(std::string name);

	Resource_List					Resources;
	
	std::vector<sprite_info*>		UnitTypes;
	sprite_info*					TempUnit;

	std::vector<prj_info*>			PrjTypes;
	prj_info*						TempPrj;

	Game_Engine*					ptrWorld;

	std::stringstream sstr;
	
	std::string	str1;
	std::string	cat;

	

};

#endif