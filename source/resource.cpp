#include "resource.h"
#include <string>
#include <math.h>
#include <fstream>
#include "states.h"



//#include "grpapi.h"

animation_Data::animation_Data() {
	this->ID = -1;
}

void animation_Data::Init(long framesMax, long anglesMax) {
	
	/*
	//typedef SDL_Rect (*ObjectArray)[1][1][1];
	//ObjectArray animArray = (ObjectArray) new SDL_Rect[clips][framesMax][anglesMax];
	frameArray = &new SDL_Rect[framesMax];
	for (int j = 0; j < framesMax; j++) {
		angleArray = new SDL_Rect[anglesMax];
		for (int k = 0; k < anglesMax; k++) {
			//angleArray[k] = new SDL_Rect;
			angleArray[k].x = 0; angleArray[k].y = 0; angleArray[k].w = 0; angleArray[k].h = 9; 
		}
		(*(frameArray+j)) = angleArray;
		//frameArray = angleArray;
	}
*/
	return;

	/*int *lol3 = NULL;
		lol3 = new int[clips];
		lol3[0] = 5;
		lol3[1] = 10;
		int **lol2 = NULL;
		lol2 = &new int[clips];
		//int lol[2][2];
		lol2[0] = lol3;
		lol2[1] = lol32;*/
	//(*(animArray+j)) = frameArray;
	//for (int i = 0; i < clips; i++) {

		//animData[i].animArray = new SDL_Rect[framesMax][anglesMax];

		
		//animData[i].

		
	//}
	//int fsdd = 0;
	//return;
}

void animation_Data::SetRect(long frame, long angle, SDL_Rect clip) {
//	(*(frameArray))[angle].x = clip.x;
		//frameArray[frame][angle] = clip;
}

