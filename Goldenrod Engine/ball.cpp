#include "ball.h"

// Constructors/Destructor
Ball::Ball(ProcessedInputLine* inLine){

    this->currentTileID = inLine->getID();
    this->addBallShapes(inLine->getVerts()[0]);
    this->physics = new Physics(inLine->getVerts()[0]);

};

Ball::~Ball(){

    for(int i = 0; i < this->shapes.size(); i++){
        delete this->shapes.at(i);
    }
    this->shapes.clear();
    delete this->physics;

};

// Print info
void Ball::printInfo(){

};

// Access methods
//int Ball::getID(){ return this->ballID; };

int Ball::getCurrentTileID(){ return this->currentTileID; };

vector<Shape*> Ball::getShapes(){ return this->shapes; };

Physics* Ball::getPhysics(){ return this->physics; };

//void Ball::setID(int newID){ this->ballID = newID; };

void Ball::setCurrentTileID(int newID){ this->currentTileID = newID; };

// Private methods
void Ball::addBallShapes(vec3 point, vec4 color, float radius, float offset){

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
