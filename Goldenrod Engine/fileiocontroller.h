#ifndef FILEIOCONTROLLER_H
#define FILEIOCONTROLLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "gamecontroller.h"
#include "Tokenizer.h"

using namespace std;

/* fileIOController: handles file IO; creates tiles for level from file */
class FileIOController{

public:
	FileIOController();
	~FileIOController();

	bool createLevelFromFile(GameController* game, string levelFile);

private:
    /* InputData: helper class that temporarily contains input data */
    class InputData {

    public:
        string keyword;
        int ID;
        int numEdges;
        int numNeighbors;
        vector<glm::vec3> verts;
        vector<int> neighborIDs;

        InputData();
        ~InputData();
        void clear();
        void setTileData(Tokenizer &str);
        void setTeeData(Tokenizer &str);
        void setCupData(Tokenizer &str);

    };
    
};

#endif