#ifndef IOCONTROLLER_H

#define IOCONTROLLER_H

#include <cstdlib>
#include <iostream>

#include "gameiocontroller.h"
#include "fileiocontroller.h"

using namespace std;

/* IOContoller: contains and maintains file and game IO classes*/
class IOController{
	
public:
	IOController();
	~IOController();


private:
	GameIOController* gameIO;
	FileIOController* fileIO;

};

#endif