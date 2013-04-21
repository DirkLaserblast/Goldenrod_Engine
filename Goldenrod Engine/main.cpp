#include "graphics.h"
#include "iocontroller.h"
#include "gamecontroller.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	// Initialize game engine
	IOController* IO = new IOController();
	GameController* game = new GameController();
	initializeGraphics(argc, argv);

	// Clean-up
	delete IO;
	delete game;

	return 0;
}

