#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <cstdlib>
#include <vector>

#include "level.h"

using namespace std;

class GameController{

public:
	GameController();
	~GameController();

	/* Level functions */

	/*
     * addLevel(): Creates a new level object and adds a pointer to the levels vector.
	 * return: returns true if sucessful, and false if failed
     */
	bool addLevel();

private:
	Level currentLevel;
	vector<Level> levels;

};

#endif
