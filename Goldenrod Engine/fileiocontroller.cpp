#include "fileiocontroller.h"

FileIOController::FileIOController(){

};

FileIOController::~FileIOController(){

};

bool FileIOController::createLevelFromFile(GameController* game, string levelFile){

	ifstream inFile(levelFile);
    string line;
    stringstream os;
    int keyword,
        ID;

	if(inFile.is_open()){
        cerr << "Successfully opened " << levelFile.c_str() << " for reading." << endl;
		// Add new level to game (pushed to the back of the game levels vector)
		game->addLevel();

		// Loop to parse file contents and add tiles to new level
        while(inFile.good()){
            getline(inFile, line);
            os.str(line);

        }

		// Check if a valid level was created
        return game->checkValidCurrentLevel();
	}
	else{
		cerr << "Could not open " << levelFile.c_str() << " for reading." << endl;
        return false;
	}

};

void FileIOController::parseFileContent(ifstream inFile){



};
