#ifndef COLLISIONCONTROLLER_H
#define COLLISIONCONTROLLER_H

#define DEBUG_WALL_PAINT false
#define COLLISION_DAMP_FACTOR 0.95f

#include <vector>

#include "glm/glm.hpp"

#include "sound.h"
#include "levelcontroller.h"

using glm::vec3;
using std::vector;

bool detectCollisions(Tile* currentTile, Physics * physics, SoundEngine* sound);

#endif
