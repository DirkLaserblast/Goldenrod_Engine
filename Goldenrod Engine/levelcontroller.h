#ifndef LEVELCONTROLLER_H
#define LEVELCONTROLLER_H

#include "processedinputline.h"
#include "fileiocontroller.h"
#include "level_refactor.h"

class LevelController{

public:
	// Constructors/Destructor
	LevelController();
    ~LevelController();

	// Load level
    void loadCurrentLevel(FileIOController* fileIO);
	void loadNextLevel(FileIOController* fileIO);
	void loadPrevLevel(FileIOController* fileIO);
	void loadFirstLevel(FileIOController* fileIO);
	void loadLastLevel(FileIOController* fileIO);
	void loadLevel(FileIOController* fileIO, int levelIndex);

	// Access methods
	Level* getCurrentLevel();

private:
	Level* currentLevel;

};

#endif