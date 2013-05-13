#include "system_levelcontroller.h"

LevelController::LevelController(){

    this->type = sLEVEL_CONTROLLER_T;
    this->currentLevel = NULL;

};

LevelController::~LevelController(){
    
};

void LevelController::update(){
    
};

void LevelController::printInfo(){
    
};

void LevelController::addLevel(){       

};

void LevelController::addLevel(vector<ProcessedInputLine*>* inLines){

    Level* newLevel = new Level(inLines);

    this->levels.push_back(newLevel);
    this->currentLevel = newLevel;

};

void LevelController::deleteLevel(){
    
};

void LevelController::nextLevel(){
    
};

void LevelController::prevLevel(){
    
};

void LevelController::goToLevel(){
    
};

vector<Shape>* LevelController::getCurrentLevelShapes(){ // REMOVE THIS AFTER CONVERT TO USING VBOs
    
    return this->currentLevel->getCurrentLevelShapes();

};

void LevelController::updateCurrentLevelShapes(){

    this->currentLevel->updateCurrentLevelShapes();

};
