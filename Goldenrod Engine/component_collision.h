#ifndef COMPONENT_COLLISION_H
#define COMPONENT_COLLISION_H

#include <vector>

#include "glm/glm.hpp"

#include "component.h"
#include "shape.h"

class Collision: public Component{

public:
    Collision();
    Collision(glm::vec3 center, vector<glm::vec3> collisionVerts);
    ~Collision();
    void update();
    void printInfo();

    void addBorderCollisionShapes(vector<glm::vec3> collisionVerts, float height, float thickness);
    void addBallCollisionShapes(vector<glm::vec3> collisionVerts, glm::vec4 color, float depth);

private:
    glm::vec3 center;
    vector<glm::vec3> collisionVerts;
    vector<Shape*> collisionShapes; // WILL NEED TO CHANGE WHEN USE VBOs
    
    void generateBorderCollisionShapes(vector<Shape*>& shapes, vector<glm::vec3> collisionVerts, float height, float thickness);

    void generateBallCollisionShapes(vector<Shape*>& shapes, vector<glm::vec3> collisionVerts, glm::vec4 color, float depth);

};

#endif