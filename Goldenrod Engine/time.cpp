//Time functions: updates and returns deltaTime, to be used with physics updates, etc.

#include "time.h"

Timer::Timer()
{
	this->oldTime = clock();
}

clock_t Timer::delta()
{
	clock_t delta = clock() - this->oldTime;
	this->oldTime = delta;
	return delta;
}