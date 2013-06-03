/* main.cpp
*/

/*	Graphics code based on CMPS 160 work
 *  OpenGL base program by Nathaniel Cesario
 *	Lab3 example by Kevin Meggs
 *  Further modified by Casey Scheide
 */

//------------------------Includes------------------------//

// Arrow for aiming
#include "arrow.h"

// File IO
#include "fileiocontroller.h"

// Level
//#include "level_refactor.h"
#include "levelcontroller.h"

// Timer
#include "time.h"

// Shader
#include "shader.h"

// Glui
#include "GL\glui.h"

//irrKlang Sound Engine
#include "sound.h"

//------------------------Macros------------------------//

#define PI 3.14159L
#define NUM_HIGH_SCORES 5 // must be five or less because of GLUI

//------------------------Debug Flags---------------------------//

bool DEBUG_TILE_PAINT = true;
bool DEBUG_WALL_PAINT = true;

//------------------------Initialization------------------------//

// Initialize systems and other controllers -- These have to be global for the time being...
FileIOController* fileIO = new FileIOController();
LevelController* levelController = new LevelController();

Timer* gameTime = new Timer();
int tickSpeed = 50; //Speed of timer function in milliseconds -- this really should be in the timer class...
int MAX_SPEED = 100;

Shader *shader = NULL;

Arrow* arrow = new Arrow();

int mainWindow; //Used for GLUI
int WIN_WIDTH = 1280, WIN_HEIGHT = 900; //window width/height
mat4 modelView, projection, camera, cameraTemp; //matrices for shaders
vec3 lightPos(0,1,0), viewPos(4,1,4); //Initial position of light source and camera
mat4 modelTrans, mTrans, crTrans, csTrans, ctTrans; //Transformation matrices.

//Camera variables
float cameraRotate[2] = {0, 1.0}; //Set by GLUI controls (horizontal, vertical)
float cameraPan[2] = {0, 0};
float cameraZoom[1] = {6.0};
int cameraMode = 0; //Camera selection: 0 = free-look, 1 = third-person, 2 = top-down
float yRotation;
float oldYRotation;
float height;
float zoom;

float modelRotate[2];
float modelTranslateXZ[2];
float modelTranslateY[1];

// Vectors for rendering
vector<Shape*> shapes; //Stores all the currently rendered shapes

vector<float> verts; //vertex array
vector<float> norms; //normals array
vector<float> color; //vertex color array

//Keyboard and mouse IO variables
int upMouseXPos;
int downMouseXPos;
int upMouseYPos;
int downMouseYPos;
int mousePosDiff;
int mouseYPosDiff;
bool leftbDown = false;
bool rightbDown = false;

//GLUI variables
GLUI *gluiWindow;
GLUI_Translation *camRotateTrans;
GLUI_Translation *camZoomTrans;
GLUI_Spinner *angleSpinner;
GLUI_Spinner *powerSpinner;
GLUI_Button *fireButton;
GLUI_Button *soundButton;
GLUI_StaticText *userName;
GLUI_StaticText *currentHole;
GLUI_StaticText *totalNumHoles;
GLUI_StaticText *numStrokes;
GLUI_StaticText *par;
GLUI_StaticText *highScoresList[5];
string name;

// Sound
SoundEngine *sound;

//Variables for gameplay controls
int launchAngle = 0; //Angle to hit the ball, in degrees from 0 to 359
vec3 launchVector = vec3(0.0, 0.0, 1.0); //Vector representing same angle
float launchAngleRadians = 0;
int launchPower; //How hard to "hit" the ball, between 1 and 7 (for now)
bool ballMoving = false;

// Score
int currentHoleScore = 0;
int totalScore = 0;
int highScores[NUM_HIGH_SCORES] = {0};

//------------------------Game Functions------------------------//

void updateTotalScore(){

	// Update score vars
	totalScore += currentHoleScore;
	currentHoleScore = 0;

};

void saveAndResetScore(){

	int oldScore, newScore = totalScore;

	// Add score to high scores if needed
	for(int i = 0; i < NUM_HIGH_SCORES; i++){
		oldScore = highScores[i];
		if(oldScore > newScore || oldScore == 0){
			highScores[i] = newScore;
			newScore = oldScore;
		}
	}

	 // Reset scores
	totalScore = 0;
	currentHoleScore = 0;

};

