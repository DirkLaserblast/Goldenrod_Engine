#include "fileiocontroller.h"

FileIOController::FileIOController(){

};

FileIOController::~FileIOController(){

};

bool FileIOController::createLevelFromFile(GameController* game, string levelFile){

	ifstream inFile(levelFile);

	if(inFile.is_open()){
		// Parse file contents

		// Add level to game
		return game->addLevel();
	}
	else{
		cerr << "Could not open " << levelFile.c_str() << " for reading" << endl;
        return false;
	}

};

void FileIOController::parseFileContent(ifstream inFile){



};
