#ifndef COMPONENT_H
#define COMPONENT_H

#include "gameobject.h"

using namespace std;

enum COMPONENT_TYPE { cNONE_T, cVBO_T, cVAO_T, cPHYSC_T };
const string componentTypes[] = { NONE, "cVBO_T", "cVAO_T", "cPHYSC_T" };

class Component: public GameObject{

public:    
    virtual void update();
    virtual void printInfo();

    COMPONENT_TYPE getType();

protected:
    COMPONENT_TYPE type; // System type tag for object

};

#endif