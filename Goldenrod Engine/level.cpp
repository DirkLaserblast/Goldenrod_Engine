#include "level.h"

int Level::totalLevels = 0; // initialize static member

Level::Level(){
	this->ID = Level::totalLevels++;
    this->validate();
};

Level::~Level(){

}

int Level::getTotalLevels(){ return Level::totalLevels; };

int Level::getID(){ return this->ID; };

void Level::validate(){

    // Need to add this function

};

bool Level::isValid(){

	return this->valid;

};

bool Level::addTile(int ID, int numEdges, int numVerts, int posIndex, int colIndex, int normIndex,
                    int numNeighbors, int neighborIndex){

    Tile* newTile = new Tile(ID, numEdges, numVerts, posIndex, colIndex, normIndex, numNeighbors, neighborIndex);

    if(newTile->isValid()){
		this->tiles.push_back(*newTile);
		return true;
	}
	else{
		delete newTile;
		return false;
	}

};
