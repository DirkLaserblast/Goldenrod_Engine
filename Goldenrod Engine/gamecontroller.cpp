#include "gamecontroller.h"

GameController::GameController(){

};

GameController::~GameController(){

	levels.clear();

};

vector<Shape>* GameController::getCurrentLevelShapes(){

    return this->currentLevel->getShapes();

};

void GameController::addLevel(){

	Level* newLevel = new Level();
    this->currentLevel = newLevel;

};

void GameController::addTileCurrentLevel(int ID, int numEdges, int numNeighbors, vector<glm::vec3> verts, vector<int> neighborIDs){

    this->currentLevel->addTile(ID, numEdges, numNeighbors, verts, neighborIDs);

};

void GameController::addTeeCurrentLevel(int ID, glm::vec3 loc){

    this->currentLevel->addTee(ID, loc);

};

void GameController::addCupCurrentLevel(int ID, glm::vec3 loc){

    this->currentLevel->addCup(ID, loc);

};

bool GameController::checkValidCurrentLevel(){

    return this->currentLevel->isValid();

};
