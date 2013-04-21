#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <cstdlib>
#include <vector>

#include "level.h"

using namespace std;

class GameController{

public:
	GameController();
	~GameController();

private:
	vector<Level> levels;

};

#endif