//Start the ball moving using direction and power from GLUI input
void launchBall(int i)
{
    ballMoving = true;

    launchAngleRadians = (float) launchAngle * (PI/180);
    launchVector = normalize(vec3(sin(launchAngleRadians), 0.0, cos(launchAngleRadians)));

	Level *currentLevel = levelController->getCurrentLevel();
	Ball *ball = currentLevel->getBall();
	Physics *physics = ball->getPhysics();

    float prevY = physics->getDirection().y;
    physics->setDirection(glm::vec3(launchVector.x, prevY, launchVector.z));

	// If new tile is flat make sure no y-component
    if(levelController->getCurrentLevel()->getTile(ball->getCurrentTileID())->getShapes().at(0)->normals()[0] == glm::vec3(0.0,1.0,0.0)){
        physics->setDirection(
            glm::vec3(physics->getDirection().x, 0.0, physics->getDirection().z));
    }
    // If new tile is not flat add y-component
    else{
        glm::vec3 oldDirection = physics->getDirection();
        glm::vec3 upVector = glm::vec3(0.0,1.0,0.0);
        // Get current tile normal
        glm::vec3 tileNormal = levelController->getCurrentLevel()->getTile(ball->getCurrentTileID())->getShapes().at(0)->normals()[0];
        glm::vec3 xVector = glm::cross(oldDirection, upVector);
        glm::vec3 newDirection = glm::normalize(glm::cross(tileNormal, xVector));
        physics->setDirection(newDirection);
    }

    physics->setSpeed(launchPower/100.0f);

	// Increment current hole score and update glui
	currentHoleScore++;
	numStrokes->set_int_val(currentHoleScore);

	angleSpinner->disable();
	powerSpinner->disable();
	fireButton->disable();
}

// Ball stopped moving
void ballStopped(){

    // Set ballMoving to false
    ballMoving = false; 

    // Move arrow to ball's position
    arrow->resetTransformationMatrix();
    arrow->translate(levelController->getCurrentLevel()->getBall()->getPhysics()->getPosition());

    // Re-enable GLUI
    angleSpinner->enable();
	powerSpinner->enable();
	fireButton->enable();

};

// Move to the next hole
void nextHole(){

    // Load next hole
    if(fileIO->getNumHoles() - 1 == levelController->getCurrentLevel()->getLevelID()){
        // Load first level if on final level
        levelController->loadLevel(fileIO, 0);
		// Update highscores
		saveAndResetScore();
    }
    else{
        // Load next level
        levelController->loadNextLevel(fileIO);
		// Update total score
		updateTotalScore();
    }

    // Move arrow to ball's starting position
    arrow->translate(levelController->getCurrentLevel()->getBall()->getPhysics()->getPosition());
    arrow->translate(glm::vec3(0.0, BALL_OFFSET, 0.0));

    // Reload shapes
    shapes.clear();
    shapes = levelController->getCurrentLevel()->getLevelShapes();
    reloadAllShapes(&verts, &color, &norms, shapes);

    // Set flags
    ballStopped();

};

// Move to the previous hole
void prevHole(){

    // Load prev hole
    if(levelController->getCurrentLevel()->getLevelID() == 0){
        // Load last level if on first level
        levelController->loadLevel(fileIO, (fileIO->getNumHoles() - 1));
    }
    else{
        // Load prev level
        levelController->loadPrevLevel(fileIO);
    }

    // Move arrow to ball's starting position
    arrow->translate(levelController->getCurrentLevel()->getBall()->getPhysics()->getPosition());
    arrow->translate(glm::vec3(0.0, BALL_OFFSET, 0.0));

    // Reload shapes
    shapes.clear();
    shapes = levelController->getCurrentLevel()->getLevelShapes();
    reloadAllShapes(&verts, &color, &norms, shapes);

    // Set flags
    ballStopped();

};

