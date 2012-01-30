#ifndef BREWRESOURCE_H
#define BREWRESOURCE_H

#include "resource.h"

#include <list>

#define f16(X)			            ((int32) ((X) * 65535))
#define ITOX(i)						((i) << 16)

class Resource_List
{
public:
	
	void Init();
	Resource_List(){ objCount = 0; };
	~Resource_List();

	ResourceObject* NewResource(long resID);
	ResourceObject* FindID(long resID);
	ResourceObject* GetPrevious(ResourceObject* resID);

	bool Loaded(char resName);
	void SetupResource(long resID, ResourceObject* resObj);

	//ResourceObject resObjects[1];

	ResourceObject* resObjectsStart;
	ResourceObject* resObjects; //AKA the tail

	long objCount;
	
	// Our application, Shell, and Display
	//AEEApplet*	m_applet;
	//IBitmap*	m_pDDBitmap;	
	//SDL_Surface 
	
	// File I/O
	//IFileMgr*	m_pIFileMgr;

	//member functions
	//void FreeData( void );
	
	//void LoadTextures(IShell *m_pIShell);	
};

#endif