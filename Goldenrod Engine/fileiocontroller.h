#ifndef FILEIOCONTROLLER_H
#define FILEIOCONTROLLER_H

#include <cstdlib>
#include <iostream>
#include <fstream>

#include "gamecontroller.h"

using namespace std;

/* fileIOController: handles file IO; creates tiles for level from file */
class FileIOController{

public:
	FileIOController();
	~FileIOController();

	bool createLevelFromFile(GameController* game, string levelFile);

private:

};

#endif