#include "level_conversion.h"

int Level::levelCount = 0;

Level::Level(){

    // Default values
    this->name = "NONE";
    this->type = "LEVEL_T";
    this->ID = (Level::levelCount)++;

};

Level::Level(vector<ProcessedInputLine*>* inLines){

    // Same as default
    this->name = "NONE";
    this->type = "LEVEL_T";
    this->ID = (Level::levelCount)++;

    string keyword;

    for(int i = 0; i < inLines->size(); i++){
        // Create appropriate Entities and add to corresponding vectors
        keyword = (*inLines)[i]->getKeyword();
        if(keyword == "tile"){
            this->addTile(*((*inLines)[i]));
        }
        else if(keyword == "tee"){
            this->addTee(*((*inLines)[i]));
        }
        else if(keyword == "cup"){
            this->addCup(*((*inLines)[i]));
        }
        else{
            cerr << endl << "Invalid keyword in processed file." << endl;
        }
    }

};

Level::~Level(){

};

void Level::update(){

};

void Level::printInfo(){

    // Print level name
    cerr << endl << "LEVEL NAME: " << this->name << endl;
    // Print level type
    cerr << "TYPE: " << this->type << endl;
    // Print ID
    cerr << "ID: " << this->ID << endl;


};

void Level::addTile(ProcessedInputLine& inLine){
    
    // New entity
    Entity* newTile = new Entity(eSTATIC_GEOMETRY_T);

    // Attach components
    newTile->attachComponent(new Tile());

    newTile->attachComponent(new VBO());

    newTile->attachComponent(new Shapes(inLine.getVerts(), TILE_COLOR, TILE_DEPTH));

    this->tiles.push_back(newTile);

};

void Level::deleteTile(){
    
};

void Level::addCup(ProcessedInputLine& inLine){
    
    // New entity
    Entity* newCup = new Entity(eSTATIC_GEOMETRY_T);

    // Attach components
    newCup->attachComponent(new Cup());

    newCup->attachComponent(new VBO());

    vector<glm::vec3> cupVerts = squareFromPoint(inLine.getVerts().at(0),CUP_WIDTH,CUP_HEIGHT,CUP_OFFSET);
    newCup->attachComponent(new Shapes(cupVerts, CUP_COLOR, CUP_DEPTH));

    this->cup = newCup;

};

void Level::deleteCup(){
    
};

void Level::addTee(ProcessedInputLine& inLine){
    
    // New entity
    Entity* newTee = new Entity(eSTATIC_GEOMETRY_T);

    // Attach components
    newTee->attachComponent(new Tee());

    newTee->attachComponent(new VBO());

    vector<glm::vec3> teeVerts = squareFromPoint(inLine.getVerts().at(0),TEE_WIDTH,TEE_HEIGHT,TEE_OFFSET);
    newTee->attachComponent(new Shapes(teeVerts, TEE_COLOR, TEE_DEPTH));

    this->tee = newTee;

};

void deleteTee(){
    
};

int Level::getLevelCount(){ return this->levelCount; };

vector<Shape>* Level::getCurrentLevelShapes(){

  vector<Shape>* allCurrentLevelShapes;

  // Get tile shapes
  for(int i = 0; i < this->tiles.size(); i++){
      
  }

  return allCurrentLevelShapes;


};

vector<glm::vec3> Level::squareFromPoint(glm::vec3 point, float width, float height, float offset){
    
    vector<glm::vec3> squareVerts;

    squareVerts.push_back(glm::vec3(point.x - width, point.y + offset, point.z + height));
    squareVerts.push_back(glm::vec3(point.x + width, point.y + offset, point.z + height));
    squareVerts.push_back(glm::vec3(point.x + width, point.y + offset, point.z - height));
    squareVerts.push_back(glm::vec3(point.x - width, point.y + offset, point.z - height));

    return squareVerts;

};
