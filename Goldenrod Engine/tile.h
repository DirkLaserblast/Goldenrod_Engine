#ifndef TILE_H
#define TILE_H

// Includes
#include <vector>

#include "glm/glm.hpp"

#include "processedinputline.h"
//#include "geometry.h"
//#include "transformation.h"
#include "shape.h" // use instead of VBOs right now
#include "physics.h"
#include "border.h"

// Tile macros
#define TILE_DEFAULT_ID -1
#define TILE_DEFAULT_FRICTION 0.0001L
#define TILE_DEFAULT_COLOR glm::vec4(0.0,1.0,0.0,1.0) // GREEN
#define TILE_DEFAULT_DEPTH 0.1f
#define TILE_USE_BORDER true // Make borders when adding tiles

// Using
using std::vector;
using glm::vec3;
using glm::vec4;

class Tile{

public:
    // Constructors/Destructor
	Tile(ProcessedInputLine* inLine);
    ~Tile();

    // Print info
    void printInfo();

	// Access methods
	int getID();
    double getFriction();
	vector<int> getNeighborIDs();
	vector<Shape*>* getShapes(); // Returns pointer to shapes vector
	Border* getBorders();

	void setID(int newID);
	void setFriction(double newFriction = TILE_DEFAULT_FRICTION);

private:
    // Tile data
    int tileID;
    double friction;
    vector<int>neighborIDs;
	vector<Shape*> shapes;
	Border* borders;

    // Private methods
    void addTileShapes(vector<vec3> verts, vec4 color = TILE_DEFAULT_COLOR, float depth = TILE_DEFAULT_DEPTH);
};

#endif