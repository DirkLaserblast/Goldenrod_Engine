#include "component_tile.h"

Tile::Tile(){

    this->name = "NONE";
    this->type = cTILE_T;

};

Tile::~Tile(){

};

void Tile::update(){

};

void Tile::printInfo(){

    this->Component::printInfo();

    // Print info specific to Tile

};
