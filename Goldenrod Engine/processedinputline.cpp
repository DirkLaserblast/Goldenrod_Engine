#include "processedinputline.h"

// ProcessedInputLine functions
ProcessedInputLine::ProcessedInputLine(){
    
    this->keyword = "unset";
	this->name = "unset";
    this->ID = 0;
	this->par = 0;
    this->numHoles = 0;
    this->numEdges = 0;
    this->numNeighbors = 0;
    this->verts.clear();
    this->neighborIDs.clear();

};

ProcessedInputLine::~ProcessedInputLine(){
    
    this->verts.clear();
    this->neighborIDs.clear();

};

void ProcessedInputLine::clear(){
    
    this->keyword = "unset";
	this->name = "unset";
    this->ID = 0;
	this->par = 0;
    this->numHoles = 0;
    this->numEdges = 0;
    this->numNeighbors = 0;
    this->verts.clear();
    this->neighborIDs.clear();

};

void ProcessedInputLine::setCourse(Tokenizer &str){
	
	keyword = "course";

    str.publicSkipDelimiter();
    str.setDelimiter("\"");
	name = str.next(); // there may be an issue here...
    str.setDelimiter(DEFAULT_DELIMITER);

    numHoles = atoi(str.next().c_str());

};

void ProcessedInputLine::setHoleName(Tokenizer &str){
	
	keyword = "name";

    str.publicSkipDelimiter();
    str.setDelimiter("\"");
	name = str.next(); // there may be an issue here...
    str.setDelimiter(DEFAULT_DELIMITER);

};

void ProcessedInputLine::setHolePar(Tokenizer &str){
	
	keyword = "par";
	par = atoi(str.next().c_str());

};

void ProcessedInputLine::setTileData(Tokenizer &str){
    
    keyword = "tile";
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

void ProcessedInputLine::setTeeData(Tokenizer &str){
    
    keyword = "tee";
    ID = atoi(str.next().c_str()); // there has to be a better way to convert strings to int...
    //verts.push_back(glm::vec3(atof(str.next().c_str()), atof(str.next().c_str()), atof(str.next().c_str())));

    // Debug test code
    float x = atof(str.next().c_str());
    float y = atof(str.next().c_str());
    float z = atof(str.next().c_str());
    verts.push_back(glm::vec3(x,y,z));

};

void ProcessedInputLine::setCupData(Tokenizer &str){
    
    keyword = "cup";
    ID = atoi(str.next().c_str()); // there has to be a better way to convert strings to int...
    //verts.push_back(glm::vec3(atof(str.next().c_str()), atof(str.next().c_str()), atof(str.next().c_str())));

    // Debug test code
    float x = atof(str.next().c_str());
    float y = atof(str.next().c_str());
    float z = atof(str.next().c_str());
    verts.push_back(glm::vec3(x,y,z));

};

string ProcessedInputLine::getKeyword(){ return this->keyword; };

string ProcessedInputLine::getName(){ return this->name; };

int ProcessedInputLine::getID() { return this->ID; };

int ProcessedInputLine::getPar() { return this->par; };

int ProcessedInputLine::getNumHoles(){ return this->numHoles; };

int ProcessedInputLine::getNumEdges() { return this->numEdges; };

int ProcessedInputLine::getNumNeighbors() { return this->numNeighbors; };

vector<glm::vec3> ProcessedInputLine::getVerts() { return this->verts; };

vector<int> ProcessedInputLine::getNeighborIDs() { return this->neighborIDs; };
