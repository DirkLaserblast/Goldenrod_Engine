#ifndef COMPONENT_BORDER_H
#define COMPONENT_BORDER_H

#include "component.h"

#define BORDER_COLOR  glm::vec4(1.0,0.5,0.0,1.0) // Red-orange
#define BORDER_HEIGHT 0.2f
#define BORDER_THICKNESS 0.01f

class Border: public Component{

public:
    Border(int tileID);
    ~Border();
    void update();
    void printInfo();

    int getAttachedTileID();

private:
    int attachedTileID;
};

#endif