#include "graphics.h"
#include "fileiocontroller.h"
#include "gameiocontroller.h"
#include "gamecontroller.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	// Initialize game engine
	FileIOController* fileIO = new FileIOController();
	GameIOController* gameIO = new GameIOController();
	GameController* game = new GameController();

    fileIO->createLevelFromFile(game, "hole.00.db"); // test code

	initializeGraphics(argc, argv, "MiniGolf", 1280, 720);

	// Clean-up
	delete fileIO;
	delete gameIO;
	delete game;

	return 0;
}

