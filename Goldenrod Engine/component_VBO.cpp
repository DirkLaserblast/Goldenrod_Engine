#include "component_VBO.h"

VBO::VBO(){

    this->name = "NONE";
    this->type = cVBO_T;

};

VBO::~VBO(){

};

void VBO::update(){

};

void VBO::printInfo(){

    this->Component::printInfo();

    // Print info specific to VBO

};
