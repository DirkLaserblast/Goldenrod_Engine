#ifndef PROCESSEDINPUTLINE_H
#define PROCESSEDINPUTLINE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

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

    void setCourse(Tokenizer &str);
	void setHoleName(Tokenizer &str);
	void setHolePar(Tokenizer &str);
    void setTileData(Tokenizer &str);
    void setTeeData(Tokenizer &str);
    void setCupData(Tokenizer &str);

    string getKeyword();
	string getName();
    int getID();
	int getPar();
    int getNumHoles();
    int getNumEdges();
    int getNumNeighbors();
    vector<glm::vec3> getVerts();
    vector<int> getNeighborIDs();

private:
    string keyword;
	string name;
    int ID;
	int par;
    int numHoles;
    int numEdges;
    int numNeighbors;
    vector<glm::vec3> verts;
    vector<int> neighborIDs;

};

#endif