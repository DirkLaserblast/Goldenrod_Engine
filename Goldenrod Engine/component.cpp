#include "component.h"

void  Component::update(){

};

void Component::printInfo(){

    // Print name from super class
    this->GameObject::printInfo();

    // Print system type
    cout << "COMPONENT TYPE: " << componentTypes[this->type] << endl;

};

COMPONENT_TYPE Component::getType(){ return this->type; };
