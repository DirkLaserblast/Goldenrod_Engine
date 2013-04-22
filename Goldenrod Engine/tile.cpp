/*tiles.cpp
Functions:
Load tiles from a file
Draw tiles to screen
Tile datatype
*/

#include "tile.h"

Tile::Tile(int ID, int numEdges, int numVerts, int posIndex, int colIndex,
           int normIndex, int numNeighbors, int neighborIndex){

    this->ID = ID;

    this->numEdges = numEdges;
    this->numVerts = numVerts;
    this->posIndex = posIndex;
    this->colIndex = colIndex;
    this->normIndex = normIndex;
    this->numNeighbors = numNeighbors;
    this->neighborIndex = neighborIndex;

    this->validate();

};

Tile::~Tile(){   

};

void Tile::validate(){

    // Need to add this function

};

bool Tile::isValid(){

	return this->valid;

};

int Tile::getID(){ return ID; };

int Tile::getNumEdges(){ return numEdges; };
int Tile::getNumVerts(){ return numVerts; };   
int Tile::getPosIndex(){ return posIndex; };
int Tile::getColIndex(){ return colIndex; };
int Tile::getNormIndex(){ return normIndex; };

int Tile::getNumNeighbors(){ return numNeighbors; };
int Tile::getNeighborIndex(){ return neighborIndex; };

