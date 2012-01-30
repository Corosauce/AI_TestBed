#ifndef MAP_H
#define MAP_H

#include "glSDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "screen.h"
#include "resource_manager.h"
//#include "control.h"
#include "mappy/SDLMappy.h"
#include <vector>

class Game_Engine;
class ent_agent;

class TileOccupyInfo {
public:
	long		when;
	ent_agent*	who;

};

class Tile
{
    public:

	Tile();
	Tile(long OccupyMax); // how the hell do i use this this class is init'd in an array
	~Tile();

	void reset();
    //The attributes of the tile
    //SDL_Rect box;

    //The tile type
    //int type;
	//std::vector<TileOccupyInfo *> OccupyData;
	
	//this causes nightmares with multithreading, replaced	
	//std::vector<long> OccupyData;

	//new replacement
	long*	OccupyData;
	long	OccupyCount;

	bool Occupied;

	//kinda useless - team is looked up from id used in OccupyData
	int team;

    //Initializes the variables
    //void Init( int x, int y, int tileType, int tileSize );
	

    //Shows the tile
    //void show();

    //Get the tile type
    //int get_type();

    //Get the collision box
    //SDL_Rect get_box();
};

struct scale {
	float x;
	float y;
	float z;
};

struct viewport_info {
	int width;
	int height;

	int x;
	int y;

	int gui_x;
	int gui_y;
};


struct world_info {
	float width;
	float height;

	float tile_size;
	float tiles_count_x;
	float tiles_count_y;
	int total_tiles;
	int	tile_types_count;

	//scale Scale;
};

class map
{
public:
	
	
	map();
	~map();

	void Init(Game_Engine* worldpointer, Screen* Screen ,float width, float height,float tile_size);

	//bool loadMap(const char* str);

	void RenderWorld();
	void renderMap();
	void renderMiniMap();
	void renderCollideMap();
	void renderPredMap();

	void renderTileMap(Tile* tileMap, const Uint8 r, const Uint8 g, const Uint8 b, bool showText, int xoffset = 0);

	bool		inViewport(int x, int y, int w, int h);
	SDL_Rect	clipToViewport(int x, int y, SDL_Rect tclip);
	
	/*int			width;
	int			height;*/

	//pos_info	viewscreen;

	//tilemap		tileset;

	//PERHAPS dont use any, render directly to main
	//dont use these, seems much slower to not blit
	//directly to main screen surface
	SDL_Surface*	minimap;
	SDL_Surface*	viewport;
	SDL_Surface*	tile_set_image;

	

	viewport_info	ViewportInfo;
	//camera_info		Camera;
	world_info		WorldInfo;


	//Tells the below tiles the max amount of units reported occupied per each tile
	long			OccupyMax;

	//Minimap data
	long			minimapSizeX;
	long			minimapSizeY;
	long			minimapScale;
	
	
					//for loaded tiles
	Tile*			tiles;
					//data for which tiles go where
	Tile*			tile_map;
	Tile*			tile_map_temp;
	Tile*			tile_map_swap;
					//prediction tiles (for debugging?)
	Tile*			pred_map;

					//
	Tile*			path_map;
	Tile*			wps_map;
	Tile*			wpe_map;

	Tile*			node_map;

	Tile*			waypoint_map;

	SDLMappy*		MapSource;


	//scale			ViewportScale;
	//scale			UnitScale;

	Game_Engine*	ptrWorld;
	Screen*			ptrScreen;

	ResourceObject *ResID;


	std::stringstream sstr;
	std::string str1;

	//member functions
	

};









#endif