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
    void processFile(string fileName);
    vector<ProcessedInputLine*>* getCurrentFile();

private:
    vector<ProcessedInputLine*>* currentFile;
    
};

#endif