/* main.cpp
*/

/*	Graphics code based on CMPS 160 work
 *  OpenGL base program by Nathaniel Cesario
 *	Lab3 example by Kevin Meggs
 *  Further modified by Casey Scheide
 */

// Shader
#include "shader.h"

// Entity
#include "entity.h"

// System
#include "system.h"
#include "system_levelcontroller.h"
#include "system_physics.h"

// Component
#include "component.h"
#include "component_VBO.h"
#include "component_shapes.h"

// IO
#include "fileiocontroller.h"

// Arrow for aiming
#include "arrow.h"

// Timer
#include "time.h"

// Glui
#include "GL\glui.h"

#define PI 3.14159L
#define DRAW_BALL_VELOCITY true
#define BALL_VELOCITY_COLOR glm::vec4(255,62,150,1.0)

// Initialize systems and other controllers -- These have to be global for the time being...
LevelController* levelController = new LevelController();

FileIOController* fileIO = new FileIOController();

Timer* gameTime = new Timer();
int tickSpeed = 30; //Speed of timer function in milliseconds

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
float height;
float zoom;

float modelRotate[2];
float modelTranslateXZ[2];
float modelTranslateY[1];

vector<Shape> shapes; //Stores all the currently rendered shapes

vector<float> verts; //vertex array
vector<float> norms; //normals array
vector<float> color; //vertex color array

int upMouseXPos;
int downMouseXPos;
int upMouseYPos;
int downMouseYPos;
int mousePosDiff;
int mouseYPosDiff;
bool d_down = false;
bool r_down = false;
bool s_down = false;
bool x_down = false;
bool y_down = false;
bool z_down = false;
bool leftbDown = false;
bool rightbDown = false;

//GLUI variables
GLUI *gluiWindow;
GLUI_Translation *camRotateTrans;
GLUI_Translation *camZoomTrans;
GLUI_Spinner *angleSpinner;
GLUI_Spinner *powerSpinner;
GLUI_Button *fireButton;
GLUI_StaticText *userName;
GLUI_StaticText *currentHole;
GLUI_StaticText *totalNumHoles;
GLUI_StaticText *numStrokes;
GLUI_StaticText *par;
GLUI_StaticText *highScores[5];
string name;

//Variables for gameplay controls
int launchAngle = 0; //Angle to hit the ball, in degrees from 0 to 359
vec3 launchVector = vec3(0.0, 0.0, 1.0); //Vector representing same angle
float launchAngleRadians = 0;
int launchPower; //How hard to "hit" the ball, between 1 and 7 (for now)
bool ballMoving = false;

//Start the ball moving using direction and power from GLUI input
void launchBall(int i)
{
    ballMoving = true;

    launchAngleRadians = (float) launchAngle * (PI/180);
    launchVector = normalize(vec3(sin(launchAngleRadians), 0.0, cos(launchAngleRadians)));

	float prevY = levelController->getCurrentLevel()->ballDirection.y;
	levelController->getCurrentLevel()->ballDirection = glm::vec3(launchVector.x, prevY, launchVector.z);

	// If new tile is flat make sure no y-component
    if(levelController->getCurrentLevel()->getTile(levelController->getCurrentLevel()->ballCurrentTileID)->publicShapes->getShapes().at(0)->normals()[0] == glm::vec3(0.0,1.0,0.0)){
        levelController->getCurrentLevel()->ballDirection = glm::vec3(levelController->getCurrentLevel()->ballDirection.x, 0.0, levelController->getCurrentLevel()->ballDirection.z);
    }
    // If new tile is not flat add y-component
    else{
        glm::vec3 oldDirection = levelController->getCurrentLevel()->ballDirection;
        glm::vec3 upVector = glm::vec3(0.0,1.0,0.0);
        // Get current tile normal
        glm::vec3 tileNormal = levelController->getCurrentLevel()->getTile(levelController->getCurrentLevel()->ballCurrentTileID)->publicShapes->getShapes()[0]->normals()[0];
        glm::vec3 xVector = glm::cross(oldDirection, upVector);
        glm::vec3 newDirection = glm::normalize(glm::cross(tileNormal, xVector));
        levelController->getCurrentLevel()->ballDirection = newDirection;
    }

    levelController->getCurrentLevel()->ballSpeed = (launchPower/100.0f);

	angleSpinner->disable();
	powerSpinner->disable();
	fireButton->disable();
}

