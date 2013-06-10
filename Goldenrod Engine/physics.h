#ifndef PHYSICS_H
#define PHYSICS_H

// Includes
#include <vector>

#include "glm/glm.hpp"

#include "shape.h" // use instead of VBOs right now

// Physics macros
#define PHYSICS_DEFAULT_POSITION glm::vec3(0.0,0.0,0.0)
#define PHYSICS_DEFAULT_DIRECTION glm::vec3(0.0,0.0,0.0)
#define PHYSICS_DEFAULT_SPEED 0.0L

// Using
using std::vector;
using glm::vec3;
using glm::normalize;

class Physics{

public:
    Physics(vec3 pos = PHYSICS_DEFAULT_POSITION, vec3 dir = PHYSICS_DEFAULT_DIRECTION, double speed = PHYSICS_DEFAULT_SPEED);
    ~Physics();

	// Print info
    void printInfo();

	// Access methods
	double getSpeed();
	vec3 getPosition();
	vec3 getDirection();
    double getInitialSpeed();
	vec3 getInitialPosition();
	vec3 getInitialDirection();
	vec3 getVelocity();
	vec3 getNextPosition();

	void setSpeed(double newSpeed);
	void setPosition(vec3 newPosition);
	void setPositionX(float newX);
	void setPositionY(float newY);
	void setPositionZ(float newZ);
	void setDirection(vec3 newDirection);
	void flipYDirection();

	// Update method
	void updatePosition();

    // Reset to initial state
    void reset();

private:
	double speed;
	vec3 position;
	vec3 direction;

    double initialSpeed;
    vec3 initialPosition;
    vec3 initialDirection;

};

#endif