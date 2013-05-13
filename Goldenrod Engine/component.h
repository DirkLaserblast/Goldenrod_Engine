#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <iostream>

using namespace std;

enum COMPONENT_TYPE { cNONE_T, cTEST_T, cVBO_T, cVAO_T, cSHAPES_T, cTILE_T, cBORDER_T, cCUP_T, cTEE_T, cBALL_T, cPHYSC_T, cCOLLISION_T};
const string componentTypes[] = { "NONE", "cTEST_T", "cVBO_T", "cVAO_T", "cSHAPES_T", "cTILE_T", "cBORDER_T", "cCUP_T", "cTEE_T", "cBALL_T", "cPHYSC_T", "cCOLLISION_T"};

class Component{

public:    
    virtual void printInfo();

    COMPONENT_TYPE getType();

protected:
    string name; // Assigned name for component
    COMPONENT_TYPE type; // System type tag for object

};

#endif