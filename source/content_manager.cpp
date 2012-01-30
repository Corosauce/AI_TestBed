#include "content_manager.h"

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "inc/dirent.h"


// testmsms.cpp : Defines the entry point for the console application.
#include "inc/stdafx.h"
//#include <stdio.h>
//#include <iostream>
//#include <windows.h>




//#include "inc/INI.h"

//#define _AFXDLL

bool Content_Manager::loadContent() {

	




	std::vector<std::string> results;

	

	//Load Unit Types
	if (!lsdir("content\\sprite",results)) { return false; }

	int newResId = 0;
	int resCount;

	for (resCount = 0; resCount < results.size(); resCount++) {
		//this->Resources.N

		if (results.at(resCount) != "." && results.at(resCount) != "..") {

			this->TempUnit = new sprite_info;

			TempUnit->name = results.at(resCount);
			TempUnit->ResID = this->Resources.NewResource(newResId);
			

			if (this->loadSprite(TempUnit->name,TempUnit)) {
				TempUnit->ResID->Init(TempUnit->ResID->resInfo.imgFormat,"sprite",TempUnit->name);
				TempUnit->typeID = newResId;
				this->UnitTypes.push_back(TempUnit);

				newResId++;
			}

			this->ptrWorld->loader.Update("units",newResId);

			
		}
		
	}
	
	results.clear();
	if (!lsdir("content\\prj",results)) { return false; }

	//newResId = 0;

	for (resCount = 0; resCount < results.size(); resCount++) {
		//this->Resources.N

		if (results.at(resCount) != "." && results.at(resCount) != "..") {

			this->TempPrj = new prj_info;

			TempPrj->name = results.at(resCount);
			TempPrj->ResID = this->Resources.NewResource(newResId);
			
			this->loadPrj(TempPrj->name,TempPrj);

			//TempPrj->ResID->Init(RES_TYPE_FILEMAP,"prj",TempPrj->name);
			TempPrj->ResID->Init(TempPrj->ResID->resInfo.imgFormat,"prj",TempPrj->name);
			
			TempUnit->typeID = newResId;
			this->PrjTypes.push_back(TempPrj);

			newResId++;
		}
		
	}



	return true;

}

