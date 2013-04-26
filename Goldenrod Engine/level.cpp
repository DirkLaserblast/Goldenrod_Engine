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

bool Level::addTile(int ID, int numEdges, int numNeighbors, vector<glm::vec3> verts, vector<int> neighborIDs, bool addDepthFlag){

    int posIndex = this->verts.size();
    int colIndex = this->cols.size();
    int normIndex = this->norms.size();
    int shapeIndex = this->shapes.size();
    vector<Shape> newShapes;

    // Code to generate the Shapes to add for the tile
    if(addDepthFlag){
        newShapes = generateDepthShapes(verts); // testing code
    }
    else{
        newShapes.push_back(Shape(verts, TILE_COLOR));
    }  

    Tile* newTile = new Tile(ID, numEdges, numNeighbors, newShapes.size(), posIndex, colIndex, normIndex, shapeIndex);

    // Validate Entity
    if(newTile->isValid()){
        // Add tile to level
		this->tiles.push_back((*newTile));
        // Add vert data to level -- DOES NOT INCLUDE ADDITIONAL VERT DATA FOR TILES WITH DEPTH
        this->verts.insert(this->verts.end(), verts.begin(), verts.end());
        // Add shape(s) data to level
        for(int i = 0; i < newShapes.size(); i++){
            this->shapes.push_back(newShapes[i]);
        }
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

vector<Shape> Level::generateDepthShapes(vector<glm::vec3> verts){
    
    // Separate out new vectors with verts for each face in correct order for shape generation
    // Top and bottom
    vector<glm::vec3> top = verts;

    vector<glm::vec3> bottom;
    int topVertIndex = 0;
    for(int i = 0; i < verts.size(); i++){
        bottom.push_back(glm::vec3(top[topVertIndex].x, top[topVertIndex].y - TILE_DEPTH, top[topVertIndex].z));

        topVertIndex--;
        if(topVertIndex < 0){
            topVertIndex = top.size() - 1;
        }
    }

    // Sides
    // Initial vert index setup
    int firstVertIndex = 0, // index of top vert
        secondVertIndex = 0, // index of bottom vert
        thirdVertIndex = (bottom.size() - 1), // index of bottom vert
        fourthVertIndex = 1; // index of top vert

    vector<glm::vec3> sides; // Store all side verts 

    for(int i = 0; i < verts.size(); i++){ // For each side
        sides.push_back(top[firstVertIndex]);
        sides.push_back(bottom[secondVertIndex]);
        sides.push_back(bottom[thirdVertIndex]);
        sides.push_back(top[fourthVertIndex]);

        // Update vert indices
        firstVertIndex--;
        secondVertIndex++;
        thirdVertIndex++;
        fourthVertIndex--;

        // Correct for wrap around
        if(firstVertIndex < 0){
            firstVertIndex = (top.size() - 1);
        }
        if(secondVertIndex > (bottom.size() - 1)){
            secondVertIndex = 0;
        }
        if(thirdVertIndex > (bottom.size() - 1)){
            thirdVertIndex = 0;
        }
        if(fourthVertIndex < 0){
            fourthVertIndex = (top.size() - 1);
        }
    }

    // Create shape vector to return
    vector<Shape> newShapes;
    vector<glm::vec3> tmpVec;
    vector<glm::vec3>::iterator it;

    newShapes.push_back(Shape(top, TILE_COLOR));
    newShapes.push_back(Shape(bottom, TILE_COLOR));
    for(it = sides.begin(); it != sides.end(); it+=4){
        tmpVec.clear();
        tmpVec.insert(tmpVec.begin(), it, (it +4));
        newShapes.push_back(Shape(tmpVec, TILE_COLOR));
    }

    return newShapes;

};
