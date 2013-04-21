#ifndef GAMEIOCONTROLLER_H

#define GAMEIOCONTROLLER_H

#include <cstdlib>
#include <iostream>

using namespace std;

class IOController; // forward declare

/* gameIOContoller: handles game IO such as user interaction */
class GameIOController{

private:
	GameIOController();
	~GameIOController();
	friend class IOController;
};

#endif