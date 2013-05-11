#include <cstdlib>
#include <iostream>

// Game Object Classes
#include "entity.h"
#include "system.h"
#include "system_levelcontroller.h"
#include "component.h"
#include "fileiocontroller_conversion.h"

// Systems


// Components
#include "component_VBO.h"

int main(int argc, char **argv)
{
	
    // Setup
    Entity* myEntity = new Entity();
    LevelController* myLevelController = new LevelController();

    // Test Entity
    myEntity->attachComponent(new VBO());
    myEntity->printInfo();
    myEntity->deleteComponent(cVBO_T);
    myEntity->printInfo();

    // Test Level Controller
    myLevelController->addLevel(FileIOController::processFile("hole.01.db"));

    int wait;
    cin >> wait;

	return 0;

}