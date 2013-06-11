#include "levelcontroller.h"

// Constructors/Destructor
LevelController::LevelController(){

    this->currentLevel = NULL;

};

LevelController::~LevelController(){

    delete this->currentLevel;

};

// Load level
void LevelController::loadCurrentLevel(FileIOController* fileIO){

    // Load/reload current level
    //cerr << endl << "Loading/reloading current level." << endl;
    delete this->currentLevel;
    this->currentLevel = new Level(fileIO->getCurrentHole(), fileIO->getCurrentHoleID());

};

void LevelController::loadNextLevel(FileIOController* fileIO){

    //cerr << endl << "Loading next level." << endl;
	// Check if on end
    if(fileIO->nextHole()){
        // Delete the current level
        delete this->currentLevel;

        // Load next level
        this->currentLevel = new Level(fileIO->getCurrentHole(), fileIO->getCurrentHoleID());
	}
    else{
        cerr << "Cannot load level at next index: Index out of bounds." << endl;
    }

};

void LevelController::loadPrevLevel(FileIOController* fileIO){

    //cerr << endl << "Loading previous level." << endl;
    // Check if on end
    if(fileIO->prevHole()){
        // Delete the current level
        delete this->currentLevel;

        // Load prev level
        this->currentLevel = new Level(fileIO->getCurrentHole(), fileIO->getCurrentHoleID());
	}
    else{
        cerr << "Cannot load level at previous: Index out of bounds." << endl;
    }

};

void LevelController::loadFirstLevel(FileIOController* fileIO){
	
	//cerr << endl << "Loading first level." << endl;

	delete this->currentLevel;
	fileIO->goToHole(0);
	this->currentLevel = new Level(fileIO->getCurrentHole(), fileIO->getCurrentHoleID());

};

void LevelController::loadLastLevel(FileIOController* fileIO){
	
	//cerr << endl << "Loading last level." << endl;

	delete this->currentLevel;
	fileIO->goToHole(fileIO->getNumHoles() - 1);
	this->currentLevel = new Level(fileIO->getCurrentHole(), fileIO->getCurrentHoleID());

};

void LevelController::loadLevel(FileIOController* fileIO, int levelIndex){

    //cerr << endl << "Loading level at index " << levelIndex << "." << endl;
    // Check for bad index
    if(fileIO->goToHole(levelIndex)){
        // Delete the current level
        delete this->currentLevel;

        // Load next level
        this->currentLevel = new Level(fileIO->getCurrentHole(), fileIO->getCurrentHoleID());
    }
    else{
        cerr <<"Cannot load level at index " << levelIndex << ": Index out of bounds." << endl;
    }

};

Level* LevelController::getCurrentLevel(){ return this->currentLevel; };
