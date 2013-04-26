#ifndef TILE_H
#define TILE_H

#include <vector>

#include "shape.h"
#include "tileBorder.h"

#define TILE_COLOR glm::vec4(0.0,1.0,0.0,1.0)
#define TILE_DEPTH 0.1f

using namespace std;

class Tile{

public:
    // Functions
	Tile(int ID, int numEdges, int numShapes, int numNeighbors, int locIndex, int colIndex,
         int normIndex, int shapeIndex, vector<vec3> verts, vector<int> neighbors);
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
	vector<int> getNeighbors();
	vector<Shape*> shapesPointer; //Holds pointers to all the shapes used in the tile, good for accessing normals, etc.
	vector<Border*> borders; //Pointers to all borders associated with the tile
	vector<vec3> edges; //Vector that holds pairs of vertices (edges), can find out which tiles share vertices with this

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
	vector<int> neighborIDs;

};

#endif