// Updates the camera position
void updateCamera(vec3 ballPosition, vec3 ballDirection, bool smoothMotion)
{
	if (cameraRotate[1] < 0.01) cameraRotate[1] = 0.01; //Prevent camera from flipping over vertically
	if (cameraRotate[1] > 2) cameraRotate[1] = 2;
	if (cameraZoom[0] < 2) cameraZoom[0] = 2; //Prevent zooming through the ground

	switch(cameraMode)
	{
	case 0: //Free look
		camRotateTrans->enable();
		camZoomTrans->enable();

		yRotation = cameraRotate[0];
		height = cameraRotate[1];
		zoom = cameraZoom[0];

		viewPos = vec3(zoom * cos(yRotation) * sin(height), zoom * cos(height), (zoom * sin(yRotation) * sin(height)));
		camera = lookAt(viewPos, vec3(0, 0, 0), vec3(0,1,0));
		break;
	case 1: //Third person
		camRotateTrans->disable(); //Disable camera controls
		camZoomTrans->disable();

		height = 1;
		zoom = 2;

		//Set rotation based on direction ball is moving
		//If ball has changed direction, start to turn in that direction, if smoothMotion is on
        oldYRotation = yRotation;
		yRotation = atan2(-ballDirection.z, -ballDirection.x);

		if(oldYRotation - yRotation < -0.1 && smoothMotion)
		{
			yRotation = oldYRotation + 0.1;
		}
		else if (oldYRotation - yRotation > 0.1 && smoothMotion)
		{
			yRotation = oldYRotation - 0.1;
		}
		else yRotation = atan2(-ballDirection.z, -ballDirection.x);


		viewPos = vec3(zoom * cos(yRotation) * sin(height), zoom * cos(height), (zoom * sin(yRotation) * sin(height)));
		camera = lookAt(ballPosition + viewPos, ballPosition, vec3(0, 1, 0));
		break;
	case 2: //Top down
		camRotateTrans->disable();
		camZoomTrans->enable();

		yRotation = cameraRotate[0];
		height = cameraRotate[1];
		zoom = cameraZoom[0];

		viewPos = vec3(0, cameraZoom[0], 0);
		camera = lookAt(viewPos, vec3(0, 0, 0), vec3(-1, 0, 0));
		break;
	}
}

void updateHUD(int hole, int totalHoles, int currentStroke, int levelPar, int highScores[NUM_HIGH_SCORES])
{

	currentHole->set_text(("Hole: " + to_string((long double)hole)).c_str());
	totalNumHoles->set_text(("Total Holes: " + to_string((long double)totalHoles)).c_str());
	numStrokes->set_text(("Stroke: " + to_string((long double)currentStroke)).c_str());
	par->set_text(("Par: " + to_string((long double)levelPar)).c_str());
	
}

