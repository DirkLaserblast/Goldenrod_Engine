#include "physicscontroller.h"

vec3 updateBallDirection(LevelController* levelController, SoundEngine* sound){

    // Handles
    Level *currentLevel = levelController->getCurrentLevel();
	Ball *ball = currentLevel->getBall();
	Physics *physics = ball->getPhysics();
	Tile* currentTile = currentLevel->getTile(ball->getCurrentTileID());

    vec3 newDirection = physics->getDirection();

    // Update ball's current tile and direction if moved to/from slanted tile
    // Get top of assumed current tile
    Shape* tileTop = currentLevel->getTile(ball->getCurrentTileID())->getShapes().at(0);
    // Calculate if point will be within its current tile after it is moved
    bool inTile = tileTop->checkIfInside(physics->getNextPosition(), TILE_DEFAULT_DEPTH);
    // If not in tile
    if(inTile == false)
	{
		//Play sound
		sound->getEngine()->play2D("sfx/retro_roll.wav");

		// Debug -- Stop highlighting current tile
		if (DEBUG_TILE_PAINT)
		{
			currentTile->getShapes()[0]->changeColor(TILE_DEFAULT_COLOR);
			currentTile->getShapes()[0]->reload();
		}

        //Check all tiles to find which one we are in (tried to check only neighbors above, but had issues)          
        for(int i =0; i < currentLevel->getTiles().size(); i++)
		{
            // Check tile
            inTile = currentLevel->getTiles()[i]->getShapes().at(0)->checkIfInside(physics->getNextPosition(), TILE_DEFAULT_DEPTH);
            if(inTile){
                Tile* prevTile = currentTile; // Store previous tile
				ball->setCurrentTileID(currentLevel->getTiles()[i]->getID());
                currentTile = currentLevel->getTile(ball->getCurrentTileID()); // update handle to current tile

                // If new tile is flat make sure no y-component
                if(currentTile->getShapes().at(0)->normals()[0] == glm::vec3(0.0,1.0,0.0)){
                    newDirection = glm::vec3(physics->getDirection().x, 0.0, physics->getDirection().z);
                }
                // If new tile is not flat add y-component
                else{
                    glm::vec3 oldDirection = physics->getDirection();
					glm::vec3 upVector = prevTile->getShapes()[0]->normals()[0];
                    // Get current tile normal
                    glm::vec3 tileNormal = currentLevel->getTile(ball->getCurrentTileID())->getShapes().at(0)->normals()[0];
                    glm::vec3 xVector = glm::cross(oldDirection, upVector);
                    newDirection = glm::normalize(glm::cross(tileNormal, xVector));
                }

				//cout << endl << "Changed tile." << endl; // debug
				//cout << "Old direction: " << physics->getDirection().x << "," << physics->getDirection().y << "," << physics->getDirection().z << endl;
				//cout << "New direction: " << newDirection.x << "," << newDirection.y << "," << newDirection.z << endl;
				//cout << "New roll direction: " << currentTile->getRollDirection().x << "," << currentTile->getRollDirection().y << "," << currentTile->getRollDirection().z << endl << endl;

                break;
            }
        }
    } 

	// Modify direction based on gravity
	if(currentTile->getShapes().at(0)->normals()[0] != glm::vec3(0.0,1.0,0.0)){
		newDirection.x += (currentTile->getSlope()/GRAVITY_DIVISOR) * currentTile->getRollDirection().x;
		newDirection.y += (currentTile->getSlope()/GRAVITY_DIVISOR) * currentTile->getRollDirection().y;
		newDirection.z += (currentTile->getSlope()/GRAVITY_DIVISOR) * currentTile->getRollDirection().z;
	}

    return newDirection;

};
