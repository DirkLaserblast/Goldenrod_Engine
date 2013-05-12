#ifndef COMPONENT_SHAPES_H
#define COMPONENT_SHAPES_H

#include "component.h"
#include "shape.h"

class Shapes: public Component{

public:
    Shapes();

    /* Shapes: constructor that creates and adds shapes to component
       @param: verts to construct shapes
       @param: color of shape
       @param: depth of shape for generating "wedge" instead of flat tile
    */
    Shapes(vector<glm::vec3> verts, glm::vec4 color, float depth);

    ~Shapes();
    void update();
    void printInfo();

    vector<Shape*> getShapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs

private:
    vector<Shape*> shapes;

    void generateTileShapes(vector<Shape*>& shapes, vector<glm::vec3> verts, glm::vec4 color, float depth);

};

#endif