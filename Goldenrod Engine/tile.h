#ifndef TILE_H
#define TILE_H

#include <vector>

#include "shape.h"

#define TILE_COLOR glm::vec4(0.0,1.0,0.0,1.0)
#define TILE_DEPTH 0.1f

using namespace std;

class Tile{

public:
    // Functions
	Tile(int ID, int numEdges, int numShapes, int numNeighbors, int locIndex, int colIndex,
         int normIndex, int shapeIndex);
	~Tile();

    void validate();
	bool isValid();

    int getID();

    int getNumEdges();
    int getNumShapes();
    int getLocIndex();
    int getColIndex();
    int getNormIndex();
    int getShapeIndex();
    int getNumNeighbors();

private:
    // Local Variables
	bool valid;
	int ID;
    int numEdges,
        numShapes,
        locIndex,
        colIndex,
        normIndex,
        shapeIndex,
        numNeighbors;

};

#endif
