#include "component.h"

void  Component::update(){

};

void Component::printInfo(){

    // Print component name
    cout << endl << "COMPONENT NAME: " << this->name << endl;

    // Print system type
    cout << "TYPE: " << componentTypes[this->type] << endl;

};

COMPONENT_TYPE Component::getType(){ return this->type; };
