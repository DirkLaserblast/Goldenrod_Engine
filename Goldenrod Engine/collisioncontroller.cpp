#include "collisioncontroller.h"

bool detectCollisions(Tile* currentTile, Physics * physics, SoundEngine* sound)
{
	vector<Shape*> borderShapes = currentTile->getBorders()->getInwardShapes();
	vec3 ballPosition = physics->getPosition();

	for (int i = 0; i < borderShapes.size(); i++)
	{

		Shape *currentShape = borderShapes[i];
		float distance = currentShape->distanceToPlane(ballPosition);
		vec3 borderNormal = currentShape->normals()[0];
		vec3 incoming = normalize(physics->getVelocity());
		float planeDelta = currentShape->distanceToPlane(vec3(0.0, borderNormal.y, 0.0)); //Distance from origin to plane along normal

		//Get distance to border plane along ball direction vector
		float t = -(dot(borderNormal, physics->getPosition()) + planeDelta) / dot(borderNormal, physics->getDirection());
		float distanceToPlane = t * length(physics->getDirection());

		//Make sure wall is in front of the ball
		if (dot(borderNormal, incoming) < 0)
		{
			vec3 predPos = physics->getNextPosition();
			float predictedDistance = sqrt(((ballPosition.x - predPos.x)*(ballPosition.x - predPos.x)) + ((ballPosition.y - predPos.y)*(ballPosition.y - predPos.y)) + ((ballPosition.z - predPos.z)*(ballPosition.z - predPos.z)));
			if (distance <= predictedDistance) //Collision detected
			{
				if (DEBUG_WALL_PAINT)
				{
					borderShapes[i]->changeColor(vec4(1.0));
					borderShapes[i]->reload();
				}

				//Play bounce SFX
				bounceSFX(physics->getSpeed(), sound);

				vec3 newDirection = normalize(2.0f * (borderNormal * -incoming) * borderNormal + incoming);

				//cout << "Bounce: " << dot(newDirection, borderNormal) << "\n";
				if (dot(newDirection, borderNormal) > 0) physics->setDirection(newDirection);
				else physics->setDirection(borderNormal * incoming);

				// Flip y if hit border at y max or min of tile (prevents floating/sinking)
				if(currentTile->getShapes()[0]->getMinY() > predPos.y || currentTile->getShapes()[0]->getMaxY() < predPos.y){
					physics->flipYDirection();
				}

                // Reduce speed
                physics->setSpeed(physics->getSpeed()*COLLISION_DAMP_FACTOR);

				return true;
			}

		}

	}

		//End wall collision checking
	return false;
}