#include "component_physc.h"

Physc::Physc(){

    this->name = "NONE";
    this->type = cPHYSC_T;
    this->direction = glm::vec3(0.0,0.0,0.0);
    this->position = glm::vec3(0.0,0.0,0.0);
    this->speed = 0;

};

Physc::~Physc(){

};

void Physc::update(float friction){

    // May need to update or normalize direction, but not sure how...
    this->speed -= friction;
    if(this->speed < 0){
        this->speed = 0;
    }

    float newX = this->position.x + (this->direction.x * this->speed);
    float newY = this->position.y + (this->direction.y * this->speed);
    float newZ = this->position.z + (this->direction.z * this->speed);

    this->position = glm::vec3(newX,newY,newZ);

};

void Physc::printInfo(){

    this->Component::printInfo();

    // Print info specific to Physc

};

glm::vec3 Physc::getDirection(){ return this->direction; };

void Physc::setDirection(glm::vec3 newDirection){ this->direction = newDirection; };

glm::vec3 Physc::getPosition(){ return this->position; };

void Physc::setPosition(glm::vec3 newPosition){ this->position = newPosition; };

double Physc::getSpeed(){ return this->speed; };

void Physc::setSpeed(double newSpeed){ this->speed = newSpeed; };
