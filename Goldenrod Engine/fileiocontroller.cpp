#include "fileiocontroller.h"

FileIOController::FileIOController(){

};

FileIOController::~FileIOController(){

};

bool FileIOController::createLevelFromFile(GameController* game, string levelFile){

	ifstream inFile(levelFile);
    string line;
    Tokenizer str;

    string keyword;
    int ID;

	if(inFile.is_open()){
        cerr << "Successfully opened " << levelFile.c_str() << " for reading." << endl;
		// Add new level to game (pushed to the back of the game levels vector)
		game->addLevel();

		// Loop to parse file contents and add tiles to new level
        while(inFile.good()){
            getline(inFile, line); // get next line of input file
            str.setString(line); // attach string to tokenizer

            keyword = str.next(); // determine keyword

            if(keyword == "tile"){

                cout << "tile" << endl;

            }
            else if(keyword == "tee"){

                cout << "tee" << endl;

            }
            else if(keyword == "cup"){

                cout << "cup" << endl;

            }
            else if(keyword == ""){

                cout << "End of file." << endl;

            }
            else{

                cout << "Invalid keyword." << endl;

            }

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
