#include "component_tee.h"

Tee::Tee(){

    this->name = "NONE";
    this->type = cTEE_T;

};

Tee::~Tee(){

};

void Tee::update(){

};

void Tee::printInfo(){

    this->Component::printInfo();

    // Print info specific to Tee

};