void ResourceObject::Init(int type, std::string resType, std::string resObject)
{

	/*dataTypes = new dataFile_Types[4];
	dataTypes[DATAFILE_SPRITE_PHYSDATA].fileValueCount = 7;
	dataTypes[DATAFILE_SPRITE_UNITDATA].fileValueCount = 10;
	dataTypes[DATAFILE_SPRITE_ANIMDATA].fileValueCount = 4;*/
	
	this->resInfo.type = resType;

	//this->
	
	// if we're loading a single image containing all animations
	if (type == RES_TYPE_TILEMAP) {
	
		sstr.str("");
		sstr<<"content\\"<<resType<<"\\"<<resObject<<"\\tileset.bmp";
		str1 = sstr.str();

		surface = load_image(str1,this->resInfo.alpha_r,this->resInfo.alpha_g,this->resInfo.alpha_b);

		this->resInfo.width = surface->w;
		this->resInfo.height = surface->h;
		this->resInfo.xTileSize = 32;
		this->resInfo.yTileSize = 32;

		long clips, framesMax, anglesMax;
		clips = 1;
		framesMax = 1;
		anglesMax = 1;

		animData = new animation_Data[clips];
		stateToAnim.resize(states::COUNT_ANIM_STATE);

		

		animData[0].clipArray = new frame_Array[framesMax];

			
		animData[0].clipArray[0].angles = new angle_Array[anglesMax];

		animData[0].clipArray[0].angles[0].clip.x = 0;
		animData[0].clipArray[0].angles[0].clip.y = 0;
		animData[0].clipArray[0].angles[0].clip.w = surface->w;
		animData[0].clipArray[0].angles[0].clip.h = surface->h;


		stateToAnim.at(0) = &animData[0];
		stateToAnim.at(1) = &animData[0];
		stateToAnim.at(2) = &animData[0];
		stateToAnim.at(10) = &animData[0];



	}
	//If we're loading a series of files to merge to one
	else if (type == RES_TYPE_FILEMAP) {
		sstr.str("");
		sstr<<"content\\"<<resType<<"\\"<<resObject<<"\\animdata.dat";
		str1 = sstr.str();

		std::ifstream animFile( str1.c_str() );

		if( animFile == NULL )
		{
			return;
		}

		long clips, framesMax, anglesMax;
		long resWidth = 0, resHeight = 0, totalFrameCount = 0;

		animFile >> clips;
		animFile >> framesMax;
		animFile >> anglesMax;

		animation_Data* animDataTemp = new animation_Data;

		animData = new animation_Data[clips];
		//animData = new animation_Data[clips+2];
		stateToAnim.resize(states::COUNT_ANIM_STATE);
		std::vector<animation_Data *>::iterator itaD = stateToAnim.begin();
		
		//itaD = stateToAnim.begin();
		//itaD+=0;
		animDataTemp->ID = -1;
		//stateToAnim.insert(itaD,animDataTemp);
		//stateToAnim.insert(itaD,animDataTemp);
		stateToAnim.at(0) = animDataTemp;
		stateToAnim.at(2) = animDataTemp;
		stateToAnim.at(10) = animDataTemp;

		itaD = stateToAnim.begin();
		

		//stateToAnim = new animation_Data[COUNT_ANIM_STATE];

		SDL_Rect biggestClip;

		animFile >> biggestClip.x;
		animFile >> biggestClip.y;
		animFile >> biggestClip.w;
		animFile >> biggestClip.h;
		int i;
		for (i = 0; i < clips; i++) {
			animFile >> animData[i].ID;
			animFile >> animData[i].frames;
			animFile >> animData[i].angles;
			animFile >> animData[i].execTime;
			animData[i].clipArray = new frame_Array[framesMax];

			for (int k = 0; k < framesMax; k++) {
				animData[i].clipArray[k].angles = new angle_Array[anglesMax];
				for (int l = 0; l < anglesMax; l++) {
					
					animData[i].clipArray[k].angles[l].clip.x = 0;
					animData[i].clipArray[k].angles[l].clip.y = 0;
					animData[i].clipArray[k].angles[l].clip.w = 0;
					animData[i].clipArray[k].angles[l].clip.h = 0;
				}
			}
			if (animData[i].angles == 0) {
				resWidth += (1 * biggestClip.w);
				totalFrameCount += animData[i].frames;
			} else {
				resWidth += (animData[i].frames * biggestClip.w);
				totalFrameCount += animData[i].frames * anglesMax;
			}

			//animData[i].Init(framesMax,anglesMax);

			//itaD = stateToAnim.begin();
			//itaD+=animData[i].ID;
			//stateToAnim.insert(itaD,&animData[i]);
			stateToAnim.at(animData[i].ID) = &animData[i];

			if (animData[i].ID == 1) {
				if (stateToAnim.at(0)->ID == -1) {
					stateToAnim.at(0) = &animData[i];
				}
				if (stateToAnim.at(2)->ID == -1) {
					stateToAnim.at(2) = &animData[i];
				}
				if (stateToAnim.at(10)->ID == -1) {
					stateToAnim.at(10) = &animData[i];
				}

			}

			//update render and animate code to use this lookup vector, also update ai states to anim code
			
		}

		if (stateToAnim.at(0)->ID < 0) {
			int asdasd = stateToAnim.at(0)->ID;
			int sdf = 0;
		}

		//SDL_Rect lol = animData[4].frameArray[0][0];
		//animData[4].frameArray[0][0] = lol;
		
		resHeight = anglesMax * biggestClip.h;

		if (animFile.fail()) {
			//ERROR
			animFile.close();
		}
		
		animFile.close();

		this->surface = SDL_CreateRGBSurface(SDL_HWSURFACE,resWidth,resHeight,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
		
		SDL_Surface* image = new SDL_Surface;
		SDL_Surface* mask = new SDL_Surface;
		SDL_Surface* temp = new SDL_Surface;
		SDL_Surface* temp2 = new SDL_Surface;

		SDL_Rect offset;
		SDL_Rect clip;

		//Flip vars
		int yOffset;
		int yOffsetFlipped = 16;
		bool bFlip;
		int maxWidth = 0;
		int maxHeight = 0;

		int clipNum = 0;
		int frameNum = 0;
		int angleNum = 0;

		int xTileStart = 0;

		SDL_Rect tPtr;
		
		
		//biggestClip.y = 99;						//REMOVE MEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE


		//animData[4].frameArray[0][0] = lol;
		for (i = 0; i < totalFrameCount; i++) {

			float yTemp = (fmod(i*1.0f,anglesMax*1.0f));

			bFlip = false;
			if (yTemp == 0) {
				yOffsetFlipped = 15;
				yOffset = 0;
			}
			if (fmod(yTemp,2) == 1) {
				if (yTemp != 1) {
					angleNum = yOffsetFlipped;
					
					yOffsetFlipped--;
					bFlip = true;
				}
			} else {
				angleNum = yOffset;
				
				yOffset++;
			}


			//this code determines where the next blit should be placed
			offset.y = angleNum*biggestClip.h;
			offset.x = ((int)(i/anglesMax))*biggestClip.w;
			offset.w = biggestClip.w;
			offset.h = biggestClip.h;
			
			std::string sZeroes;
			sstr.str("");

			
			if (totalFrameCount < 10) {
				
				/*if (i < 10) {
					sstr<<"00";
				} else if (i < 100) {*/
					sstr<<"";
				//}
			} else if (totalFrameCount < 100) {	
				if (i < 10) {
					sstr<<"0";
				} else if (i < 100) {
					sstr<<"";
				}
			} else {
				
				
				if (i < 10) {
					sstr<<"00";
				} else if (i < 100) {
					sstr<<"0";
				}
			}

			/*sstr.str("");
			sstr<<resType<<"\\"<<resObject<<"\\img\\";
			str1 = sstr.str();

			std::vector<std::string> results;
			this->lsdir(str1.c_str(),results);*/

			//changed for vulture
			

			sZeroes = sstr.str();
			//lol = animData[4].frameArray[0][0];
			//animData[4].frameArray[0][0] = animData[4].frameArray[0][1];
			sstr.str("");
			sstr<<"content\\"<<resType<<"\\"<<resObject<<"\\img\\"<<resObject<<" "<<sZeroes<<i<<".bmp";
			str1 = sstr.str();
			image = load_image(str1,this->resInfo.alpha_r,this->resInfo.alpha_g,this->resInfo.alpha_b);
			//this->LoadTextures();
			//animData[4].angles = 5;
			//lol = animData[4].frameArray[0][1];
			
			clip.x = 0;
			clip.y = 0;
			clip.w = image->w;
			clip.h = image->h;
			//animData[4].clipArray[0].angles[0].clip = &clip;


			//image = SDL_CreateRGBSurface(SDL_HWSURFACE,128,128,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
			//mask = SDL_CreateRGBSurface(SDL_HWSURFACE,128,128,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
			temp = SDL_CreateRGBSurface(SDL_HWSURFACE,clip.w,clip.h,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
			temp2 = SDL_CreateRGBSurface(SDL_HWSURFACE,clip.w,clip.h,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
			this->test = SDL_CreateRGBSurface(SDL_HWSURFACE,clip.w,clip.h,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
			//temp = SDL_DisplayFormat(temp);
	
			if (bFlip) {
				SDL_BlitSurface(image,&image->clip_rect,temp,&temp->clip_rect);
				SDL_FillRect( image, &image->clip_rect, SDL_MapRGB( image->format,this->resInfo.alpha_r,this->resInfo.alpha_g,this->resInfo.alpha_b ) );
				SDL_SetColorKey( image, SDL_SRCCOLORKEY, SDL_MapRGB( image->format,this->resInfo.alpha_r,this->resInfo.alpha_g,this->resInfo.alpha_b ) );
				xFlip(temp,image);
			}

			sstr.str("");
			sstr<<"content\\"<<resType<<"\\"<<resObject<<"\\img\\"<<resObject<<"_mask "<<sZeroes<<i<<".bmp";
			str1 = sstr.str();
			mask = load_image(str1,this->resInfo.alpha_r,this->resInfo.alpha_g,this->resInfo.alpha_b);

			//new
			
			SDL_FillRect( temp2, &temp2->clip_rect, SDL_MapRGB( temp2->format,this->resInfo.alpha_r,this->resInfo.alpha_g,this->resInfo.alpha_b ) );

			SDL_BlitSurface(mask,&mask->clip_rect,temp2,&temp2->clip_rect);

			SDL_FillRect( mask, &mask->clip_rect, SDL_MapRGB( mask->format, 255, 255, 255 ) );
			//SDL_SetColorKey( temp2, SDL_SRCCOLORKEY, SDL_MapRGB( temp2->format, 255, 255, 255 ) );

			//SDL_FillRect( test, &test->clip_rect, SDL_MapRGB( test->format, 255, 255, 255 ) );
			SDL_SetColorKey( mask, SDL_SRCCOLORKEY, SDL_MapRGB( mask->format, 255, 255, 255 ) );
			
			this->invert_mask(temp2,mask);
			SDL_BlitSurface(mask,&mask->clip_rect,test,&test->clip_rect);

			if (bFlip) {
				SDL_BlitSurface(mask,&mask->clip_rect,temp,&temp->clip_rect);
				SDL_FillRect( mask, &mask->clip_rect, SDL_MapRGB( mask->format, 255, 255, 255 ) );
				SDL_SetColorKey( mask, SDL_SRCCOLORKEY, SDL_MapRGB( mask->format, 255, 255, 255 ) );
				xFlip(temp,mask);
			}

			

			//all this converts the black background to rgb 0 255 255 so its the new transparent color
			//without messing up the fact that black is used in these images normally
			SDL_FillRect( temp, &temp->clip_rect, SDL_MapRGB( temp->format, 0, 255, 255 ) );
			SDL_BlitSurface(mask,&mask->clip_rect,temp,&temp->clip_rect);
			SDL_BlitSurface(image,&image->clip_rect,temp,&temp->clip_rect);

			

			int xTile = ((int)(i/anglesMax));
			int yTile = (fmod(i*1.0f,anglesMax*1.0f));
			
			//int frameNum = 0;
			//angleNum = yTile; // im setup above
			frameNum = xTile-xTileStart;

			//FIX THIS CODE, iterates through clips, i added clipnum++, right?
			//for (int j = 0; j < clips; j++) {
				if (frameNum >= animData[clipNum].frames) {
					frameNum = 0;
					xTileStart = xTile;
					clipNum++;
					int lol = 0;
					//tPtr = animData[clipNum].clipArray[frameNum].angles[angleNum].clip;
				} else {
					//clipNum = j;
					
				}
			//}


			tPtr = this->getVisualSize(temp,0,255,255);

			/*offset.x += biggestClip.x;
			offset.y += biggestClip.y;
			offset.w = biggestClip.w;
			offset.h = biggestClip.h;*/

			SDL_Rect bufferClip = offset;

			bufferClip.x += ((biggestClip.w - tPtr.w)/2);//((tPtr.x - biggestClip.x));// + ((biggestClip.w - tPtr.w)/2);//-((tPtr.w)/2);
			bufferClip.y += ((biggestClip.h - tPtr.h)/2);//((tPtr.y - biggestClip.y));// + ((biggestClip.h - tPtr.h)/2);//-((tPtr.h)/2);  (tPtr.y - biggestClip.y);//-((tPtr.h)/2);
			bufferClip.w = tPtr.w;
			bufferClip.h = tPtr.h;



			//REMOVE ME!
			//SDL_FillRect(surface,&offset,SDL_MapRGB(surface->format,0,(rand() % 255),0));

			if (resObject == "vulture") {
				int asas = 0;
			}
			animData[clipNum].clipArray[frameNum].angles[angleNum].clip = bufferClip; // i think this is crashing cause it goes over clip max, also last frame isnt accounted for right, ling death
			animData[clipNum].clipArray[frameNum].angles[angleNum].xOffset = tPtr.x-biggestClip.x;
			animData[clipNum].clipArray[frameNum].angles[angleNum].yOffset = tPtr.y-biggestClip.y;

			SDL_BlitSurface( temp, &tPtr, surface, &bufferClip );

			//keep this code to get the biggest clip size of other graphics for the data files
			if (tPtr.x < biggestClip.x) {
				biggestClip.x = tPtr.x;
			}
			if (tPtr.w > biggestClip.w) {
				biggestClip.w = tPtr.w;
			}
			if (tPtr.y < biggestClip.y) {
				biggestClip.y = tPtr.y;
			}
			if (tPtr.h > biggestClip.h) {
				biggestClip.h = tPtr.h;
			}
			

		}

		this->resInfo.width = surface->w;
		this->resInfo.height = surface->h;
		this->resInfo.xTileSize = biggestClip.w;
		this->resInfo.yTileSize = biggestClip.h;


		SDL_SetColorKey( surface, SDL_SRCCOLORKEY | SDL_HWSURFACE, SDL_MapRGB( surface->format, 0, 255, 255 ) );
		surface = SDL_DisplayFormat( surface );

		test = SDL_DisplayFormat( test );
		
		
        //Free the old image
        //SDL_FreeSurface( loadedImage );

		


	}
}

ResourceObject::ResourceObject()
{
	this->NextObj = NULL;
}

ResourceObject::~ResourceObject()
{
	FreeData();	
}


void ResourceObject::LoadTextures()
{
	int i = 0;
}

SDL_Surface* ResourceObject::load_image(std::string filename, const Uint8 rCKey = 0,const Uint8 gCKey = 0xFF,const Uint8 bCKey = 0xFF) {
    //Temporary storage for the image that's loaded
    //SDL_Rect lol = animData[4].frameArray[0][0];
	//animData[4].frameArray[0][0] = lol;
	SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

	

    //Load the image
    //loadedImage = SDL_LoadBMP( filename.c_str() );
	loadedImage = IMG_Load( filename.c_str() );

    //If nothing went wrong in loading the image
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );

		//If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, rCKey, gCKey, bCKey ) );
        }
    }

    //Return the optimized image
    return optimizedImage;
}

void ResourceObject::FreeData()
{
	//surface = NULL;
	SDL_FreeSurface(surface);
}



void ResourceObject::xFlip(SDL_Surface *src, SDL_Surface *dest) {
	for (int y = 0; y <= src->clip_rect.h; y++) {
		for (int x = 0; x <= src->clip_rect.w; x++) {
			SDL_Rect cFrom;
			cFrom.x = x;
			cFrom.y = y;
			cFrom.w = 1;
			cFrom.h = 1;

			SDL_Rect cTo;
			cTo.x = src->clip_rect.w - x;
			cTo.y = y;
			cTo.w = 1;
			cTo.h = 1;

			SDL_BlitSurface(src,&cFrom,dest,&cTo);
		}
	}
}

void ResourceObject::invert_mask(SDL_Surface *src, SDL_Surface* dest) {

	SDL_PixelFormat *fmt;
	Uint32 temp, pixel;
	Uint8 red, green, blue, alpha;

	fmt = src->format;
	SDL_LockSurface(src);

	//SDL_FillRect( dest, &dest->clip_rect, SDL_MapRGB( dest->format, 0, 0, 255 ) );

	for (int y = 0; y <= src->clip_rect.h; y++) {
		for (int x = 0; x <= src->clip_rect.w; x++) {

			pixel = this->getpixel(src,x,y);
			SDL_GetRGB(pixel,fmt,&red,&green,&blue);

			if ((red == 0xFF) && (green == 0xFF) && (blue == 0xFF)) {
				SDL_Rect cFrom;
				cFrom.x = x;
				cFrom.y = y;
				cFrom.w = 1;
				cFrom.h = 1;
				SDL_FillRect( dest, &cFrom, SDL_MapRGB( dest->format, 0, 0, 0 ) );
			} else {
				
			}

		}
	}
	SDL_UnlockSurface(src);
}

SDL_Rect ResourceObject::getVisualSize(SDL_Surface* surface, const Uint8 rCKey = 0,const Uint8 gCKey = 0xFF,const Uint8 bCKey = 0xFF) {

	/* Extracting color components from a 32-bit color value */
	SDL_PixelFormat *fmt;
	SDL_Rect spriteSize;
	Uint32 temp, pixel;
	Uint8 red, green, blue, alpha;

	int left_x = surface->w, top_y = surface->h;
	int right_x = 0, bottom_y = 0;
	
	
	

	//surface = this->surface;

	//surface = SDL_CreateRGBSurface(SDL_HWSURFACE,128,128,32,0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
	//SDL_FillRect( surface, &surface->clip_rect, SDL_MapRGB( surface->format, 0, 255, 255 ) );

	fmt = surface->format;
	SDL_LockSurface(surface);
	for (int y = 0; y < surface->clip_rect.h; y++) {
		for (int x = 0; x < surface->clip_rect.w; x++) {
			//pixel = *((Uint32*)surface->pixels);

			pixel = this->getpixel(surface,x,y);
			SDL_GetRGB(pixel,fmt,&red,&green,&blue);

			if (red != rCKey) {
				if (green != gCKey) {
					if (blue != bCKey) {
						int dfg = 0;
						if (x < left_x) {
							left_x = x;
						}
						if (x > right_x) {
							right_x = x;
						}
						if (y < top_y) {
							top_y = y;
						}
						if (y > bottom_y) {
							bottom_y = y;
						}
						
					}
				}
			}

			

		}
	}

	spriteSize.x = left_x;
	spriteSize.y = top_y;
	spriteSize.w = right_x - left_x+1;
	spriteSize.h = bottom_y - top_y+1;

	SDL_UnlockSurface(surface);
	return spriteSize;
	//printf("Pixel Color -> R: %d,  G: %d,  B: %d,  A: %d\n", red, green, blue, alpha);

}

/*
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 ResourceObject::getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    } // switch
}

void ResourceObject::lsdir(const char *path, std::vector<std::string> &results)
{
        WIN32_FIND_DATA fdata;
        HANDLE dhandle;

        // must append \* to the path
        {
                char buf[MAX_PATH];
                sprintf_s(buf, "%s\\*", path);
                if((dhandle = FindFirstFile(buf, &fdata)) == INVALID_HANDLE_VALUE) {
                        //throw Error("FindFirstFile");
                }
        }

        // even an "empty" directory will give two results - . and ..
        //results.append(String(fdata.cFileName));
		results.push_back(fdata.cFileName);

        while(1) {
                if(FindNextFile(dhandle, &fdata)) {
					results.push_back(fdata.cFileName);
                        //results.append(String(fdata.cFileName));
                } else {
                        if(GetLastError() == ERROR_NO_MORE_FILES) {
                                break;
                        } else {
                                FindClose(dhandle);
                                //throw Error("FindNextFile");
                        }
                }
        }

        if(FindClose(dhandle) == 0) {
                //throw Error("FindClose");
        }
} 
