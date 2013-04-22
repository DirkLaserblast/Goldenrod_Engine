#include "gamecontroller.h"

GameController::GameController(){

};

GameController::~GameController(){

	levels.clear();

};

bool GameController::addLevel(){

	Level* newLevel = new Level();

	if(newLevel->isValid()){
		this->levels.push_back(*newLevel);
		return true;
	}
	else{
		delete newLevel;
		return false;
	}

};
