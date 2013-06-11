#ifndef PHYSICSCONTROLLER_H
#define PHYSICSCONTROLLER_H

#define DEBUG_TILE_PAINT false
#define GRAVITY_DIVISOR 10.0f

#include <iostream>
#include <vector>

#include "glm/glm.hpp"

#include "sound.h"
#include "levelcontroller.h"

using glm::vec3;
using std::vector;

vec3 updateBallDirection(LevelController* levelController, SoundEngine* sound);

#endif