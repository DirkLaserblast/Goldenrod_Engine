#ifndef TIME_H
#define TIME_H

#include <ctime>
using namespace std;

class Timer
{
public:
	Timer();
	clock_t delta(); //Returns the change in time since the last call to delta
private:
	clock_t oldTime;
};

#endif