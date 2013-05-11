#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;
using std::cerr;
using std::cout;

enum SYSTEM_TYPE { sNONE_T, sRENDERER_T, sFILEIO_T, sGAMEIO_T, sGAME_T, sPHYSICS_T, sSHADER_MANAGER_T };
const string systemTypes[] = { "NONE", "sRENDERER_T", "sFILEIO_T", "sGAMEIO_T", "sGAME_T", "sPHYSICS_T", "sSHADER_MANAGER_T" };

class System{

public:    
    virtual void update();
    virtual void printInfo();

protected:
    string name; // Assigned name for system
    SYSTEM_TYPE type; // System type tag for object   

};

#endif