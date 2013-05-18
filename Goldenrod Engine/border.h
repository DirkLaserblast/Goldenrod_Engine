#ifndef BORDER_H
#define BORDER_H

// Includes
#include <vector>

#include "glm/glm.hpp"

//#include "geometry.h"
//#include "transformation.h"
#include "shape.h" // use instead of VBOs right now

// Border macros
#define BORDER_COLOR  glm::vec4(1.0,0.5,0.0,1.0) // Red-orange
#define BORDER_HEIGHT 0.2f
#define BORDER_THICKNESS 0.01f

// Using
using std::vector;
using glm::vec3;

class Border{

public:
    Border();
	Border(vector<vec3> verts, vector<int> neighborIDs);
    ~Border();

    void printInfo();

	// Access methods
	int getID();
	vector<Shape*>* getShapes(); // Returns pointer to shapes vector

private:
    int tileID;
    //Geometry* geometry;
    //Transformation* transform;
	vector<Shape*> shapes;
};

#endif