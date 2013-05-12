#include "component_border.h"

Border::Border(int tileID){

    this->name = "NONE";
    this->type = cBORDER_T;
    this->attachedTileID = tileID;

};

Border::~Border(){

};

void Border::update(){

};

void Border::printInfo(){

    this->Component::printInfo();

    // Print info specific to Border

};
