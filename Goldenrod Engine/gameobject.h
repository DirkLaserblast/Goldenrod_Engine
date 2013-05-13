#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <iostream>

#define NONE "NONE"

using namespace std;

class GameObject{

public:
	virtual void update() =0; // abstract function
    virtual void printInfo();

protected:
    string name; // Assigned name for object
    
};

#endif