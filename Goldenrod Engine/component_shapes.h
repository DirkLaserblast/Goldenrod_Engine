#ifndef COMPONENT_SHAPES_H
#define COMPONENT_SHAPES_H

#include <math.h>

#include "component.h"
#include "shape.h"

class Shapes: public Component{

public:
    Shapes();
    ~Shapes();
    void update();
    void printInfo();

    // Used for tiles, tee, and cup
    void addWedgeShapes(vector<glm::vec3> verts, glm::vec4 color, float depth);

    // Used for borders
    void addBorderShapes(vector<glm::vec3> verts, glm::vec4 color, vector<int> neighborIDs, float height, float thickness);

    vector<Shape*> getShapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs

private:
    vector<Shape*> shapes;

    void generateWedgeShapes(vector<Shape*>& shapes, vector<glm::vec3> verts, glm::vec4 color, float depth);

    void generateBorderShapes(vector<Shape*>& shapes, vector<glm::vec3> verts, glm::vec4 color, vector<int> neighborIDs, float height, float thickness);

};

#endif