// Run by GLUT every [tickspeed] miliseconds
void tick(int in)
{
	
	// Local handles to objects
	Level *currentLevel = levelController->getCurrentLevel();
	Ball *ball = currentLevel->getBall();
	Physics *physics = ball->getPhysics();

	Tile* currentTile = currentLevel->getTile(ball->getCurrentTileID());
    vector<int> borderIDs = currentTile->getNeighborIDs();
    vector<Shape*> borderShapes = currentTile->getBorders()->getShapes();
    vec3 ballPosition = physics->getPosition();

	// Debug -- Highlight current tile
	if (DEBUG_TILE_PAINT)
	{
		currentTile->getShapes()[0]->changeColor(TILE_HIGHLIGHT_COLOR);
		currentTile->getShapes()[0]->reload();
	}

    // Collision with cup
    glm::vec3 ballPos = physics->getPosition();
    glm::vec3 cupPos = currentLevel->getCup()->getPhysics()->getPosition();
    float cupPlaneDist = sqrt(((ballPos.x - cupPos.x)*(ballPos.x - cupPos.x)) + ((ballPos.z - cupPos.z)*(ballPos.z - cupPos.z)));
    //cout << endl << cupDist << endl; // debug
    if(cupPlaneDist < (CUP_RADIUS - (0.8 * BALL_RADIUS)) && abs(cupPos.y - ballPos.y) <= BALL_OFFSET){

        //----------------CHANGE TO NEXT HOLE----------------//
        nextHole();
    }

    // Physics calculations   
    if(ballMoving)
	{

		//Check distance to all adjacent borders
		for (int i = 0; i < borderShapes.size(); i++)
		{

			Shape *currentShape = borderShapes[i];
			float distance = currentShape->distanceToPlane(ballPosition);
			vec3 borderNormal = currentShape->normals()[0];
			vec3 incoming = physics->getDirection();

			//Make sure wall is in front of the ball
			if (abs(acos(dot(borderNormal, incoming))) > PI/2)
			{
				//printf("Angle: %f\n", abs(acos(dot(borderNormal, incoming))));
				//cout << "Distance: " << distance << "\n";
				//See if distance you are about to move would move through the wall
				vec3 predPos = physics->getNextPosition();
				float predictedDistance = sqrt(((ballPosition.x - predPos.x)*(ballPosition.x - predPos.x)) + ((ballPosition.y - predPos.y)*(ballPosition.y - predPos.y)) + ((ballPosition.z - predPos.z)*(ballPosition.z - predPos.z)));
				//printf("Predicted distance = %f\n", predictedDistance);
				if (distance <= physics->getSpeed()) //Collision detected, deflect
				{
					if (DEBUG_WALL_PAINT)
					{
						borderShapes[i]->changeColor(vec4(1.0));
						borderShapes[i]->reload();
					}
					//cout << acos(dot(borderNormal, incoming)) << "\n";
					physics->setDirection(normalize(2.0f * (borderNormal * -incoming) * borderNormal + incoming));
				}
				//else cout << "Ignoring wall\n";
			}

		}

		//End wall collision checking

        // Update ball's current tile and direction if moved to/from slanted tile
        // Get top of assumed current tile
        Shape* tileTop = currentLevel->getTile(ball->getCurrentTileID())->getShapes().at(0);
        // Calculate if point will be within its current tile after it is moved
        bool inTile = tileTop->checkIfInside(physics->getNextPosition());
        // If not in tile
        if(inTile == false)
		{
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
                inTile = currentLevel->getTiles()[i]->getShapes().at(0)->checkIfInside(physics->getNextPosition());
                if(inTile){
                    ball->setCurrentTileID(currentLevel->getTiles()[i]->getID());

                    // Update direction
                    // If new tile is flat make sure no y-component
                    if(currentLevel->getTile(ball->getCurrentTileID())->getShapes().at(0)->normals()[0] == glm::vec3(0.0,1.0,0.0)){
                        physics->setDirection(
                            glm::vec3(physics->getDirection().x, 0.0, physics->getDirection().z));
                    }
                    // If new tile is not flat add y-component
                    else{
                        glm::vec3 oldDirection = physics->getDirection();
                        glm::vec3 upVector = glm::vec3(0.0,1.0,0.0);
                        // Get current tile normal
                        glm::vec3 tileNormal = currentLevel->getTile(ball->getCurrentTileID())->getShapes().at(0)->normals()[0];
                        glm::vec3 xVector = glm::cross(oldDirection, upVector);
                        glm::vec3 newDirection = glm::normalize(glm::cross(tileNormal, xVector));
                        physics->setDirection(newDirection);
                    }

                    break;
                }
            }			
        } 
		//// Modify direction based on gravity and update direction again
		//if(currentLevel->getTile(currentLevel->ballCurrentTileID)->publicShapes->getShapes().at(0)->normals()[0] != glm::vec3(0.0,1.0,0.0)){
		//	// Calculate R direction
		//	glm::vec3 oldDirection = currentLevel->ballDirection;
  //          glm::vec3 upVector = glm::vec3(0.0,1.0,0.0);
  //          // Get current tile normal
  //          glm::vec3 tileNormal = currentLevel->getTile(currentLevel->ballCurrentTileID)->publicShapes->getShapes()[0]->normals()[0];
  //          glm::vec3 xVector = glm::cross(upVector, tileNormal);
  //          glm::vec3 rDirection = glm::normalize(glm::cross(xVector, tileNormal));

		//	currentLevel->ballDirection = glm::vec3(oldDirection + rDirection);

		//	// Update direction
		//	oldDirection = currentLevel->ballDirection;
		//	xVector = glm::cross(oldDirection, upVector);
		//	glm::vec3 newDirection = glm::normalize(glm::cross(tileNormal, xVector));

		//	currentLevel->ballDirection = newDirection;
		//}

		// Update ball speed
		double ballSpeed = physics->getSpeed();
		if(ballSpeed == 0.0){
			// Do nothing
		}
		else if(ballSpeed > 0.005){
			physics->setSpeed(ballSpeed - TILE_DEFAULT_FRICTION*(ballSpeed*100));
			ballSpeed = physics->getSpeed();
		}
		else{
			physics->setSpeed(0.0);
		}
		//cout << endl << "ball speed:" << currentLevel->ballSpeed << endl; // debug
		//cout << endl << "ball tile ID: " << currentLevel->ballCurrentTileID << endl; // debug

		// Check if ball stopped
		if(ballSpeed == 0){
			ballStopped();
		}

		// Update ballPosition
		physics->updatePosition();

		// Update shapes for drawing
		Shape* ballShape = ball->getShapes().at(0);
		ballShape->translate(physics->getVelocity());
		ballShape->reload();

    }   
    
	// Update HUD
	currentLevel = levelController->getCurrentLevel();
	updateHUD(currentLevel->getLevelID(), fileIO->getNumHoles(), currentHoleScore, currentLevel->getPar(), highScores);

	//If controls are enabled (ball not yet launched), then make ball direction equal to launchVector
	if (angleSpinner->enabled)
	{
		//float launchAngleRadians = launchAngle * (PI/180);
		//launchVector = normalize(vec3(sin(launchAngleRadians), 0.0, cos(launchAngleRadians)));
        updateCamera(physics->getPosition(), launchVector, false);
	}
	else
	{
		updateCamera(physics->getPosition(), physics->getDirection(), true);
	}

	glutTimerFunc(tickSpeed, tick, 0);
}

