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
    this->shapes.clear();

    if(tee != NULL){
        delete tee;
    }
    if(cup != NULL){
        delete cup;
    }

}

int Level::getTotalLevels(){ return Level::totalLevels; };

int Level::getID(){ return this->ID; };

vector<Shape>* Level::getShapes(){ return &(this->shapes); };

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
    int shapeIndex = this->shapes.size();

    Shape* newShape = new Shape(verts,TILE_COLOR);  

    Tile* newTile = new Tile(ID, numEdges, numNeighbors, posIndex, colIndex, normIndex, shapeIndex);

    // Validate Entity
    if(newTile->isValid()){
        // Add tile to level
		this->tiles.push_back((*newTile));
        // Add vert data to level
        this->verts.insert(this->verts.end(), verts.begin(), verts.end());
        // Add shape data to level
        this->shapes.push_back((*newShape));
		return true;
	}
	else{
		delete newTile;
		return false;
	}

};

bool Level::addTee(int ID, glm::vec3 loc){

    int locIndex = this->verts.size();
    int shapeIndex = this->shapes.size();

    // Determine verts for tee based on pos
    vector<vec3> teeVerts;
    teeVerts.push_back(glm::vec3(loc.x - TEE_RADIUS, loc.y + TEE_PLANE_OFFSET, loc.z + TEE_RADIUS));
    teeVerts.push_back(glm::vec3(loc.x + TEE_RADIUS, loc.y + TEE_PLANE_OFFSET, loc.z + TEE_RADIUS));
    teeVerts.push_back(glm::vec3(loc.x + TEE_RADIUS, loc.y + TEE_PLANE_OFFSET, loc.z - TEE_RADIUS));
    teeVerts.push_back(glm::vec3(loc.x - TEE_RADIUS, loc.y + TEE_PLANE_OFFSET, loc.z - TEE_RADIUS));     
    
    Shape* newShape = new Shape(teeVerts, TEE_COLOR);

    Tee* newTee = new Tee(ID, locIndex, shapeIndex);

    // Validate Entity
    if(newTee->isValid()){
        // Add tee to level
		this->tee = newTee;
        // Add vert data to level
        this->verts.push_back(loc);
        // Add shape data to level
        this->shapes.push_back((*newShape));
		return true;
	}
	else{
		delete newTee;
		return false;
	}

};

bool Level::addCup(int ID, glm::vec3 loc){

    int locIndex = this->verts.size();
    int shapeIndex = this->shapes.size();

    // Determine verts for tee based on pos
    vector<vec3> cupVerts;
    cupVerts.push_back(glm::vec3(loc.x - CUP_RADIUS, loc.y + CUP_PLANE_OFFSET, loc.z + CUP_RADIUS));
    cupVerts.push_back(glm::vec3(loc.x + CUP_RADIUS, loc.y + CUP_PLANE_OFFSET, loc.z + CUP_RADIUS));
    cupVerts.push_back(glm::vec3(loc.x + CUP_RADIUS, loc.y + CUP_PLANE_OFFSET, loc.z - CUP_RADIUS));
    cupVerts.push_back(glm::vec3(loc.x - CUP_RADIUS, loc.y + CUP_PLANE_OFFSET, loc.z - CUP_RADIUS));     
    
    Shape* newShape = new Shape(cupVerts, CUP_COLOR);

    Cup* newCup = new Cup(ID, locIndex, shapeIndex);

    // Validate Entity
    if(newCup->isValid()){
        // Add cup to level
		this->cup = newCup;
        // Add vert data to level
        this->verts.push_back(loc);
        // Add shape data to level
        this->shapes.push_back((*newShape));
		return true;
	}
	else{
		delete newCup;
		return false;
	}

};
