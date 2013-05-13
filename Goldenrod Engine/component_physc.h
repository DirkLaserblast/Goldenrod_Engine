#ifndef COMPONENT_PHYSC_H
#define COMPONENT_PHYSC_H

#include "glm/glm.hpp"

#include "component.h"

class Physc: public Component{

public:
    Physc();
    ~Physc();
    void update(float friction);
    void printInfo();

    glm::vec3 getDirection();
    void setDirection(glm::vec3 newDirection);

    glm::vec3 getPosition();
    void setPosition(glm::vec3 newPosition);

    double getSpeed();
    void setSpeed(double newSpeed);

private:
    glm::vec3 direction; // normalized vector to hold direction
    glm::vec3 position; // loc of center point
    double speed;

};

#endif