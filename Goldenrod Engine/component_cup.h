#ifndef COMPONENT_CUP_H
#define COMPONENT_CUP_H

#include "component.h"

#define CUP_COLOR  glm::vec4(1.0,0.0,0.0,1.0) // RED
#define CUP_DEPTH 0.0f
#define CUP_WIDTH 0.1f // Currently a square
#define CUP_HEIGHT 0.1f
#define CUP_OFFSET 0.001f

class Cup: public Component{

public:
    Cup();
    ~Cup();
    void update();
    void printInfo();

private:
    
};

#endif