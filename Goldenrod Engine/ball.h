#ifndef BALL_H
#define BALL_H

// Includes
#include <vector>

#include "glm/glm.hpp"

#include "processedinputline.h"
//#include "geometry.h"
//#include "transformation.h"
#include "shape.h" // use instead of VBOs right now
#include "physics.h"

// Ball macros
#define BALL_COLOR glm::vec4(1.0,1.0,1.0,1.0) // WHITE
#define BALL_DEPTH 0.0f // Currently unused
#define BALL_RADIUS 0.035f
#define BALL_OFFSET 0.01f

// Using
using std::vector;
using glm::vec3;
using glm::vec4;

class Ball{

public:
    // Constructors/Destructor
	Ball(ProcessedInputLine* inLine);
    ~Ball();

    // Print info
    void printInfo();

	// Access methods
	//int getID();
    int getCurrentTileID();
    int getInitialTileID();
	vector<Shape*> getShapes(); // Returns pointer to shapes vector
    Physics* getPhysics();

	//void setID(int newID);
    void setCurrentTileID(int newID);

    // Reset ball
    void reset();

private:
    // Tile data
    //int ballID;
    int currentTileID;
    int initialTileID;
	vector<Shape*> shapes;
    Physics* physics;

    // Private methods
    void addBallShapes(vec3 point, vec4 color = BALL_COLOR, float radius = BALL_RADIUS, float offset = BALL_OFFSET);
};

#endif