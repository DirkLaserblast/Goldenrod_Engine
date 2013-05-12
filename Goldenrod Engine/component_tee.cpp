#include "component_tee.h"

Tee::Tee(){

    this->name = "NONE";
    this->type = cTEE_T;
    this->tileID = -1;

};

Tee::Tee(int tileID){

    this->name = "NONE";
    this->type = cTEE_T;
    this->tileID = tileID;
};

Tee::~Tee(){

};

void Tee::update(){

};

void Tee::printInfo(){

    this->Component::printInfo();

    // Print info specific to Tee

};
