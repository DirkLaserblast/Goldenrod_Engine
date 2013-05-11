#ifndef COMPONENT_SHAPES_H
#define COMPONENT_SHAPES_H

#include "component.h"
#include "shape.h"

class Shapes: public Component{

public:
    Shapes();
    ~Shapes();
    void update();
    void printInfo();
    void addTileShapes(vector<glm::vec3> verts, glm::vec4 color, float depth);

private:
    vector<Shape*> shapes;

    vector<Shape*> generateTileShapes(vector<glm::vec3> verts, glm::vec4 color, float depth);

};

#endif