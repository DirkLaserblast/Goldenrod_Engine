#ifndef TILEBORDER_H
#define TILEBORDER_H

#include <vector>

#include "shape.h"

#define BORDER_COLOR glm::vec4(0.0,1.0,0.0,1.0)
#define BORDER_OFFSET 0.5f
#define BORDER_RADIUS 0.1f

using namespace std;

class Border{

public:
    // Functions
	Border(int ID, int numShapes);
	~Border();

    void validate();
	bool isValid();

    int getID();

    int getNumShapes();
    int getLocIndex();
    int getColIndex();
    int getNormIndex();
    int getShapeIndex();
	vector<Shape*> shapesPointer;

private:
    // Local Variables
	bool valid;
	int ID;
    int numShapes,
        locIndex,
        colIndex,
        normIndex,
        shapeIndex;

};

#endif