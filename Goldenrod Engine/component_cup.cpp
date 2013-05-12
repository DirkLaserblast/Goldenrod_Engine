#include "component_cup.h"

Cup::Cup(){

    this->name = "NONE";
    this->type = cCUP_T;

};

Cup::~Cup(){

};

void Cup::update(){

};

void Cup::printInfo(){

    this->Component::printInfo();

    // Print info specific to Cup

};
