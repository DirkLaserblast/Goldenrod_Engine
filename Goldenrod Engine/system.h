#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>

#include "gameobject.h"
#include "entity.h"

using namespace std;

enum SYSTEM_TYPE { sNONE_T, sRENDERER_T, sFILEIO_T, sGAMEIO_T, sGAME_T, sPHYSICS_T, sSHADER_MANAGER_T };
const string systemTypes[] = { NONE, "sRENDERER_T", "sFILEIO_T", "sGAMEIO_T", "sGAME_T", "sPHYSICS_T", "sSHADER_MANAGER_T" };

class System: public GameObject{

public:    
    virtual void update();
    virtual void printInfo();

protected:
    SYSTEM_TYPE type; // System type tag for object   
    vector<Entity*> entities; // Vector of pointers to assigned entities

};

#endif