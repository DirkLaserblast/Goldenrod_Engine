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

    void addTileCurrentLevel(int ID, int numEdges, int numNeighbors, vector<glm::vec3> verts, vector<int> neighborIDs);

    void addTeeCurrentLevel(int ID, glm::vec3 loc);

    void addCupCurrentLevel(int ID, glm::vec3 loc);

    bool checkValidCurrentLevel();

    // Need to add a function to cycle through levels.

private:
	Level* currentLevel;
	vector<Level> levels; // Currently unused

};

#endif
