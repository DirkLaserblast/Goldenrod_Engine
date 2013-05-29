#include "ball.h"

// Constructors/Destructor
Ball::Ball(ProcessedInputLine* inLine){

    this->currentTileID = inLine->getID();
    this->initialTileID = inLine->getID();
    this->addBallShapes(inLine->getVerts()[0]);

    // Correct for ball offset
    vec3 tmpVerts = inLine->getVerts()[0];
    tmpVerts = vec3(tmpVerts.x, tmpVerts.y + BALL_OFFSET, tmpVerts.z);
    this->physics = new Physics(tmpVerts);

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

int Ball::getInitialTileID(){ return this->initialTileID; };

vector<Shape*> Ball::getShapes(){ return this->shapes; };

Physics* Ball::getPhysics(){ return this->physics; };

//void Ball::setID(int newID){ this->ballID = newID; };

void Ball::setCurrentTileID(int newID){ this->currentTileID = newID; };

void Ball::reset(){

    // Translate shape back to origin
    this->shapes[0]->translate(-(this->physics->getPosition()));

    // Reset physics
    this->physics->reset();

    // Translate shape to initial position
    this->shapes[0]->translate(this->physics->getPosition());

    // Reload ball shape
    this->shapes[0]->reload();

    // Reset current tile
    this->currentTileID = this->initialTileID;

};

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
