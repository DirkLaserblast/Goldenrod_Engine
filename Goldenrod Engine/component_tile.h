#ifndef COMPONENT_TILE_H
#define COMPONENT_TILE_H

#include <vector>

#include "component.h"

#define TILE_COLOR glm::vec4(0.0,1.0,0.0,1.0) // GREEN
#define TILE_DEPTH 0.1f
#define TILE_USE_BORDER true // Make borders when adding tiles
#define TILE_FRICTION 0.05f

class Tile: public Component{

public:
    Tile();
    Tile(int tileID, vector<int> neighborIDs);
    ~Tile();
    void update();
    void printInfo();

    float getFriction();
    void setFriction(float newFriction);

private:
    int tileID;
    float friction;
    vector<int>neighborIDs;
};

#endif