// Ball stopped moving
void ballStopped(){

    ballMoving = false;    

    angleSpinner->enable();
	powerSpinner->enable();
	fireButton->enable();

};

//Updates the camera position
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
        levelController->getCurrentLevel()->ballOldYRotation = yRotation;
		yRotation = atan2(-ballDirection.z, -ballDirection.x);

		if(levelController->getCurrentLevel()->ballOldYRotation - yRotation < -0.1 && smoothMotion)
		{
			yRotation = levelController->getCurrentLevel()->ballOldYRotation + 0.1;
		}
		else if (levelController->getCurrentLevel()->ballOldYRotation - yRotation > 0.1 && smoothMotion)
		{
			yRotation = levelController->getCurrentLevel()->ballOldYRotation - 0.1;
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

//Run by GLUT every [tickspeed] miliseconds
void tick(int in)
{
	//Collision checking
	Entity* currentTile = levelController->getCurrentLevel()->getTile(levelController->getCurrentLevel()->ballCurrentTileID);
	Tile* tmpTile = static_cast<Tile*>(currentTile->getComponent(0));
	vector<int> borderIDs = tmpTile->getNeighborIDs();

	Tile* tmpTileComp = static_cast<Tile*> (currentTile->components[0]);
	Entity* tmpBorderEntity = tmpTileComp->borders;
	Shapes* tmpBorderShapesComp = tmpBorderEntity->publicShapes;
	vec3 ballPosition = levelController->getCurrentLevel()->ballPosition;

	vector<Shape*> borderShapes = tmpBorderShapesComp->getShapes();

	//Check distance to all adjacent borders
	for (int i = 0; i < borderShapes.size(); i++)
	{
		Shape *currentShape = borderShapes[i];
		float distance = currentShape->distanceToPlane(ballPosition);
		//cout << "Distance: " << distance << "\n";
		if (distance <= 0.06) //Collision detected, deflect
		{
			//cout << "Collision!\n";
			vec3 borderNormal = currentShape->normals()[0];
			vec3 incoming = levelController->getCurrentLevel()->ballDirection;
			levelController->getCurrentLevel()->ballDirection = normalize(2.0f * (borderNormal * -incoming) * borderNormal + incoming);
			break;
		}
	}

	//End Collision Checking

    // Collision with cup
    glm::vec3 ballPos = levelController->getCurrentLevel()->ballPosition;
    glm::vec3 cupPos = levelController->getCurrentLevel()->cupPosition;
    float cupDist = sqrt(((ballPos.x - cupPos.x)*(ballPos.x - cupPos.x)) + ((ballPos.y - cupPos.y)*(ballPos.y - cupPos.y)) + ((ballPos.z - cupPos.z)*(ballPos.z - cupPos.z)));
    //cout << endl << cupDist << endl; // debug
    if(cupDist < (BALL_RADIUS)){
        // Win condition -- ball collides with tee
        // Translate ball and arrow to origin
        shapes[shapes.size() - 1].translate(-ballPos);
        arrow->translate(-ballPos);

        // Set ball position to tee position
        levelController->getCurrentLevel()->ballPosition = levelController->getCurrentLevel()->teePosition;

        // Translate ball and arrow to tee position (ballPosition)
        shapes[shapes.size() - 1].translate(levelController->getCurrentLevel()->ballPosition);
        arrow->translate(levelController->getCurrentLevel()->ballPosition);

        // Reload ball shape
        shapes[shapes.size() - 1].reload();

        // Reset ball physics
        //levelController->getCurrentLevel()->ballDirection = glm::vec3();
        levelController->getCurrentLevel()->ballSpeed = 0;          
            
        // Set ballMoving flag to false
        ballStopped();
    }

    // Physics calculations
    glm::vec3 deltaPos = glm::vec3(); // modify if moving
    if(ballMoving){
        // Calculate current delta pos
        glm::vec3 ballDirection = levelController->getCurrentLevel()->ballDirection;
        double ballSpeed = levelController->getCurrentLevel()->ballSpeed;
        //deltaPos.x = (ballDirection.x * ballSpeed);
        //deltaPos.y = (ballDirection.y * ballSpeed);
        //deltaPos.z = (ballDirection.z * ballSpeed);

        // Update ball's current tile and direction if moved to/from slanted tile
        // Get top of assumed current tile
        Shape* tileTop = levelController->getCurrentLevel()->getTile(levelController->getCurrentLevel()->ballCurrentTileID)->publicShapes->getShapes().at(0);
        // Calculate if point will be within its current tile after it is moved
        bool inTile = tileTop->checkIfInside(levelController->getCurrentLevel()->ballPosition + deltaPos);
        // If not in tile
        if(inTile == false){           
            //Check all tiles to find which one we are in (tried to check only neighbors above, but had issues)          
            for(int i =0; i < levelController->getCurrentLevel()->getTiles().size(); i++){
                // Check tile
                inTile = levelController->getCurrentLevel()->getTiles()[i]->publicShapes->getShapes().at(0)->checkIfInside(levelController->getCurrentLevel()->ballPosition + deltaPos);
                if(inTile){
                    levelController->getCurrentLevel()->ballCurrentTileID = levelController->getCurrentLevel()->getTiles()[i]->publicCollision->getCurrentTileID();

                    // Update direction
                    // If new tile is flat make sure no y-component
                    if(levelController->getCurrentLevel()->getTile(levelController->getCurrentLevel()->ballCurrentTileID)->publicShapes->getShapes().at(0)->normals()[0] == glm::vec3(0.0,1.0,0.0)){
                        levelController->getCurrentLevel()->ballDirection = glm::vec3(levelController->getCurrentLevel()->ballDirection.x, 0.0, levelController->getCurrentLevel()->ballDirection.z);
                    }
                    // If new tile is not flat add y-component
                    else{
                        glm::vec3 oldDirection = levelController->getCurrentLevel()->ballDirection;
                        glm::vec3 upVector = glm::vec3(0.0,1.0,0.0);
                        // Get current tile normal
                        glm::vec3 tileNormal = levelController->getCurrentLevel()->getTile(levelController->getCurrentLevel()->ballCurrentTileID)->publicShapes->getShapes()[0]->normals()[0];
                        glm::vec3 xVector = glm::cross(oldDirection, upVector);
                        glm::vec3 newDirection = glm::normalize(glm::cross(tileNormal, xVector));
                        levelController->getCurrentLevel()->ballDirection = newDirection;
                    }

                    break;
                }
            }			
        } 
		//// Modify direction based on gravity and update direction again
		//if(levelController->getCurrentLevel()->getTile(levelController->getCurrentLevel()->ballCurrentTileID)->publicShapes->getShapes().at(0)->normals()[0] != glm::vec3(0.0,1.0,0.0)){
		//	// Calculate R direction
		//	glm::vec3 oldDirection = levelController->getCurrentLevel()->ballDirection;
  //          glm::vec3 upVector = glm::vec3(0.0,1.0,0.0);
  //          // Get current tile normal
  //          glm::vec3 tileNormal = levelController->getCurrentLevel()->getTile(levelController->getCurrentLevel()->ballCurrentTileID)->publicShapes->getShapes()[0]->normals()[0];
  //          glm::vec3 xVector = glm::cross(upVector, tileNormal);
  //          glm::vec3 rDirection = glm::normalize(glm::cross(xVector, tileNormal));

		//	levelController->getCurrentLevel()->ballDirection = glm::vec3(oldDirection + rDirection);

		//	// Update direction
		//	oldDirection = levelController->getCurrentLevel()->ballDirection;
		//	xVector = glm::cross(oldDirection, upVector);
		//	glm::vec3 newDirection = glm::normalize(glm::cross(tileNormal, xVector));

		//	levelController->getCurrentLevel()->ballDirection = newDirection;
		//}

		// Calculate deltaPos
		ballDirection = levelController->getCurrentLevel()->ballDirection;
		deltaPos.x = (ballDirection.x * ballSpeed);
		deltaPos.y = (ballDirection.y * ballSpeed);
		deltaPos.z = (ballDirection.z * ballSpeed);

    }

	// Update ball speed -- outside of physics calc so that modified direction takes effect
    levelController->getCurrentLevel()->ballSpeed -= TILE_FRICTION*(levelController->getCurrentLevel()->ballSpeed*100);
    if(levelController->getCurrentLevel()->ballSpeed <= 0.005){
        levelController->getCurrentLevel()->ballSpeed = 0;
    }
	//cout << endl << "ball speed:" << levelController->getCurrentLevel()->ballSpeed << endl; // debug
	//cout << endl << "ball tile ID: " << levelController->getCurrentLevel()->ballCurrentTileID << endl; // debug

    // Update ballPosition
    levelController->getCurrentLevel()->ballPosition += deltaPos;

    // Update drawing
    // Update shapes for drawing
    shapes[shapes.size() - 1].translate(deltaPos);
    shapes[shapes.size() - 1].reload();

    // Update arrow pos for drawing
    arrow->translate(deltaPos);

    // Check if ball stopped
    if(levelController->getCurrentLevel()->ballSpeed == 0){
        ballStopped();
    }

	//If controls are enabled (ball not yet launched), then make ball direction equals launchVector
	if (angleSpinner->enabled)
	{
		float launchAngleRadians = launchAngle * (PI/180);
		launchVector = normalize(vec3(sin(launchAngleRadians), 0.0, cos(launchAngleRadians)));
		updateCamera(levelController->getCurrentLevel()->ballPosition, launchVector, false);
	}
	else
	{
		//Replace launchVector here with actual ball direction
		updateCamera(levelController->getCurrentLevel()->ballPosition, levelController->getCurrentLevel()->ballDirection, true);
	}

	//Update HUD
	userName->set_text(string("Player: " + name).c_str());
	currentHole;
	totalNumHoles;
	numStrokes;
	par;

	glutTimerFunc(tickSpeed, tick, 0);
}

//reshape function for GLUT
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

//display function for GLUT
void display()
{

    glViewport(0,0,WIN_WIDTH,WIN_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Resets all transformation matrices.
    if (r_down == true)
    {
    	modelView = glm::mat4();
    	mTrans = glm::mat4();
        r_down = false;
    }

    //camera = crTrans * csTrans * ctTrans * camera;
	
    mat4 modelCam = camera * modelView;

    //grab the normal matrix from the modelview matrix (upper 3x3 entries of
    //modelview).
    mat3 normalMatrix(modelCam);
    normalMatrix = inverse(normalMatrix);
    normalMatrix = transpose(normalMatrix);

    //Tell OpenGL which shader program we want to use. In this case, we are only
    //using one, but in general we might have many shader programs.
    glUseProgram(shader->program);

    //Pass the matrices and animation time to GPU
    glUniformMatrix4fv(
            shader->modelViewLoc, //handle to variable in the shader program
            1, //how many matrices we want to send
            GL_FALSE, //transpose the matrix
            value_ptr(modelCam) //a pointer to an array containing the entries for
              //the matrix
            );
    glUniformMatrix4fv(shader->projectionLoc, 1, GL_FALSE,
            value_ptr(projection));
    glUniformMatrix3fv(shader->normalMatrixLoc, 1, GL_FALSE,
            value_ptr(normalMatrix));
    glUniform3fv(shader->lightPosLoc, 1, value_ptr(lightPos));
    glUniform3fv(shader->viewPosLoc, 1, value_ptr(viewPos));

    glBindBuffer(GL_ARRAY_BUFFER, shader->vertexBuffer); //which buffer we want
      //to use
    glBufferData(
                GL_ARRAY_BUFFER, //what kind of buffer (an array)
                verts.size() * sizeof(float), //size of the buffer in bytes
                verts.data(), //pointer to data we want to fill the buffer with
                GL_DYNAMIC_DRAW //how we intend to use the buffer
                );
    glEnableVertexAttribArray(shader->vertexLoc); //enable the attribute
    glVertexAttribPointer(
            shader->vertexLoc, //handle to variable in shader program
            3, //vector size (e.g. for texture coordinates this could be 2).
            GL_FLOAT, //what type of data is (e.g. GL_FLOAT, GL_INT, etc.)
            GL_FALSE, //normalize the data?
            0, //stride of data (e.g. offset in bytes). Most of the time leaving
              //this at 0 (assumes data is in one, contiguous array) is fine
              //unless we're doing something really complex.
            NULL //since our stride will be 0 in general, leaving this NULL is
              //also fine in general
            );

    glBindBuffer(GL_ARRAY_BUFFER, shader->normalBuffer); //which buffer we want
      //to use
    glBufferData(
                GL_ARRAY_BUFFER, //what kind of buffer (an array)
                norms.size() * sizeof(float), //size of the buffer in bytes
                norms.data(), //pointer to data we want to fill the buffer with
                GL_DYNAMIC_DRAW //how we intend to use the buffer
                );
    glEnableVertexAttribArray(shader->normalLoc); //enable the attribute
    glVertexAttribPointer(
            shader->normalLoc, //handle to variable in shader program
            3, //vector size (e.g. for texture coordinates this could be 2).
            GL_FLOAT, //what type of data is (e.g. GL_FLOAT, GL_INT, etc.)
            GL_FALSE, //normalize the data?
            0, //stride of data (e.g. offset in bytes). Most of the time leaving
              //this at 0 (assumes data is in one, contiguous array) is fine
              //unless we're doing something really complex.
            NULL //since our stride will be 0 in general, leaving this NULL is
              //also fine in general
            );

    glBindBuffer(GL_ARRAY_BUFFER, shader->colorBuffer);
        glBufferData(
                GL_ARRAY_BUFFER,
                color.size() * sizeof(float),
                color.data(),
                GL_DYNAMIC_DRAW
                );
    glEnableVertexAttribArray(shader->colorLoc);
    glVertexAttribPointer(shader->colorLoc, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    //draw the vertices/normals we just specified.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader->indexBuffer);

	for (int i = 0; i < (int) shapes.size(); i++)
	{
		glDrawArrays(GL_TRIANGLE_FAN, shapes[i].startIndex / 3, shapes[i].numVertices());
	}

    //----------------------------DRAW ARROW----------------------------//
    if(ballMoving == false){
        // Update arrow rotation        
        arrow->rotate(launchAngle);

        modelCam = camera * arrow->getModelTransformMatrix();

        //Pass the matrix
        glUniformMatrix4fv(
                shader->modelViewLoc, //handle to variable in the shader program
                1, //how many matrices we want to send
                GL_FALSE, //transpose the matrix
                value_ptr(modelCam) //a pointer to an array containing the entries for
                  //the matrix
                );

        glBindBuffer(GL_ARRAY_BUFFER, shader->vertexBuffer); //which buffer we want
          //to use
        glBufferData(
                    GL_ARRAY_BUFFER, //what kind of buffer (an array)
                    arrow->getVerts().size() * sizeof(glm::vec3), //size of the buffer in bytes
                    arrow->getVerts().data(), //pointer to data we want to fill the buffer with
                    GL_DYNAMIC_DRAW //how we intend to use the buffer
                    );
        glEnableVertexAttribArray(shader->vertexLoc); //enable the attribute
        glVertexAttribPointer(
                shader->vertexLoc, //handle to variable in shader program
                3, //vector size (e.g. for texture coordinates this could be 2).
                GL_FLOAT, //what type of data is (e.g. GL_FLOAT, GL_INT, etc.)
                GL_FALSE, //normalize the data?
                0, //stride of data (e.g. offset in bytes). Most of the time leaving
                  //this at 0 (assumes data is in one, contiguous array) is fine
                  //unless we're doing something really complex.
                NULL //since our stride will be 0 in general, leaving this NULL is
                  //also fine in general
                );

        glBindBuffer(GL_ARRAY_BUFFER, shader->colorBuffer);
            glBufferData(
                    GL_ARRAY_BUFFER,
                    arrow->getVertColors().size() * sizeof(glm::vec4),
                    arrow->getVertColors().data(),
                    GL_DYNAMIC_DRAW
                    );
        glEnableVertexAttribArray(shader->colorLoc);
        glVertexAttribPointer(shader->colorLoc, 4, GL_FLOAT, GL_FALSE, 0, NULL);

        glDrawArrays(GL_LINES, 0, 6);

    }

    glutSwapBuffers();

}

//idle function for GLUT
void idle()
{
	if(glutGetWindow() != mainWindow){
		glutSetWindow(mainWindow);
	}

    glutPostRedisplay();
}

//captures keyboard input for GLUT
//Sets variables for doing transforms
void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 'r')
		r_down = true;
	if (key == 's')
		s_down = true;
    if (key == 'x')
    	x_down = true;
    if (key == 'y')
    	y_down = true;
    if (key == 'z')
    	z_down = true;
}

//Callback for mouse button events.
//Sets variables for position and button state.
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


//Callback for mouse movement while a button is down.
//Also applies transformations, as doing so in Display was causing the
//    transforms to slide, rather than just move.
void mouseMove(int x, int y)
{
    downMouseXPos = x;
    downMouseYPos = y;
    mousePosDiff = downMouseXPos - upMouseXPos;
    mouseYPosDiff = downMouseYPos - upMouseYPos;
    upMouseXPos = downMouseXPos;
    upMouseYPos = downMouseYPos;

    modelView = glm::mat4();


    if (x_down == true && leftbDown == true)
        mTrans = glm::translate(mTrans, glm::vec3((static_cast<float>(mousePosDiff) / -10),0,0));
    else if (y_down == true && leftbDown == true)
        mTrans = glm::translate(mTrans, glm::vec3(0,(static_cast<float>(mousePosDiff) / 10),0));
    else if (z_down == true && leftbDown == true)
        mTrans = glm::translate(mTrans, glm::vec3(0,0,(static_cast<float>(mousePosDiff) / -10)));
    else if (s_down == true && leftbDown == true)
        mTrans = glm::scale(mTrans, glm::vec3(pow(1.1f, (static_cast<float>(mousePosDiff))),pow(1.1f, (static_cast<float>(mousePosDiff))),pow(1.1f, (static_cast<float>(mousePosDiff)))));
    else if (x_down == true && rightbDown == true)
        mTrans = glm::rotate(mTrans, static_cast<float>(mousePosDiff), glm::vec3(1,0,0));
    else if (y_down == true && rightbDown == true)
        mTrans = glm::rotate(mTrans, static_cast<float>(mousePosDiff), glm::vec3(0,1,0));
    else if (z_down == true && rightbDown == true)
        mTrans = glm::rotate(mTrans, static_cast<float>(mousePosDiff), glm::vec3(0,0,1));


    modelView = modelView * mTrans;
    s_down = false;
    x_down = false;
    y_down = false;
    z_down = false;
    glutPostRedisplay();
}

//do some GLUT initialization, also set up GLUI
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
	powerSpinner->set_int_limits(1, 15, GLUI_LIMIT_CLAMP);
	powerSpinner->set_int_val(launchPower);
	powerSpinner->set_speed(0.1);

	fireButton = gluiWindowLeft->add_button("Go!", 0, launchBall);

	GLUI_Panel *holePanel = gluiWindowLeft->add_panel("Status");
	userName = gluiWindowLeft->add_statictext_to_panel(holePanel, "PLayer: ");
	currentHole = gluiWindowLeft->add_statictext_to_panel(holePanel, "Hole: ");
	totalNumHoles = gluiWindowLeft->add_statictext_to_panel(holePanel, "Total Holes: ");
	numStrokes = gluiWindowLeft->add_statictext_to_panel(holePanel, "Current Stroke: ");
	par = gluiWindowLeft->add_statictext_to_panel(holePanel, "Par: ");

	GLUI_Panel *scoresPanel = gluiWindowLeft->add_panel("High Scores");
	highScores[0] = gluiWindowLeft->add_statictext_to_panel(scoresPanel, "No high scores");
	highScores[1] = gluiWindowLeft->add_statictext_to_panel(scoresPanel, "");
	highScores[2] = gluiWindowLeft->add_statictext_to_panel(scoresPanel, "");
	highScores[3] = gluiWindowLeft->add_statictext_to_panel(scoresPanel, "");
	highScores[4] = gluiWindowLeft->add_statictext_to_panel(scoresPanel, "");
	GLUI_Master.auto_set_viewport();

	gluiWindow->set_main_gfx_window(mainWindow);
	GLUI_Master.set_glutIdleFunc(idle);
	GLUI_Master.sync_live_all();
}