bool Content_Manager::loadSprite(std::string file, sprite_info* ptrObj) {

	DWORD active;
	char thm[MAX_PATH] = "";
	

	int temp2 = 0;
	

	sstr.str("");
	sstr<<"content\\"<<"sprite\\"<<file<<"\\unitdata.ini";
	str1 = sstr.str();
	

	//std::string file, 

	cat = "general";

	active = GetPrivateProfileString(cat.c_str(), "name", "FAIL", thm, MAX_PATH, str1.c_str());
	
	std::string	cmp1("FAIL");
	std::string	cmp2(thm);
	//cmp1(temp);
	//cmp2(thm);
	temp2 = cmp1.compare(cmp2);

	if (cmp1.compare(cmp2) != -1) {
		return false;
	}

	active = GetPrivateProfileString(cat.c_str(), "health", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->health = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "viewdistance", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->viewdistance = atof(thm);


	cat = "phys";

	active = GetPrivateProfileString(cat.c_str(), "turnspeed", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->turnspeed = atof(thm);
	
	active = GetPrivateProfileString(cat.c_str(), "goforce", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->goforce = atof(thm);
	
	active = GetPrivateProfileString(cat.c_str(), "stopforce", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->stopforce = atof(thm);
	
	active = GetPrivateProfileString(cat.c_str(), "cruisespeed", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->cruisespeed = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "maxforcespeed", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->maxforcespeed = atof(thm);

	cat = "weapon";

	active = GetPrivateProfileString(cat.c_str(), "name", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->weapons[0].name = thm;

	active = GetPrivateProfileString(cat.c_str(), "range", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->weapons[0].range = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "reloadtime", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->weapons[0].reloadtime = atof(thm);

	cat = "animation";

	active = GetPrivateProfileString(cat.c_str(), "imgformat", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->ResID->resInfo.imgFormat = atof(thm);
	
	active = GetPrivateProfileString(cat.c_str(), "angle.max", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->angle.max = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "alpha_r", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->ResID->resInfo.alpha_r = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "alpha_g", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->ResID->resInfo.alpha_g = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "alpha_b", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->ResID->resInfo.alpha_b = atof(thm);
	
	//ptrObj->animation_list.Init(ptrObj);
	//XPos = GetPrivateProfileInt(szAppName, "viewdistance", 0, str1.c_str());

	return true;
}

bool Content_Manager::loadPrj(std::string file, prj_info* ptrObj) {

	DWORD active;
	char thm[MAX_PATH] = "";
	

	sstr.str("");
	sstr<<"content\\"<<"prj\\"<<file<<"\\prjdata.ini";
	str1 = sstr.str();
	

	//std::string file, 

	cat = "general";

	active = GetPrivateProfileString(cat.c_str(), "name", "FAIL", thm, MAX_PATH, str1.c_str());

	std::string	cmp1("FAIL");
	std::string	cmp2(thm);
	//cmp1(temp);
	//cmp2(thm);
	//temp2 = cmp1.compare(cmp2);

	//just for testing if the file exists by getting a mandatory field
	if (cmp1.compare(cmp2) != -1) {
		return false;
	}

	
	active = GetPrivateProfileString(cat.c_str(), "health", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->health = atof(thm);

	cat = "phys";

	active = GetPrivateProfileString(cat.c_str(), "turnspeed", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->turnspeed = atof(thm);
	
	active = GetPrivateProfileString(cat.c_str(), "goforce", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->goforce = atof(thm);
	
	active = GetPrivateProfileString(cat.c_str(), "stopforce", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->stopforce = atof(thm);
	
	active = GetPrivateProfileString(cat.c_str(), "cruisespeed", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->cruisespeed = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "maxforcespeed", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->maxforcespeed = atof(thm);

	cat = "weapon";

	active = GetPrivateProfileString(cat.c_str(), "dmg_radius", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->dmg_radius = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "dmg_amount", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->dmg_amount = atof(thm);

	cat = "animation";

	active = GetPrivateProfileString(cat.c_str(), "imgformat", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->ResID->resInfo.imgFormat = atof(thm);
	
	active = GetPrivateProfileString(cat.c_str(), "angle.max", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->angle.max = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "alpha_r", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->ResID->resInfo.alpha_r = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "alpha_g", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->ResID->resInfo.alpha_g = atof(thm);

	active = GetPrivateProfileString(cat.c_str(), "alpha_b", "FAIL", thm, MAX_PATH, str1.c_str());
	ptrObj->ResID->resInfo.alpha_b = atof(thm);

	
	
	return true;
}

prj_info* Content_Manager::LoadPrjName(std::string name) {
	for (int i = 0; i < this->PrjTypes.size(); i++) {

		if (PrjTypes.at(i)->name == name) {
			return PrjTypes.at(i);
		}

	}

	return false;
}

sprite_info* Content_Manager::LoadUnitName(std::string name) {
	for (int i = 0; i < this->UnitTypes.size(); i++) {

		if (UnitTypes.at(i)->name == name) {
			return UnitTypes.at(i);
		}

	}

	return false;
}

long Content_Manager::UnitNameToID(std::string name) {
	for (int i = 0; i < this->UnitTypes.size(); i++) {

		if (UnitTypes.at(i)->name == name) {
			return UnitTypes.at(i)->typeID;
			//return UnitTypes.at(i);
		}

	}

	return false;
}

sprite_info* Content_Manager::LoadUnitID(long ID) {
	return UnitTypes.at(ID);
	

	return false;
}

bool Content_Manager::lsdir(const char *path, std::vector<std::string> &results)
{
        WIN32_FIND_DATA fdata;
        HANDLE dhandle;

        // must append \* to the path
        {
                char buf[MAX_PATH];
                sprintf_s(buf, "%s\\*", path);
                if((dhandle = FindFirstFile(buf, &fdata)) == INVALID_HANDLE_VALUE) {
                        return false;
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

		return true;
} 