// Reshape function for GLUT
void reshape(int w, int h)
{
	GLUI_Master.auto_set_viewport();
    WIN_WIDTH = w;
    WIN_HEIGHT = h;
    projection = perspective(
            float_t(45),
            float_t(WIN_WIDTH) / float_t(WIN_HEIGHT),
            float_t(0.1),
            float_t(1000.0)
    );
}

// Display function for GLUT
void display()
{

    glViewport(0,0,WIN_WIDTH,WIN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // camera = crTrans * csTrans * ctTrans * camera;
	
    mat4 modelCam = camera * modelView;

    // Grab the normal matrix from the modelview matrix (upper 3x3 entries of
    // modelview).
    mat3 normalMatrix(modelCam);
    normalMatrix = inverse(normalMatrix);
    normalMatrix = transpose(normalMatrix);

    // Tell OpenGL which shader program we want to use. In this case, we are only
    // using one, but in general we might have many shader programs.
    glUseProgram(shader->program);

    // Pass the matrices and animation time to GPU
    glUniformMatrix4fv(shader->modelViewLoc, 1, GL_FALSE, value_ptr(modelCam));
    glUniformMatrix4fv(shader->projectionLoc, 1, GL_FALSE, value_ptr(projection));
    glUniformMatrix3fv(shader->normalMatrixLoc, 1, GL_FALSE, value_ptr(normalMatrix));
    glUniform3fv(shader->lightPosLoc, 1, value_ptr(lightPos));
    glUniform3fv(shader->viewPosLoc, 1, value_ptr(viewPos));

    // Buffer vertex data
    glBindBuffer(GL_ARRAY_BUFFER, shader->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
    // Enable vertex array
    glEnableVertexAttribArray(shader->vertexLoc);
    glVertexAttribPointer(shader->vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Buffer normal data
    glBindBuffer(GL_ARRAY_BUFFER, shader->normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(float), norms.data(), GL_DYNAMIC_DRAW);
    // Enable normal array
    glEnableVertexAttribArray(shader->normalLoc);
    glVertexAttribPointer(shader->normalLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Buffer color data
    glBindBuffer(GL_ARRAY_BUFFER, shader->colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), color.data(), GL_DYNAMIC_DRAW);
    // Enable color array
    glEnableVertexAttribArray(shader->colorLoc);
    glVertexAttribPointer(shader->colorLoc, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    //Bind element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader->indexBuffer);

	for (int i = 0; i < (int) shapes.size(); i++)
	{
		glDrawArrays(GL_TRIANGLE_FAN, shapes[i]->startIndex / 3, shapes[i]->numVertices());
	}

    //----------------------------DRAW ARROW----------------------------//
    if(ballMoving == false){
        // Update arrow rotation        
        arrow->rotate(launchAngle);

        modelCam = camera * arrow->getModelTransformMatrix();

        //Pass the matrix
        glUniformMatrix4fv(shader->modelViewLoc, 1, GL_FALSE, value_ptr(modelCam));

        // Buffer vertex data
        glBindBuffer(GL_ARRAY_BUFFER, shader->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, arrow->getVerts().size() * sizeof(glm::vec3), arrow->getVerts().data(), GL_DYNAMIC_DRAW);
        // Enable vertex array
        glEnableVertexAttribArray(shader->vertexLoc);
        glVertexAttribPointer(shader->vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        // Disable normal array
        glDisableVertexAttribArray(shader->normalLoc);

        // Buffer color data
        glBindBuffer(GL_ARRAY_BUFFER, shader->colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, arrow->getVertColors().size() * sizeof(glm::vec4), arrow->getVertColors().data(), GL_DYNAMIC_DRAW);
        // Enable color array
        glEnableVertexAttribArray(shader->colorLoc);
        glVertexAttribPointer(shader->colorLoc, 4, GL_FLOAT, GL_FALSE, 0, NULL);

        glDrawArrays(GL_LINES, 0, 6);

    }

    glutSwapBuffers();

}

// Idle function for GLUT
void idle()
{
	if(glutGetWindow() != mainWindow){
		glutSetWindow(mainWindow);
	}

    glutPostRedisplay();
}

// Captures keyboard input for GLUT
// Sets variables for doing transforms
void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
    if (key == 'm')
        nextHole();
    if (key == 'n')
        prevHole();
}

// Callback for mouse button events.
// Sets variables for position and button state.
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
    	upMouseXPos = x;
    	downMouseXPos = x;
    	upMouseYPos = y;
    	downMouseYPos = y;
    	leftbDown = true;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        leftbDown = false;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
    	upMouseXPos = x;
    	downMouseXPos = x;
    	upMouseYPos = y;
    	downMouseYPos = y;
    	rightbDown = true;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        rightbDown = false;
    }
}


/* Callback for mouse movement while a button is down.
 * Also applies transformations, as doing so in Display was causing the
 * transforms to slide, rather than just move.
 */
void mouseMove(int x, int y)
{
    downMouseXPos = x;
    downMouseYPos = y;
    mousePosDiff = downMouseXPos - upMouseXPos;
    mouseYPosDiff = downMouseYPos - upMouseYPos;
    upMouseXPos = downMouseXPos;
    upMouseYPos = downMouseYPos;

    modelView = glm::mat4();
    modelView = modelView * mTrans;

    glutPostRedisplay();
}

void soundTest (int i)
{
	sound->getEngine()->play2D("41-goldenrod-city.ogg", true);
	soundButton->disable();
}

// Do some GLUT initialization, also set up GLUI
void setupGLUT(char* programName)
{
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	mainWindow = glutCreateWindow(programName);

    glutReshapeFunc(reshape);

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);

    glutMouseFunc(mouse);

    glutMotionFunc(mouseMove);

    glutIdleFunc(idle);

	glutTimerFunc(tickSpeed, tick, 0);

	GLUI_Master.set_glutKeyboardFunc(keyboard);
	GLUI_Master.set_glutReshapeFunc(reshape);

	//GLUI stuff
	gluiWindow = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_RIGHT);
	camRotateTrans = gluiWindow->add_translation("Rotate Camera", GLUI_TRANSLATION_XY, cameraRotate);
	camRotateTrans->set_speed(0.01f);
	camZoomTrans = gluiWindow->add_translation("Zoom Camera", GLUI_TRANSLATION_Z, cameraZoom);
	camZoomTrans->set_speed(0.1f);
	gluiWindow->add_separator();
	gluiWindow->add_button( "Quit", 0,(GLUI_Update_CB)exit );
	
	GLUI_Panel *viewPanel = gluiWindow->add_panel("Camera View", GLUI_PANEL_EMBOSSED);
	GLUI_RadioGroup *cameraRadio = gluiWindow->add_radiogroup_to_panel(viewPanel, &cameraMode);
	gluiWindow->add_radiobutton_to_group(cameraRadio, "Free Look");
	gluiWindow->add_radiobutton_to_group(cameraRadio, "Third Person");
	gluiWindow->add_radiobutton_to_group(cameraRadio, "Top View");

	GLUI *gluiWindowLeft = GLUI_Master.create_glui_subwindow(mainWindow, GLUI_SUBWINDOW_LEFT);

	angleSpinner = gluiWindowLeft->add_spinner("Angle", GLUI_SPINNER_INT, &launchAngle);
	angleSpinner->set_int_limits(0, 359, GLUI_LIMIT_WRAP);
	angleSpinner->set_speed(0.2);
	angleSpinner->set_int_val(launchAngle);

	powerSpinner = gluiWindowLeft->add_spinner("Power", GLUI_SPINNER_INT, &launchPower);
	powerSpinner->set_int_limits(1, MAX_SPEED, GLUI_LIMIT_CLAMP);
	powerSpinner->set_int_val(launchPower);
	powerSpinner->set_speed(0.1);

	fireButton = gluiWindowLeft->add_button("Go!", 0, launchBall);

	GLUI_Panel *holePanel = gluiWindowLeft->add_panel("Status");
	userName = gluiWindowLeft->add_statictext_to_panel(holePanel, "Player: ");
	currentHole = gluiWindowLeft->add_statictext_to_panel(holePanel, "Hole: ");
	totalNumHoles = gluiWindowLeft->add_statictext_to_panel(holePanel, "Total Holes: ");
	numStrokes = gluiWindowLeft->add_statictext_to_panel(holePanel, "Current Stroke: ");	
	par = gluiWindowLeft->add_statictext_to_panel(holePanel, "Par: ");

	// Set glui initial values
	numStrokes->set_int_val(0);

	GLUI_Panel *scoresPanel = gluiWindowLeft->add_panel("High Scores");
	highScoresList[0] = gluiWindowLeft->add_statictext_to_panel(scoresPanel, "No high scores");
	highScoresList[1] = gluiWindowLeft->add_statictext_to_panel(scoresPanel, "");
	highScoresList[2] = gluiWindowLeft->add_statictext_to_panel(scoresPanel, "");
	highScoresList[3] = gluiWindowLeft->add_statictext_to_panel(scoresPanel, "");
	highScoresList[4] = gluiWindowLeft->add_statictext_to_panel(scoresPanel, "");
	
	soundButton = gluiWindowLeft->add_button("Sound Test!", 0, soundTest);
	
	GLUI_Master.auto_set_viewport();

	gluiWindow->set_main_gfx_window(mainWindow);
	GLUI_Master.set_glutIdleFunc(idle);
	GLUI_Master.sync_live_all();
}

