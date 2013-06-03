#include "cup.h"

// Constructors/Destructor
Cup::Cup(ProcessedInputLine* inLine){

    //this->cupID = inLine->getID();
    this->currentTileID = inLine->getID();
    this->addCircleShapes(inLine->getVerts()[0]);
    this->physics = new Physics(inLine->getVerts()[0]);

};

Cup::~Cup(){

    for(int i = 0; i < this->shapes.size(); i++){
        delete this->shapes.at(i);
    }
    this->shapes.clear();
    delete this->physics;

};

// Print info
void Cup::printInfo(){

};

// Access methods
//int Cup::getID(){ return this->cupID; };

int Cup::getCurrentTileID(){ return this->currentTileID; };

vector<Shape*> Cup::getShapes(){ return this->shapes; };

Physics* Cup::getPhysics(){ return this->physics; };

//void Cup::setID(int newID){ this->cupID = newID; };

void Cup::setCurrentTileID(int newID){ this->currentTileID = newID; };

// Private methods
void Cup::addRectangleShapes(vec3 point, vec4 color, float width, float height, float offset){

    vector<glm::vec3> rectVerts;

    rectVerts.push_back(glm::vec3(point.x - (width/2), point.y + offset, point.z + (height/2)));
    rectVerts.push_back(glm::vec3(point.x + (width/2), point.y + offset, point.z + (height/2)));
    rectVerts.push_back(glm::vec3(point.x + (width/2), point.y + offset, point.z - (height/2)));
    rectVerts.push_back(glm::vec3(point.x - (width/2), point.y + offset, point.z - (height/2)));

   this->shapes.push_back(new Shape(rectVerts, color));

};

void Cup::addCircleShapes(vec3 point, vec4 color, float radius, float offset){

    vector<glm::vec3> circleVerts;

    circleVerts.push_back(glm::vec3(point.x, point.y + offset, point.z + radius));
    circleVerts.push_back(glm::vec3(point.x + 0.7*radius, point.y + offset, point.z + 0.7*radius));
    circleVerts.push_back(glm::vec3(point.x + radius, point.y + offset, point.z));
    circleVerts.push_back(glm::vec3(point.x + 0.7*radius, point.y + offset, point.z - 0.7*radius));
    circleVerts.push_back(glm::vec3(point.x, point.y + offset, point.z - radius));
    circleVerts.push_back(glm::vec3(point.x - 0.7*radius, point.y + offset, point.z - 0.7*radius));
    circleVerts.push_back(glm::vec3(point.x - radius, point.y + offset, point.z));
    circleVerts.push_back(glm::vec3(point.x - 0.7*radius, point.y + offset, point.z + 0.7*radius));

    this->shapes.push_back(new Shape(circleVerts, color));

};
