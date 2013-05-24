#include "tee.h"

// Constructors/Destructor
Tee::Tee(ProcessedInputLine* inLine){

    this->teeID = inLine->getID();
    this->addRectangleShapes(inLine->getVerts()[0]);
    this->physics = new Physics(inLine->getVerts()[0]);

};

Tee::~Tee(){

    for(int i = 0; i < this->shapes.size(); i++){
        delete this->shapes.at(i);
    }
    this->shapes.clear();
    delete this->physics;

};

// Print info
void Tee::printInfo(){

};

// Access methods
int Tee::getID(){ return this->teeID; };

int Tee::getCurrentTileID(){ return this->currentTileID; };

vector<Shape*> Tee::getShapes(){ return this->shapes; };

Physics* Tee::getPhysics(){ return this->physics; };

void Tee::setID(int newID){ this->teeID = newID; };

void Tee::setCurrentTileID(int newID){ this->currentTileID = newID; };

// Private methods
void Tee::addRectangleShapes(vec3 point, vec4 color, float width, float height, float offset){

    vector<glm::vec3> rectVerts;

    rectVerts.push_back(glm::vec3(point.x - (width/2), point.y + offset, point.z + (height/2)));
    rectVerts.push_back(glm::vec3(point.x + (width/2), point.y + offset, point.z + (height/2)));
    rectVerts.push_back(glm::vec3(point.x + (width/2), point.y + offset, point.z - (height/2)));
    rectVerts.push_back(glm::vec3(point.x - (width/2), point.y + offset, point.z - (height/2)));

   this->shapes.push_back(new Shape(rectVerts, color));

};
