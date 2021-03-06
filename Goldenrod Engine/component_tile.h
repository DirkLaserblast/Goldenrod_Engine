#ifndef COMPONENT_TILE_H
#define COMPONENT_TILE_H

#include <vector>

#include "entity.h"
#include "component.h"

#define TILE_COLOR glm::vec4(0.0,1.0,0.0,1.0) // GREEN
#define TILE_DEPTH 0.1f
#define TILE_USE_BORDER true // Make borders when adding tiles
#define TILE_FRICTION 0.0001L

class Tile: public Component{

public:
    Tile();
    Tile(int tileID, vector<int> neighborIDs);
    ~Tile();
    void update();
    void printInfo();

    float getFriction();
    void setFriction(float newFriction);

    vector<int> getNeighborIDs();
	Entity* borders;

private:
    int tileID; // This is not being used for the tile ID currently; using collision component
    float friction;
    vector<int>neighborIDs;	
};

#endif