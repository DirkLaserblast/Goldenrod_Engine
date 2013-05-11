#include "level_conversion.h"

Level::Level(){

    // Default values
    this->name = "NONE";
    this->type = "LEVEL_T";
    this->ID = -1;

};

Level::Level(vector<ProcessedInputLine>& inLines){

    string keyword;

    for(int i = 0; i < inLines.size(); i++){
        // Create appropriate Entities and add to corresponding vectors
        keyword = inLines[i].getKeyword();
        if(keyword == "tile"){
            this->addTile();
        }
        else if(keyword == "tee"){
            this->addTee();
        }
        else if(keyword == "cup"){
            this->addCup();
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

void Level::addTile(){
    
    // Need to add code that properly addes tile data
    Entity* newTile = new Entity();
    newTile->attachComponent(new Tile());
    newTile->attachComponent(new VBO());

    this->tiles.push_back(newTile);

};

void Level::deleteTile(){
    
};

void Level::addCup(){
    
};

void Level::deleteCup(){
    
};

void Level::addTee(){
    
};

void deleteTee(){
    
};
