#include "component_tile.h"

Tile::Tile(){

    this->name = "NONE";
    this->type = cTILE_T;
    this->tileID = -1;
    this->friction = TILE_FRICTION;
    this->neighborIDs.clear();

};

Tile::Tile(int tileID, vector<int> neighborIDs){

    this->name = "NONE";
    this->type = cTILE_T;
    this->tileID = tileID;
    this->friction = TILE_FRICTION;
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

float Tile::getFriction(){ return this->friction; };

void Tile::setFriction(float newFriction){ this->friction = newFriction; };

vector<int> Tile::getNeighborIDs(){ return this->neighborIDs; };
