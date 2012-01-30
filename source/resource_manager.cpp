#include "resource_manager.h"
#include "resource.h"

#define length(a) (sizeof a / sizeof a[0])

void Resource_List::Init()
{
	//m_applet = applet;
}


Resource_List::~Resource_List()
{
	//FreeData();	
}


void Resource_List::SetupResource(long resID, ResourceObject* resObj) {

	
	/*if (resID == 3) {
		//resObj->Init(RES_TYPE_TILEMAP,480,30,30,"Lazer Tank.png");
	} else if (resID == 4) {
		//resObj->Init(RES_TYPE_TILEMAP,480,30,30,"Lazer Tank.bmp");
	}
	else if (resID == 2) {
		resObj->Init(RES_TYPE_TILEMAP,"map","0");
	}
	else if (resID == 5) {
		resObj->Init(RES_TYPE_FILEMAP,"sprite","zergling");
	}
	else if (resID == 1) {
		resObj->Init(RES_TYPE_FILEMAP,"sprite","dragoon");
	}
	else if (resID == 11) {
		resObj->Init(RES_TYPE_TILEMAP,"prj","bullet");
	}
	else if (resID == 6) {
		resObj->Init(RES_TYPE_FILEMAP,"sprite","vulture");
	}*/
	resObj->resID = resID;
	//resObj->LoadTextures();

}

ResourceObject* Resource_List::NewResource(long resID)
{

	ResourceObject* TempPtr;

	//Full Init if this is the first
	if (objCount == 0) {
		objCount++;
		
		TempPtr = new ResourceObject();
		this->resObjectsStart = TempPtr;
		this->resObjects = TempPtr;

		SetupResource(resID,resObjects);

		return TempPtr;

	}
	else {
		//Check if its loaded into memory already
		TempPtr = FindID(resID);
		if (!TempPtr) {
			objCount++;
			resObjects->NextObj = new ResourceObject();
			resObjects = resObjects->NextObj;

			SetupResource(resID,resObjects);

			return resObjects;
		} else {
			return TempPtr;
		}
	}
}

ResourceObject* Resource_List::FindID(long resID) {
	ResourceObject* TempPtr = this->resObjectsStart;

	if (TempPtr->resID == resID) { return TempPtr; }

	while (TempPtr->NextObj != NULL) {
		TempPtr = TempPtr->NextObj;
		if (TempPtr->resID == resID) { return TempPtr; }
	}
	return false;
}

ResourceObject* Resource_List::GetPrevious(ResourceObject* resObj) {
	ResourceObject* TempPtr = this->resObjectsStart;

	if (TempPtr == resObj) { return TempPtr; }

	while (TempPtr->NextObj != resObj) {
		TempPtr = TempPtr->NextObj;
	}
	return TempPtr;
}