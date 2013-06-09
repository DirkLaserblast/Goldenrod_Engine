#ifndef BORDER_H
#define BORDER_H

// Includes
#include <vector>

#include "glm/glm.hpp"

#include "processedinputline.h"
//#include "geometry.h"
//#include "transformation.h"
#include "shape.h" // use instead of VBOs right now

// Border macros
#define BORDER_COLOR  glm::vec4(1.0,0.5,0.0,1.0) // Red-orange
#define BORDER_HEIGHT 0.1f
#define BORDER_THICKNESS 0.01f

// Using
using std::vector;
using glm::vec3;

class Border{

public:
    // Constructors/Destructor
	Border(ProcessedInputLine* inLine);
    ~Border();

    // Print info
    void printInfo();

	// Access methods
	int getID();
	vector<Shape*> getShapes(); // Returns pointer to shapes vector
	vector<Shape*> getInwardShapes(); //Returns only shapes for inward-facing borders

    void setID(int newID);

private:
    int tileID;
    //Geometry* geometry;
    //Transformation* transform;
	vector<Shape*> shapes; // first shape should be used for collision (inward facing normal)

    void addBorderShapes(vector<vec3> verts, vector<int> neighborIDs, vec4 color = BORDER_COLOR, float height = BORDER_HEIGHT, float thickness = BORDER_THICKNESS);
};

#endif