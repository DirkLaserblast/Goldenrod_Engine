#ifndef COMPONENT_TILE_H
#define COMPONENT_TILE_H

#include "component.h"

#define TILE_COLOR glm::vec4(0.0,1.0,0.0,1.0) // GREEN
#define TILE_DEPTH 0.1f
#define TILE_USE_BORDER true // Make borders when adding tiles

class Tile: public Component{

public:
    Tile();
    ~Tile();
    void update();
    void printInfo();

private:
    
};

#endif