//initialize OpenGL background color and vertex/normal arrays
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

//setup the shader program
void setupShaders()
{
    //create the shader program from a vertex and fragment shader
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

    //notice that, since the vertex attribute norm is not used in the shader
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
    // Check if input file was given, if not use default
    if(argc > 1){
        fileIO->processFile(argv[1]);
        // Make sure that file was correctly read and parsed
        if(fileIO->getCurrentFile() != NULL){
            levelController->addLevel(fileIO->getCurrentFile());
        }
    }
    else{
        //cout << "No input file was provided." << endl;
        // Create default level since no file was specified
        fileIO->processFile(DEFAULT_LEVEL); // default level
        levelController->addLevel(fileIO->getCurrentFile());
    }

	//Get name from user
	cout << "Player name: ";
	cin >> name;
	cout << "\n";

    // Move arrows to ball's starting position
    arrow->translate(levelController->getCurrentLevel()->ballPosition);
    arrow->translate(glm::vec3(0.0, BALL_OFFSET, 0.0));

	initializeGraphics(argc, argv, "MiniGolf", 1280, 720);

    // Add shapes to game level
    levelController->updateCurrentLevelShapes();
    shapes.clear();
    shapes.insert(shapes.begin(), levelController->getCurrentLevelShapes()->begin(), levelController->getCurrentLevelShapes()->end());

    reloadAllShapes(&verts, &color, &norms, &shapes);

    glutMainLoop();

	// Clean-up -- NEED TO ADD THE REST OF THIS
    if(shader) delete shader;

	return 0;
}

