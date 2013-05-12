#ifndef COMPONENT_BALL_H
#define COMPONENT_BALL_H

#include "component.h"

#define BALL_COLOR glm::vec4(1.0,1.0,1.0,1.0) // WHITE

class Ball: public Component{

public:
    Ball();
    ~Ball();
    void update();
    void printInfo();

private:
    
};

#endif