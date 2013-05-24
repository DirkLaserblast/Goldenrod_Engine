#ifndef FILEIOCONTROLLER_H
#define FILEIOCONTROLLER_H

#include <iostream>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "processedinputline.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

/* fileIOController: handles file IO; creates tiles for level from file */
class FileIOController{

public:
    FileIOController();
    ~FileIOController();

    int getNumHoles();
    vector<ProcessedInputLine*> getCurrentHole();
	vector<ProcessedInputLine*> getHole(int index);

	void processFile(string fileName);
	bool nextHole();
    bool prevHole();
    bool goToHole(int index);

private:
	int currentHoleIndex;
    vector<ProcessedInputLine*> currentHole;
	vector<vector<ProcessedInputLine*>> parsedHoles;
    
    // Make new input line and push onto current level vector
	void createInputLine(string keyword, Tokenizer& str);

};

#endif