#ifndef LOADSCREEN_H
#define LOADSCREEN_H

//#include "stdio.h"
//#include "stdlib.h"
#include <string>
#include <iostream>
#include <sstream>



#include "SDL_ttf.h"
#include <vector>
#include <map>

#include "screen.h"


class Game_Engine;
//class Screen;

struct colorData {
	int r;
	int g;
	int b;
};

struct loadData {

	
	std::string		caption;

	long				posX;
	long				posY;
	colorData			color;

	long			curValue;
	long			maxValue;

};



class loadScreen {
	
public:

	loadScreen(){};
	~loadScreen(){};

	void Init(Game_Engine* worldpointer);
	void Update(std::string type,long val);
	void Render(std::string type);
	

	long				posX;
	long				posY;
	long				width;
	long				height;

	long				barSize;
	

	loadData			loadMap;
	loadData			loadUnits;
	loadData			loadAI;

	std::map<std::string,loadData *> section;


	Game_Engine*		ptrWorld;
	//Screen*				ptrScreen;

	std::string			caption;
	std::stringstream	sstr;

};



#endif