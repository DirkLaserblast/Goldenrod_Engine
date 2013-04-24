#include "level.h"

int Level::totalLevels = 0; // initialize static member

Level::Level(){
	this->ID = ++(Level::totalLevels);
    this->validate();
};

Level::~Level(){

    this->tiles.clear();
    this->verts.clear();
    this->cols.clear();
    this->norms.clear();

    if(tee != NULL){
        delete tee;
    }
    if(cup != NULL){
        delete cup;
    }

}

int Level::getTotalLevels(){ return Level::totalLevels; };

int Level::getID(){ return this->ID; };

void Level::validate(){

    // Need to add this function

};

bool Level::isValid(){

	return this->valid;

};

bool Level::addTile(int ID, int numEdges, int numNeighbors, vector<glm::vec3> verts, vector<int> neighborIDs){

    int posIndex = this->verts.size();
    int colIndex = this->cols.size();
    int normIndex = this->norms.size();

    Tile* newTile = new Tile(ID, numEdges, numNeighbors, posIndex, colIndex, normIndex);

    // Add vert data to level
    this->verts.insert(this->verts.end(), verts.begin(), verts.end());

    // Validate Entity
    if(newTile->isValid()){
		this->tiles.push_back((*newTile));
        //assert(this->tiles.size() != 1); // debug
		return true;
	}
	else{
		delete newTile;
		return false;
	}

};

bool Level::addTee(int ID, glm::vec3 loc){

    int locIndex = this->verts.size();

    Tee* newTee = new Tee(ID, locIndex);

    // Add vert data to level
    this->verts.push_back(loc);

    // Validate Entity
    if(newTee->isValid()){
		this->tee = newTee;
		return true;
	}
	else{
		delete newTee;
		return false;
	}

};

bool Level::addCup(int ID, glm::vec3 loc){

    int locIndex = this->verts.size();

    Cup* newCup = new Cup(ID, locIndex);

    // Add vert data to level
    this->verts.push_back(loc);

    // Validate Entity
    if(newCup->isValid()){
		this->cup = newCup;
		return true;
	}
	else{
		delete newCup;
		return false;
	}

};
