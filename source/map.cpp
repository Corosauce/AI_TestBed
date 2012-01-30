#include "map.h"
#include <fstream>
#include "control.h"
#include <math.h>
#include "player.h"
//#include 

//efficiency debug
const long OcMax = 10;



Tile::Tile() {

	this->OccupyData = new long[OcMax];

};

Tile::Tile(long OccupyMax) {

	this->OccupyData = new long[OccupyMax];

};

void Tile::reset() {
	this->Occupied = false;
	this->OccupyCount = 0;
	for (int i = 0; i < OcMax; i++) {
		this->OccupyData[i] = -2;
	}
}

Tile::~Tile() {


};

/*void Tile::Init( int x, int y, int tileType, int tileSize )
{
    //Get the offsets
    //box.x = x;
    //box.y = y;

    //Set the collision box
    //box.w = tileSize;
    //box.h = tileSize;

    //Get the tile type
    //type = tileType;
}

void Tile::show()
{
    //If the tile is on screen
    if( check_collision( camera, box ) == true )
    {
        //Show the tile
        apply_surface( box.x - camera.x, box.y - camera.y, tileSheet, screen, &clips[ type ] );
    }
}

int Tile::get_type()
{
    return type;
}

SDL_Rect Tile::get_box()
{
    return box;
}*/

map::map()
{
	int a = 1;
	this->OccupyMax = OcMax;
}

map::~map()
{
	
}

