#include "component_tile.h"

Tile::Tile(){

    this->name = "NONE";
    this->type = cTILE_T;
    this->tileID = -1;
    this->neighborIDs.clear();

};

Tile::Tile(int tileID, vector<int> neighborIDs){

    this->name = "NONE";
    this->type = cTILE_T;
    this->tileID = tileID;
    this->neighborIDs = neighborIDs;

};

Tile::~Tile(){

};

void Tile::update(){

};

void Tile::printInfo(){

    this->Component::printInfo();

    // Print info specific to Tile

};
