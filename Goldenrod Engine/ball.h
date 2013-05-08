#ifndef BALL_H
#define BALL_H

#include <vector>

#include "shape.h"

class Ball
{
public:
	Ball(vec3 position, vec3 direction, float radius);
	void translate(vec3 trans);
	vec3 position(); //Position of center of bottom of ball
	void position(vec3 pos);
	float velocity();

private:
	vec3 ballPosition;
	vec3 ballDirection;
	float ballVelocity;
};

#endif