void map::Init(Game_Engine* worldpointer, Screen* Screen,float width,float height, float tile_size)
{

	ptrWorld = worldpointer;
	ptrScreen = Screen;
	
	ptrWorld->loader.Update("map",50);

	//ViewportInfo.width = this->ptrScreen->VIEWPORT_WIDTH;
	//ViewportInfo.height = this->ptrScreen->VIEWPORT_HEIGHT;

	ViewportInfo.x = 0; //automatically set based on player position for now
	ViewportInfo.y = 0; // -^
	ViewportInfo.gui_x = 40;
	ViewportInfo.gui_y = 40;

	ViewportInfo.width = width-(ViewportInfo.gui_x*2);
	ViewportInfo.height = height-(ViewportInfo.gui_y*2);

	minimapSizeX = 320;
	minimapSizeY = 240;


	this->viewport = new SDL_Surface;
	this->viewport = SDL_CreateRGBSurface(NULL,this->ViewportInfo.width,this->ViewportInfo.height,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
	this->viewport = SDL_DisplayFormat( this->viewport );

	this->minimap = new SDL_Surface;
	this->minimap = SDL_CreateRGBSurface(NULL,minimapSizeX,minimapSizeY,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
	this->minimap = SDL_DisplayFormat( this->minimap );

	

	/*ViewportInfo.Scale.x = 0.0f;
	ViewportInfo.Scale.y = 0.0f;	
	ViewportInfo.Scale.z = -1.0f * ((WorldInfo.tiles_count_x) - 2.0f);*/

	//float yOffset = (Map->map_data.tiles_count_y / 2.0f)+1.0f;

	//UnitScale.x = (ViewportInfo.tiles_count_x / 2.0f)+2.0f;
	//UnitScale.y = (ViewportInfo.tiles_count_y / 2.0f)+1.5f;

	int mapID = 1;

	sstr.str("");
	//sstr<<"map\\"<<mapID<<"\\test.fmp";
	sstr<<"content\\map\\"<<mapID<<"\\open2.fmp";
	str1 = sstr.str();

	MapSource = new SDLMappy;

	if (MapSource->LoadMap((char*)str1.c_str(),40,40,this->ViewportInfo.width,this->ViewportInfo.height) == -1) {
	//if (Map->LoadMap("map\\0\\smooth.fmp",0,0,400,100) == -1) {
		//map fail
	
	}
	
	/*long temp;
	temp = MapSource->GetMapWidthInBlocks() * MapSource->GetMapHeightInBlocks();
	//tiles = new Tile[WorldInfo.tile_types_count-1];
	int z = this->ResID->surface->h;
	for (int i = 0; i < temp; i++) {
		//tiles[i].Init(this->WorldInfo.tile_size*i,0,i,this->WorldInfo.tile_size);
	}*/
	
	WorldInfo.width = MapSource->GetMapWidth();
	WorldInfo.height = MapSource->GetMapHeight();

	WorldInfo.tile_size = MapSource->GetMapBlockWidth();
	WorldInfo.tiles_count_x = MapSource->GetMapWidth()/MapSource->GetMapBlockWidth();
	WorldInfo.tiles_count_y = MapSource->GetMapHeight()/MapSource->GetMapBlockHeight();
	WorldInfo.total_tiles = WorldInfo.tiles_count_x * WorldInfo.tiles_count_y;

	long temp = WorldInfo.tiles_count_x;
	if (WorldInfo.tiles_count_y > temp) {
		temp = WorldInfo.tiles_count_y;
		this->minimapScale = minimapSizeY / temp;
	} else {
		
		this->minimapScale = minimapSizeX / temp;
	}
	
	//WorldInfo.tile_types_count = 4;

	this->tile_map = new Tile[WorldInfo.total_tiles];
	this->tile_map_temp = new Tile[WorldInfo.total_tiles];
	this->tile_map_swap = tile_map_temp;//new Tile[WorldInfo.total_tiles];
	this->pred_map = new Tile[WorldInfo.total_tiles];
	this->path_map = new Tile[WorldInfo.total_tiles];
	this->wps_map = new Tile[WorldInfo.total_tiles];
	this->wpe_map = new Tile[WorldInfo.total_tiles];
	this->node_map = new Tile[WorldInfo.total_tiles];
	this->waypoint_map = new Tile[WorldInfo.total_tiles];

	/*if (loadMap(str1.c_str()) && 1) {
		this->ResID = this->ptrWorld->Resources.NewResource(2);
		
	}*/

	//set_tiles();

	//For Setting up Scene
	//ViewportScale.x = 0.0f;
	//ViewportScale.y = 0.0f;                                               //
	//ViewportScale.z = 

	//UnitScale.x = ViewportInfo.width/   -(ViewportInfo.Scale.x/2.0f)
	//UnitScale.y = 

	//Get viewscreen size, store in this object

	//Load level:

		//Get player start position

		//Init needed tilemap, based of player position

		//Lock player and viewscreen in current tilemap

		//Setup conditions that will trigger a loading of a new tilemap

	//Start rendering:

		//calc viewscreen x,y based off player position

		//Render needed sets of tiles in current tilemap based on player x,y

		//For player:

			//translate x,y coords for OpenGL

			//Render in middle of screen, offset if near tilemaps edge, so render in middle + (map width - player x)

		//For each NPC:

			//determine if on screen using npc x,y with viewscreen x,y + size

			//render, translate x,y coords to OpenGL 


								
}

void map::RenderWorld() {
	
	this->renderMap();
	//this->renderMiniMap();

	
	//this->renderCollideMap();

	//this->renderPredMap();
#ifdef SHOW_DEBUG_INFO;
	
	this->renderTileMap(this->wps_map, 0x00, 0xAA, 0x00,true, -10);
	this->renderTileMap(this->wpe_map, 0xAA, 0x00, 0x00,true, -10);

	this->renderTileMap(this->pred_map, 0xFF, 0x00, 0x00,false);
	this->renderTileMap(this->waypoint_map, 0x00, 0x00, 0x00,true);

	this->renderTileMap(this->path_map, 0x00, 0x00, 0xAA,true,15);
	
#endif
	

}

void map::renderMap() {
	
	
	

	//SDL_FillRect( ptrScreen->screen, &clip2, SDL_MapRGB( ptrScreen->screen->format, 0x00, 0x00, 0xDD ) );


	MapSource->MapMoveTo(this->ViewportInfo.x,this->ViewportInfo.y);
	MapSource->MapDrawBG(this->viewport);
	this->ptrScreen->apply_surface(this->ViewportInfo.gui_x,this->ViewportInfo.gui_y,this->viewport,this->ptrScreen->screen);
	
	
	

}

void map::renderMiniMap() {
	SDL_FillRect( minimap, &minimap->clip_rect, SDL_MapRGB( ptrScreen->screen->format, 0x00, 0x77, 0x00 ) );
	SDL_Rect clip;

	

	for (int i = 0; i < this->WorldInfo.total_tiles; i++) {

		int yT = (int)(i / this->WorldInfo.tiles_count_x);		
		int xT = fmod(i*1.0f,this->WorldInfo.tiles_count_x*1.0f);

		if (this->MapSource->MapGetBlock(xT,yT)->tl) {
			clip.x = xT*this->minimapScale; ;
			clip.y = yT*this->minimapScale; ;
			clip.w = this->minimapScale; ;
			clip.h = this->minimapScale; ;
			SDL_FillRect( minimap, &clip, SDL_MapRGB( minimap->format, 0xDD, 0xDD, 0xDD ) );

		}
		if (this->ptrWorld->Map->tile_map[i].Occupied == true) {
			
			clip.x = xT*this->minimapScale; ;
			clip.y = yT*this->minimapScale; ;
			clip.w = this->minimapScale; ;
			clip.h = this->minimapScale; ;
			if (this->ptrWorld->Map->tile_map[i].team == 2) {
				SDL_FillRect( minimap, &clip, SDL_MapRGB( minimap->format, 0xDD, 0x00, 0x00 ) );
			} else {
				SDL_FillRect( minimap, &clip, SDL_MapRGB( minimap->format, 0x00, 0x00, 0xDD ) );
			}
		}
	}
	this->ptrScreen->apply_surface(this->ViewportInfo.gui_x,this->ViewportInfo.gui_y,minimap,this->ptrScreen->screen);
}

void map::renderTileMap(Tile* tileMap, const Uint8 r, const Uint8 g, const Uint8 b, bool showText, int xoffset) {
	SDL_Rect clip2;
	SDL_Rect clip3;
	/*clip2.x = this->ViewportInfo.gui_x;
	clip2.y = this->ViewportInfo.gui_y;
	clip2.w = this->ptrScreen->VIEWPORT_WIDTH;
	clip2.h = this->ptrScreen->VIEWPORT_HEIGHT;*/
	
	


	for (int i = 0; i < this->WorldInfo.total_tiles; i++) {
		if (tileMap[i].Occupied == true) {
			//if (this->ptrWorld->Map->path_map[i].Occupied == true) {
			int yT = (int)(i / this->WorldInfo.tiles_count_x);		
			int xT = fmod(i*1.0f,this->WorldInfo.tiles_count_x*1.0f);

			int xP = xT*this->WorldInfo.tile_size;
			int yP = yT*this->WorldInfo.tile_size;

			
			if (this->inViewport(xP,yP,this->WorldInfo.tile_size,this->WorldInfo.tile_size)) {

				clip2.x = xP;
				clip2.y = yP;
				clip2.w = 32;
				clip2.h = 32;

				//clip2 = this->clipToViewport(xP,yP,clip2);

				int xViewOffset = this->ViewportInfo.gui_x + (xP - this->ViewportInfo.x);
				int yViewOffset = this->ViewportInfo.gui_y + ((yP) - this->ViewportInfo.y);

				clip3.x = xViewOffset+xoffset;
				clip3.y = yViewOffset;
				clip3.w = 32;
				clip3.h = 32;

				

				
				SDL_FillRect( ptrScreen->screen, &clip3, SDL_MapRGB( ptrScreen->screen->format, r, g, b ) );
				
#ifdef SHOW_DEBUG_TEXT
				if (showText){ 
					sstr.str("");
					//sstr.str("speed: ");
					//sstr<<this->vector<<" - "<<this->targetvector;
					//sstr<<tileMap[i].OccupyData.size();

					//extra if statement for thread safety
					if (tileMap[i].Occupied == true) {
						sstr<<tileMap[i].OccupyData[0];
					}
					//sstr<<this->animation_list.curAnimFrame<<" - "<<this->angle.frame;
					str1 = sstr.str();
					ptrWorld->ptrScreen->printText(xViewOffset+xoffset,yViewOffset,str1.c_str());
				}
#endif

				//this->ptrScreen->apply_surface(xViewOffset,yViewOffset,
				//this->ResID->surface,this->ptrScreen->screen,&clip);

			}
		}
	}
}

void map::renderCollideMap() {
	
	SDL_Rect clip2;
	SDL_Rect clip3;
	/*clip2.x = this->ViewportInfo.gui_x;
	clip2.y = this->ViewportInfo.gui_y;
	clip2.w = this->ptrScreen->VIEWPORT_WIDTH;
	clip2.h = this->ptrScreen->VIEWPORT_HEIGHT;*/
	
	


	for (int i = 0; i < this->WorldInfo.total_tiles; i++) {
		if (this->ptrWorld->Map->tile_map[i].Occupied == true) {
			int yT = (int)(i / this->WorldInfo.tiles_count_x);		
			int xT = fmod(i*1.0f,this->WorldInfo.tiles_count_x*1.0f);

			int xP = xT*this->WorldInfo.tile_size;
			int yP = yT*this->WorldInfo.tile_size;

			
			if (this->inViewport(xP,yP,this->WorldInfo.tile_size,this->WorldInfo.tile_size)) {

				clip2.x = xP;
				clip2.y = yP;
				clip2.w = 32;
				clip2.h = 32;

				//clip2 = this->clipToViewport(xP,yP,clip2);

				int xViewOffset = this->ViewportInfo.gui_x + (xP - this->ViewportInfo.x);
				int yViewOffset = this->ViewportInfo.gui_y + ((yP) - this->ViewportInfo.y);

				clip3.x = xViewOffset;
				clip3.y = yViewOffset;
				clip3.w = 32;
				clip3.h = 32;

				

				
				SDL_FillRect( ptrScreen->screen, &clip3, SDL_MapRGB( ptrScreen->screen->format, 0x00, 0x00, 0xDD ) );
					
				

				//this->ptrScreen->apply_surface(xViewOffset,yViewOffset,
				//this->ResID->surface,this->ptrScreen->screen,&clip);

			}
		}
	}

	
}

void map::renderPredMap() {
	
	SDL_Rect clip2;
	SDL_Rect clip3;
	/*clip2.x = this->ViewportInfo.gui_x;
	clip2.y = this->ViewportInfo.gui_y;
	clip2.w = this->ptrScreen->VIEWPORT_WIDTH;
	clip2.h = this->ptrScreen->VIEWPORT_HEIGHT;*/

	for (int i = 0; i < this->WorldInfo.total_tiles; i++) {
		if (this->ptrWorld->Map->pred_map[i].Occupied == true) {
			int yT = (int)(i / this->WorldInfo.tiles_count_x);		
			int xT = fmod(i*1.0f,this->WorldInfo.tiles_count_x*1.0f);

			int xP = xT*this->WorldInfo.tile_size;
			int yP = yT*this->WorldInfo.tile_size;

			
			if (this->inViewport(xP,yP,this->WorldInfo.tile_size,this->WorldInfo.tile_size)) {

				clip2.x = xP;
				clip2.y = yP;
				clip2.w = 32;
				clip2.h = 32;

				//clip2 = this->clipToViewport(xP,yP,clip2);

				int xViewOffset = this->ViewportInfo.gui_x + (xP - this->ViewportInfo.x);
				int yViewOffset = this->ViewportInfo.gui_y + ((yP) - this->ViewportInfo.y);

				clip3.x = xViewOffset;
				clip3.y = yViewOffset;
				clip3.w = 32;
				clip3.h = 32;

				SDL_FillRect( ptrScreen->screen, &clip3, SDL_MapRGB( ptrScreen->screen->format, 0xDD, 0x00, 0x00 ) );
				
				//this->ptrScreen->apply_surface(xViewOffset,yViewOffset,
				//this->ResID->surface,this->ptrScreen->screen,&clip);

			}
		}
	}

	
}

bool map::inViewport(int x, int y, int w, int h) {
	//If the tile is within at least 1 tiles worth of the viewport
	if (x >= this->ViewportInfo.x-w) {
		if (x < this->ViewportInfo.x+this->ViewportInfo.width) {
			if (y >= this->ViewportInfo.y-h) {
				if (y < this->ViewportInfo.y+this->ViewportInfo.height) {

					return true;
				}
			}
		}
	}
	return false;
}

SDL_Rect map::clipToViewport(int x, int y, SDL_Rect tclip) {

	int xClip = 0;
	int yClip = 0;
	int wClip = 0;
	int hClip = 0;

	//SDL_Rect tclip;
	//tclip.x = 0;
	//tclip.y = 0;
	//tclip.w = w;
	//tclip.h = h;

	//If this tile is clipping left side x, else right side x
	if (x < this->ViewportInfo.x) {
		xClip = this->ViewportInfo.x - x;
		tclip.x += xClip;
		tclip.w -= xClip;
		
	} else if (x+tclip.w > this->ViewportInfo.x+this->ViewportInfo.width) {
		wClip = (x+tclip.w) - (this->ViewportInfo.x+this->ViewportInfo.width);
		tclip.w -= wClip;
	}

	//If this tile is clipping top side y, else bottom side y
	if (y < this->ViewportInfo.y) {
		yClip = this->ViewportInfo.y - y;
		tclip.y += yClip;
		tclip.h -= yClip;
	} else if (y+tclip.h > this->ViewportInfo.y+this->ViewportInfo.height) {
		hClip = (y+tclip.h) - (this->ViewportInfo.y+this->ViewportInfo.height);
		tclip.h -= hClip;
	}

	return tclip;
	
}