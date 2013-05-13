#ifndef COMPONENT_COLLISION_H
#define COMPONENT_COLLISION_H

#include <vector>

#include "glm/glm.hpp"

#include "component.h"
#include "shape.h"

#define COLLISION_SHAPE_COLOR glm::vec4(0.0,0.0,0.0,0.0) // invisible

class Collision: public Component{

public:
    Collision();
    ~Collision();
    void update();
    void printInfo();

    void addBorderCollisionShapes(vector<glm::vec3> verts, glm::vec4 color, vector<int> neighborIDs, float height, float thickness);

    void addWedgeCollisionShapes(vector<glm::vec3> verts, glm::vec4 color, float depth);

    void addBallCollisionShapes(vector<glm::vec3> verts, glm::vec4 color);

private:
    vector<Shape*> collisionShapes; // WILL NEED TO CHANGE WHEN USE VBOs

    // Two shapes made, normals face the same direction
    void generateBorderCollisionShapes(vector<Shape*>& shapes, vector<glm::vec3> verts, glm::vec4 color, vector<int> neighborIDs, float height, float thickness);

    void generateWedgeCollisionShapes(vector<Shape*>& shapes, vector<glm::vec3> verts, glm::vec4 color, float depth);

};

#endif