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
	 *			   Changes current level so that it points to the newly added level.
     */
	void addLevel();

    void addTileCurrentLevel(int ID, int numEdges, int numVerts, int posIndex, int colIndex,
                             int normIndex, int numNeighbors, int neighborIndex);

    bool checkValidCurrentLevel();

    // Need to add a function to cycle through levels.

private:
	Level* currentLevel;
	vector<Level> levels;

};

#endif