// Initialize OpenGL background color and vertex/normal arrays
void setupGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    projection = perspective(
            float_t(45),
            float_t(WIN_WIDTH) / float_t(WIN_HEIGHT),
            float_t(0.1),
            float_t(1000.0)
    );
}

// Setup the shader program
void setupShaders()
{
    //Create the shader program from a vertex and fragment shader
    shader = new Shader("shaders/light.vert", "shaders/light.frag");

    //Here's where we setup handles to each variable that is used in the shader
    //program. See the shader source code for more detail on what the difference
    //is between uniform and vertex attribute variables.
    shader->modelViewLoc = glGetUniformLocation(shader->program, "M");
    shader->projectionLoc = glGetUniformLocation(shader->program, "P");
    shader->normalMatrixLoc = glGetUniformLocation(shader->program, "M_n");
    shader->timeLoc = glGetUniformLocation(shader->program, "time");
    shader->lightPosLoc = glGetUniformLocation(shader->program, "L_p");
    shader->viewPosLoc = glGetUniformLocation(shader->program, "E");
    shader->lBlockLoc = glGetUniformLocation(shader->program, "lBlock");

    //Notice that, since the vertex attribute norm is not used in the shader
    //program, shader->normalLoc = -1. If we access norm in the shader program,
    //then this value will be >= 0.
    shader->vertexLoc = glGetAttribLocation(shader->program, "pos");
    shader->normalLoc = glGetAttribLocation(shader->program, "norm");
    shader->colorLoc = glGetAttribLocation(shader->program, "color");

    //Create buffers for the vertex and normal attribute arrays
    GLuint bufs[4];
    glGenBuffers(4, bufs);

    shader->vertexBuffer = bufs[0];
    shader->normalBuffer = bufs[1];
    shader->colorBuffer = bufs[2];
    shader->indexBuffer = bufs[3];

    //This is where we pass the vertex/normal data to the GPU.
    //In general, the procedure for working with buffers is:
    //  1. Tell OpenGL which buffer we're using (glBindBuffer)
    //  2. Tell OpenGL what to do with the buffer (e.g. fill buffer, use the
    //     in the buffer, etc).
    //
    //Here we are filling the buffers (glBufferData). The last parameter
    //(GL_STATIC_DRAW), says that our intention is to not change the values in
    //these buffers. If we were going to be modifying these positions frequently
    //at runtime, we might want to make this GL_DYNAMIC_DRAW instead. For right
    //now, it's not too important which you choose.

    glBindBuffer(GL_ARRAY_BUFFER, shader->vertexBuffer);
    glBufferData(
            GL_ARRAY_BUFFER, //what kind of buffer (an array)
            verts.size() * sizeof(float), //size of the buffer in bytes
            verts.data(), //pointer to data we want to fill the buffer with
            GL_DYNAMIC_DRAW //how we intend to use the buffer
            );

    glBindBuffer(GL_ARRAY_BUFFER, shader->normalBuffer);
    glBufferData(
            GL_ARRAY_BUFFER,
            norms.size() * sizeof(float),
            norms.data(),
            GL_DYNAMIC_DRAW
            );

    glBindBuffer(GL_ARRAY_BUFFER, shader->colorBuffer);
    glBufferData(
            GL_ARRAY_BUFFER,
            color.size() * sizeof(float),
            color.data(),
            GL_DYNAMIC_DRAW
            );

}

