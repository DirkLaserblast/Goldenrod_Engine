#include "gamecontroller.h"

GameController::GameController(){

};

GameController::~GameController(){

	levels.clear();

};

void GameController::addLevel(){

	Level* newLevel = new Level();
    this->currentLevel = newLevel;

};

void GameController::addTileCurrentLevel(int ID, int numEdges, int numVerts, int posIndex, int colIndex,
                             int normIndex, int numNeighbors, int neighborIndex){

    this->currentLevel->addTile(ID, numEdges, numVerts, posIndex, colIndex, normIndex, numNeighbors, neighborIndex);

};

bool GameController::checkValidCurrentLevel(){

    return this->currentLevel->isValid();

};
