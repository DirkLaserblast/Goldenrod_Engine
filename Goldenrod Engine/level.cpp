#include "level.h"

Level::Level(){

};

Level::~Level(){

}

bool Level::isValid(){

	return this->valid;

};

bool Level::addTile(int ID, int numVerts, int posIndex, int colIndex, int normIndex,
                    int numNeighbors, int neighborIndex){

    Tile* newTile = new Tile(ID, numVerts, posIndex, colIndex, normIndex, numNeighbors, neighborIndex);

    if(newTile->isValid()){
		this->tiles.push_back(*newTile);
		return true;
	}
	else{
		delete newTile;
		return false;
	}

};
