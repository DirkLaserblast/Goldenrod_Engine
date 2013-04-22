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
	initializeGraphics(argc, argv, "MiniGolf", 1280, 720);

	// Clean-up
	delete IO;
	delete game;

	return 0;
}

