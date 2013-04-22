#ifndef LEVEL_H
#define LEVEL_H

#include <cstdlib>
#include <vector>

#include "tile.h"

using namespace std;

class Level{

public:
	Level();
	~Level();

	static int getTotalLevels();
	int getID();
    void validate();
	bool isValid();
    bool addTile(int ID, int numEdges, int numVerts, int posIndex, int colIndex, int normIndex,
                 int numNeighbors, int neighborIndex);

private:
	static int totalLevels;
	int ID;
	bool valid;
	vector<Tile> tiles;
	vector<float> vertices;
	vector<float> colors;
	vector<float> normals;
	
};

#endif
