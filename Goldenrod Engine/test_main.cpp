#include <cstdlib>
#include <iostream>

// Game Object Classes
#include "entity.h"
#include "system.h"
#include "component.h"

// Systems


// Components
#include "VBO.h"

int main(int argc, char **argv)
{
	
    Entity* myEntity = new Entity();
    myEntity->attachComponent(new VBO());
    myEntity->printInfo();
    myEntity->deleteComponent(cVBO_T);
    myEntity->printInfo();

    int wait;
    cin >> wait;

	return 0;

}