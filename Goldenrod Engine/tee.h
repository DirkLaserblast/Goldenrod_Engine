#ifndef TEE_H
#define TEE_H

// Includes
#include <vector>

#include "glm/glm.hpp"

#include "processedinputline.h"
//#include "geometry.h"
//#include "transformation.h"
#include "shape.h" // use instead of VBOs right now
#include "physics.h"

// Tee macros
#define TEE_COLOR  glm::vec4(0.0,0.0,1.0,1.0) // BLUE
//#define TEE_DEPTH 0.0f
#define TEE_WIDTH 0.2f // Currently a square
#define TEE_HEIGHT 0.2f
#define TEE_OFFSET 0.001f

// Using
using std::vector;
using glm::vec3;
using glm::vec4;

class Tee{

public:
    // Constructors/Destructor
	Tee(ProcessedInputLine* inLine);
    ~Tee();

    // Print info
    void printInfo();

	// Access methods
	int getID();
    int getCurrentTileID();
	vector<Shape*> getShapes(); // Returns pointer to shapes vector
    Physics* getPhysics();

	void setID(int newID);
    void setCurrentTileID(int newID);

private:
    // Tile data
    int teeID;
    int currentTileID;
	vector<Shape*> shapes;
    Physics* physics;

    // Private methods
    void addRectangleShapes(vec3 point, vec4 color = TEE_COLOR, float width = TEE_WIDTH, float height = TEE_HEIGHT, float offset = TEE_OFFSET);
};

#endif