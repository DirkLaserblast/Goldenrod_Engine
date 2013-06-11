#include "scorecontroller.h"

void updateTotalScore(int& totalScore, int& currentHoleScore){

	// Update score vars
	totalScore += currentHoleScore;
	currentHoleScore = 0;

};

void reset(int& totalScore, int& currentHoleScore, LevelController* levelController, FileIOController* fileIO){

	levelController->loadLevel(fileIO, 0);
	currentHoleScore = 0;
	totalScore = 0;

};
