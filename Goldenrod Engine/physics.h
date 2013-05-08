#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>

class Physics
{
public:
	Physics();
	void tick(clock_t deltaTime);
private:
};

//Finds which tile the point is om
//StartingTile is the tile the object was on last, and is checked first
Tile* onTile(vec3 position, Tile* startingTile)
{

}

#endif