#ifndef RESOURCE_H
#define RESOURCE_H

#include "glSDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <windows.h>

#include <iostream>
#include <sstream>
#include <string>

const int RES_TYPE_TILEMAP = 0;
const int RES_TYPE_FILEMAP = 1;
const int RES_TYPE_GRPFILE = 2;

const int DATAFILE_SPRITE_PHYSDATA = 0;
const int DATAFILE_SPRITE_UNITDATA = 1;
const int DATAFILE_SPRITE_ANIMDATA = 2;
const int DATAFILE_SPRITE_ = 3;

struct Resource_Info {
	std::string type; //kinda not used
	
	int width;
	int height;

	int xTileSize;
	int yTileSize;

	int imgCount;

	int imgFormat;

	int alpha_r;
	int alpha_g;
	int alpha_b;
};




//Class for each animation
struct angle_Array {
	SDL_Rect			clip;
	long				xOffset;
	long				yOffset;
};

struct frame_Array {
	angle_Array*		angles;
};

struct animation_Array {
	frame_Array*		frames;
};

class animation_Data {
public:

	animation_Data();

	void Init(long framesMax, long anglesMax);
	void SetRect(long frame, long angle, SDL_Rect clip);

	int ID;
	
	int frames;
	int angles;

	long execTime;

	//SDL_Rect*			animArray; //[][][]
	frame_Array*		clipArray;

	//SDL_Rect**			frameArray;
	//SDL_Rect*			angleArray;

	std::stringstream sstr;
	std::string	str1;
	
};









struct dataFile_Types {
	
	int type;
	
	int fileValueCount;

};

class ResourceObject
{
	
public:
	
	
	ResourceObject();
	~ResourceObject();

	//Functions
	void			Init(int type, std::string resType, std::string resObject);
	void			LoadTextures();
	void			FreeData();
	SDL_Surface		*load_image(std::string filename, const Uint8 rCKey,const Uint8 gCKey,const Uint8 bCKey);
	
	void			xFlip(SDL_Surface* src, SDL_Surface* dest);
	void			invert_mask(SDL_Surface* src, SDL_Surface* dest);

	SDL_Rect		getVisualSize(SDL_Surface* surface, const Uint8 rCKey,const Uint8 gCKey,const Uint8 bCKey);
	Uint32			getpixel(SDL_Surface *surface, int x, int y);

	void			lsdir(const char *path, std::vector<std::string> &results);

	//Variables
	long			resID;
	ResourceObject* NextObj;

	bool			isLoaded;
	SDL_Surface*	surface;

	SDL_Surface*	test;
	bool			testused;

	int*			texID;

	Resource_Info	resInfo;

	std::stringstream sstr;
	std::string	str1;

	animation_Data*						animData; //[]
	std::vector<animation_Data*>		stateToAnim;

	dataFile_Types*		dataTypes;
};

#endif