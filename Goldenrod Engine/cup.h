#ifndef CUP_H
#define CUP_H

// Includes
#include <vector>

#include "glm/glm.hpp"

#include "processedinputline.h"
//#include "geometry.h"
//#include "transformation.h"
#include "shape.h" // use instead of VBOs right now
#include "physics.h"

// Cup macros
#define CUP_COLOR  glm::vec4(1.0,0.0,0.0,1.0) // RED
//#define CUP_DEPTH 0.0f
#define CUP_WIDTH 0.2f // used for square cup
#define CUP_HEIGHT 0.2f
#define CUP_RADIUS 0.075f // used for circular cup
#define CUP_OFFSET 0.001f

// Using
using std::vector;
using glm::vec3;
using glm::vec4;

class Cup{

public:
    // Constructors/Destructor
	Cup(ProcessedInputLine* inLine);
    ~Cup();

    // Print info
    void printInfo();

	// Access methods
	//int getID();
    int getCurrentTileID();
	vector<Shape*> getShapes(); // Returns pointer to shapes vector
    Physics* getPhysics();

	//void setID(int newID);
    void setCurrentTileID(int newID);

private:
    // Tile data
    //int cupID;
    int currentTileID;
	vector<Shape*> shapes;
    Physics* physics;

    // Private methods
    void addRectangleShapes(vec3 point, vec4 color = CUP_COLOR, float width = CUP_WIDTH, float height = CUP_HEIGHT, float offset = CUP_OFFSET);

	void addCircleShapes(vec3 point, vec4 color = CUP_COLOR, float radius = CUP_RADIUS, float offset = CUP_OFFSET);
};

#endif