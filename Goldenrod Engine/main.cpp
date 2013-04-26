/*main.cpp
*/

/*	Graphics code based on CMPS 160 work
 *  OpenGL base program by Nathaniel Cesario
 *	Lab3 example by Kevin Meggs
 *  Further modified by Casey Scheide
 */

#include "shape.h"
#include "fileiocontroller.h"
#include "gameiocontroller.h"
#include "gamecontroller.h"
#include "shader.h"

#include "GL/glui.h";

Shader *shader = NULL;

int mainWindow; //Used for GLUI
int WIN_WIDTH = 1280, WIN_HEIGHT = 720; //window width/height
mat4 modelView, projection, camera, cameraTemp; //matrices for shaders
vec3 lightPos(0,1,0), viewPos(4,1,4); //Initial position of light source and camera
mat4 modelTrans, mTrans, crTrans, csTrans, ctTrans; //Transformation matrices.
float cameraRotate[2] = {0, 1.0}; //Set by GLUI controls (horizontal, vertical)
float cameraPan[2] = {0, 0};
float cameraZoom[1] = {6.0};

float modelRotate[2];
float modelTranslateXZ[2];
float modelTranslateY[1];

float animTime = 0.0f, deltaT = 0.0001f; //variables for animation

vector<Shape> shapes; //Stores all the currently rendered shapes

vector<float> verts; //vertex array
vector<float> norms; //normals array
vector<float> color; //Model color array


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

float a = 0.0;


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
	float yRotation = cameraRotate[0];
	float height = cameraRotate[1];
	float zoom = cameraZoom[0];
	viewPos = vec3(zoom * cos(yRotation) * sin(height), zoom * cos(height), (zoom * sin(yRotation) * sin(height)));
	camera = lookAt(viewPos, vec3(0, 0, 0), vec3(0,1,0));
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
    glUniform1f(shader->timeLoc, animTime);
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

    //update animation variables.
    //have time oscillate between 0.0 and 1.0.
    if ((animTime >= 1.0 && deltaT > 0.0) ||
            (animTime <= 0.0 && deltaT < 0.0))
	{
        deltaT = -deltaT;
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

//do some GLUT initialization
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

	GLUI_Master.set_glutKeyboardFunc(keyboard);
	GLUI_Master.set_glutReshapeFunc(reshape);

	//GLUI stuff
	GLUI *gluiWindow = GLUI_Master.create_glui("Camera");
	gluiWindow->add_translation("Rotate Camera", GLUI_TRANSLATION_XY, cameraRotate)->set_speed(0.01f);
	gluiWindow->add_translation("Zoom Camera", GLUI_TRANSLATION_Z, cameraZoom)->set_speed(0.1f);
	gluiWindow->add_separator();
	gluiWindow->add_button( "Quit", 0,(GLUI_Update_CB)exit );

	//GLUI_Master.auto_set_viewport();

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
    shader = new Shader("shaders/vertex.vert", "shaders/fragment.frag");

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
	// Initialize game engine
	FileIOController* fileIO = new FileIOController();
	GameIOController* gameIO = new GameIOController();
	GameController* game = new GameController();

    // Check if input file was given, if not use default
    if(argc > 1){
        fileIO->createLevelFromFile(game, argv[1]); // Create level from input file -- NOT WORKING!!!
    }
    else{
        cout << "No input file was provided." << endl;
        fileIO->createLevelFromFile(game, "hole.02.db"); // default level
    }

	initializeGraphics(argc, argv, "MiniGolf", 1280, 720);

    // Add shapes to game level
    shapes.clear();
    shapes.insert(shapes.begin(), game->getCurrentLevelShapes()->begin(), game->getCurrentLevelShapes()->end());
    reloadAllShapes(&verts, &color, &norms, &shapes);

    glutMainLoop();

	// Clean-up
    if(shader) delete shader;
	if(fileIO) delete fileIO;
	if(gameIO) delete gameIO;
	if(game) delete game;

	return 0;
}

