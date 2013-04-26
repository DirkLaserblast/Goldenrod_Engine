/*tiles.cpp
Functions:
Load tiles from a file
Draw tiles to screen
Tile datatype
*/

#include "tile.h"

Tile::Tile(int ID, int numEdges, int numShapes, int numNeighbors, int locIndex, int colIndex,
           int normIndex, int shapeIndex){

    this->ID = ID;
    this->numEdges = numEdges;
    this->numShapes = numShapes;
    this->locIndex = locIndex;
    this->colIndex = colIndex;
    this->normIndex = normIndex;
    this->shapeIndex = shapeIndex;
    this->numNeighbors = numNeighbors;

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
int Tile::getNumShapes(){ return numShapes; };
int Tile::getLocIndex(){ return locIndex; };
int Tile::getColIndex(){ return colIndex; };
int Tile::getNormIndex(){ return normIndex; };
int Tile::getShapeIndex(){ return shapeIndex; };
int Tile::getNumNeighbors(){ return numNeighbors; };
