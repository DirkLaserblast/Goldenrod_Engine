#ifndef SCORECONTROLLER_H
#define SCORECONTROLLER_H

#include <iostream>
#include <vector>

#include "glm/glm.hpp"

#include "sound.h"
#include "levelcontroller.h"

using glm::vec3;
using std::vector;

void updateTotalScore(int& totalScore, int& currentHoleScore);

void reset(LevelController* levelController, FileIOController* fileIO);

#endif