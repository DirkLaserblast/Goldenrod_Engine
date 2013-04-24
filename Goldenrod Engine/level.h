#ifndef LEVEL_H
#define LEVEL_H

#include <cstdlib>
#include <vector>

#include "glm/glm.hpp"

#include "tile.h"
#include "tee.h"
#include "cup.h"

using namespace std;

class Level{

public:
	Level();
	~Level();

	static int getTotalLevels();
	int getID();
    void validate();
	bool isValid();
    bool addTile(int ID, int numEdges, int numNeighbors, vector<glm::vec3> verts, vector<int> neighborIDs);
    bool addTee(int ID, glm::vec3 loc);
    bool addCup(int ID, glm::vec3 loc);

private:
	static int totalLevels;
	int ID;
	bool valid;
    Tee* tee;
    Cup* cup;
	vector<Tile> tiles;
    vector<glm::vec3> verts;
    vector<glm::vec3> cols;
    vector<glm::vec3> norms;
	
};

#endif
