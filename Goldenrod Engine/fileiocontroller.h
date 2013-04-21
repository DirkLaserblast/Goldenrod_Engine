#ifndef FILEIOCONTROLLER_H

#define FILEIOCONTROLLER_H

#include <cstdlib>
#include <iostream>

using namespace std;

class IOController; // forward declare

/* fileIOController: handles file IO; creates tiles for level from file */
class FileIOController{

private:
	FileIOController();
	~FileIOController();
	friend class IOController;

};

#endif