#include "physics.h"

// Constructors/Destructor
Physics::Physics(vec3 pos, vec3 dir, double speed){

    // Set initial values for speed, position, and direction    
    this->position = pos;
    this->initialPosition = pos;
    this->direction = dir;
    this->initialDirection = dir;
    this->speed = speed;
    this->initialSpeed = speed;

};

Physics::~Physics(){

};

// Print info
void Physics::printInfo(){

};

// Access methods
double Physics::getSpeed(){ return this->speed; };

vec3 Physics::getPosition(){ return this->position; };

vec3 Physics::getDirection(){ return this->direction; };

double Physics::getInitialSpeed(){ return this->initialSpeed; };

vec3 Physics::getInitialPosition(){ return this->initialPosition; };

vec3 Physics::getInitialDirection(){ return this->initialDirection; };

vec3 Physics::getVelocity(){

	return vec3(this->direction.x * this->speed,
				this->direction.y * this->speed,
				this->direction.z * this->speed);

};

vec3 Physics::getNextPosition(){

    vec3 currVelocity = this->getVelocity();
	return vec3(this->position.x + currVelocity.x,
                this->position.y + currVelocity.y,
                this->position.z + currVelocity.z);

};

void Physics::setSpeed(double newSpeed){ this->speed = newSpeed; };

void Physics::setPosition(vec3 newPosition){ this->position = newPosition; };

void Physics::setDirection(vec3 newDirection){ this->direction = normalize(newDirection); };

// Update method
void Physics::updatePosition(){

	vec3 currVelocity = this->getVelocity();
	this->position = vec3(this->position.x + currVelocity.x,
                          this->position.y + currVelocity.y,
                          this->position.z + currVelocity.z);

};

// Reset
void Physics::reset(){

    this->speed = this->initialSpeed;
    this->position = this->initialPosition;
    this->direction = this->initialDirection;

};
