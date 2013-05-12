#ifndef PROCESSEDINPUTLINE_H
#define PROCESSEDINPUTLINE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "Tokenizer.h"

using std::cout;
using std::ifstream;
using std::string;
using std::vector;

/* processedInputLine: helper class that contains information from a single
                       processed line of input file */
class ProcessedInputLine {

public:
    
    ProcessedInputLine();
    ~ProcessedInputLine();
    void clear();    
    void setTileData(Tokenizer &str);
    void setTeeData(Tokenizer &str);
    void setCupData(Tokenizer &str);

    string getKeyword();
    int getID();
    int getNumEdges();
    int getNumNeighbors();
    vector<glm::vec3> getVerts();
    vector<int> getNeighborIDs();

private:
    string keyword;
    int ID;
    int numEdges;
    int numNeighbors;
    vector<glm::vec3> verts;
    vector<int> neighborIDs;

};

#endif