void initializeGraphics(int argc, char** argv, char* programName, int windowWidth, int windowHeight)
{

	WIN_WIDTH = windowWidth;
	WIN_HEIGHT = windowHeight;

    glutInit(&argc, argv);
    setupGLUT(programName);
    setupGL();

    glewInit();

    setupShaders();
    	
}

int main(int argc, char **argv)
{	
	sound = new SoundEngine();

    // Check if input file was given, if not use default
    if(argc > 1){
        fileIO->processFile(argv[1]);
        // Make sure that file was correctly read and parsed
        if(fileIO->getNumHoles() > 0){
            levelController->loadCurrentLevel(fileIO);
        }
    }
    else{
        cout << "No input file was provided." << endl;
        // Create default level since no file was specified
        fileIO->processFile(DEFAULT_COURSE);
        levelController->loadLevel(fileIO, 7);
    }

	//Get name from user
	cout << "Player name: ";
	cin >> name;
	cout << "\n";

    // Move arrow to ball's starting position
    arrow->translate(levelController->getCurrentLevel()->getBall()->getPhysics()->getPosition());
    arrow->translate(glm::vec3(0.0, BALL_OFFSET, 0.0));

    // Add shapes to game level
    shapes.clear();
    shapes = levelController->getCurrentLevel()->getLevelShapes();
    reloadAllShapes(&verts, &color, &norms, shapes);

	initializeGraphics(argc, argv, "MiniGolf", 1280, 720);   

    glutMainLoop();

	// Clean-up -- NEED TO ADD THE REST OF THIS
    if(shader) delete shader;
    if(levelController) delete levelController;
    if(fileIO) delete fileIO;
    if(arrow) delete arrow;
	if(sound) delete sound;

	return 0;
}
