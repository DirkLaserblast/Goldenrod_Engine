#ifndef FILEIOCONTROLLER_H
#define FILEIOCONTROLLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "gamecontroller.h"

using namespace std;

/* fileIOController: handles file IO; creates tiles for level from file */
class FileIOController{

public:
	FileIOController();
	~FileIOController();

	bool createLevelFromFile(GameController* game, string levelFile);

private:
    void parseFileContent(ifstream inFile);
};

#endif