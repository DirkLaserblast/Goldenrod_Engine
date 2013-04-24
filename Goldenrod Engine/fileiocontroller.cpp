#include "fileiocontroller.h"

FileIOController::FileIOController(){

};

FileIOController::~FileIOController(){

};

bool FileIOController::createLevelFromFile(GameController* game, string levelFile){

	ifstream inFile(levelFile);
    string line;
    Tokenizer str;
    InputData data;

	if(inFile.is_open()){
        cerr << "Successfully opened " << levelFile.c_str() << " for reading." << endl;
		// Add new level to game (pushed to the back of the game levels vector)
		game->addLevel();

		// Loop to parse file contents and add tiles to new level
        while(inFile.good()){
            getline(inFile, line); // get next line of input file
            str.setString(line); // attach string to tokenizer

            // Determine what kind of object and add to level
            data.keyword = str.nextEcho(); // determine keyword
            if(data.keyword == "tile"){

                data.setTileData(str);
                game->addTileCurrentLevel(data.ID, data.numEdges, data.numNeighbors, data.verts, data.neighborIDs);

            }
            else if(data.keyword == "tee"){

                data.setTeeData(str);
                game->addTeeCurrentLevel(data.ID, data.verts[0]);

            }
            else if(data.keyword == "cup"){

                data.setCupData(str);
                game->addCupCurrentLevel(data.ID, data.verts[0]);

            }
            else if(data.keyword == ""){

                cout << "End of file." << endl << endl;

            }
            else{

                cout << "Invalid keyword." << endl;

            }

            // Reset data
            data.clear();

        }


		// Check if a valid level was created
        return game->checkValidCurrentLevel();
	}
	else{
		cerr << "Could not open " << levelFile.c_str() << " for reading." << endl;
        return false;
	}

};

FileIOController::InputData::InputData(){

    this->keyword = "unset";
    this->ID = 0;
    this->numEdges = 0;
    this->numNeighbors = 0;
    this->verts.clear();
    this->neighborIDs.clear();

};

FileIOController::InputData::~InputData(){

    this->verts.clear();
    this->neighborIDs.clear();

};

void FileIOController::InputData::clear(){

    this->keyword = "unset";
    this->ID = 0;
    this->numEdges = 0;
    this->numNeighbors = 0;
    this->verts.clear();
    this->neighborIDs.clear();

};

void FileIOController::InputData::setTileData(Tokenizer &str){

    ID = atoi(str.next().c_str()); // there has to be a better way to convert strings to int...
    numEdges = atoi(str.next().c_str());

    // Get position data
    int tmpNeighborID;
    float x, y, z;
    for(int i = 0; i < numEdges; i++){
        x = atof(str.next().c_str());
        y = atof(str.next().c_str());
        z = atof(str.next().c_str());
        verts.push_back(glm::vec3(x, y, z));
    }

    // Get neighbor data
    for(int i = 0; i < numEdges; i++){
        tmpNeighborID = atoi(str.next().c_str());
        neighborIDs.push_back(tmpNeighborID);
        if(tmpNeighborID > 0){
            numNeighbors++;
        }
    }

};

void FileIOController::InputData::setTeeData(Tokenizer &str){

    ID = atoi(str.next().c_str()); // there has to be a better way to convert strings to int...
    //verts.push_back(glm::vec3(atof(str.next().c_str()), atof(str.next().c_str()), atof(str.next().c_str())));

    // Debug test code
    float x = atof(str.next().c_str());
    float y = atof(str.next().c_str());
    float z = atof(str.next().c_str());
    verts.push_back(glm::vec3(x,y,z));

};

void FileIOController::InputData::setCupData(Tokenizer &str){

    ID = atoi(str.next().c_str()); // there has to be a better way to convert strings to int...
    //verts.push_back(glm::vec3(atof(str.next().c_str()), atof(str.next().c_str()), atof(str.next().c_str())));

    // Debug test code
    float x = atof(str.next().c_str());
    float y = atof(str.next().c_str());
    float z = atof(str.next().c_str());
    verts.push_back(glm::vec3(x,y,z));

};
