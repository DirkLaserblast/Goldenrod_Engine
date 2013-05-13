#include <cstdlib>
#include <iostream>

// Entity
#include "entity.h"

// System
#include "system.h"
#include "system_levelcontroller.h"

// Component
#include "component.h"
#include "component_VBO.h"
#include "component_shapes.h"

// IO
#include "fileiocontroller_conversion.h"

int main(int argc, char **argv)
{
	
    // Setup
    Entity* myEntity = new Entity();
    LevelController* myLevelController = new LevelController();
    FileIOController* myFileIOController = new FileIOController();

    // Test Entity
    myEntity->attachComponent(new VBO());
    myEntity->printInfo();
    myEntity->deleteComponent(cVBO_T);
    myEntity->printInfo();

    // Test FileIO Controller
    myFileIOController->processFile("hole.00.db");

    // Test Level Controller
    myLevelController->addLevel(myFileIOController->getCurrentFile());
    
    myFileIOController->processFile("hole.01.db");
    myLevelController->addLevel(myFileIOController->getCurrentFile());

    int wait;
    cin >> wait;

	return 0;

}