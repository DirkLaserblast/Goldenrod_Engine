#include "system.h"

using namespace std;

void  System::update(){

};

void System::printInfo(){

    // Print system name
    cerr << endl << "SYSTEM NAME: " << this->name << endl;

    // Print system type
    cerr << "TYPE: " << systemTypes[this->type] << endl;

};
