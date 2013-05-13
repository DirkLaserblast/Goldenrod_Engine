#include "system.h"

void  System::update(){

};

void System::printInfo(){

    // Print name from super class
    this->GameObject::printInfo();

    // Print system type
    cout << "SYSTEM TYPE: " << systemTypes[this->type] << endl;

    // Print name of each assigned entity
    cout << "ENTITIES:" << endl;
    for(int i = 0; i < this->entities.size(); i++){
        (this->entities[i])->GameObject::printInfo(); // only print name
    }

    cout << endl;

};
