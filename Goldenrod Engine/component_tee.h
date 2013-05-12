#ifndef COMPONENT_TEE_H
#define COMPONENT_TEE_H

#include "component.h"

#define TEE_COLOR  glm::vec4(0.0,0.0,1.0,1.0) // BLUE
#define TEE_DEPTH 0.0f
#define TEE_WIDTH 0.1f // Currently a square
#define TEE_HEIGHT 0.1f
#define TEE_OFFSET 0.001f

class Tee: public Component{

public:
    Tee();
    ~Tee();
    void update();
    void printInfo();

private:
    
};

#endif