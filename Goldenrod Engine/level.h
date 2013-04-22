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

	bool isValid();
    bool addTile(int ID, int numVerts, int posIndex, int colIndex, int normIndex,
                 int numNeighbors, int neighborIndex);

private:
	bool valid;
	vector<Tile> tiles;
	vector<float> vertices;
	vector<float> colors;
	vector<float> normals;
	
};

#endif
