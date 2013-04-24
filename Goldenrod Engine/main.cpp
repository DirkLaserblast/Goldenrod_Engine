

/*main.cpp
*/

/*	Based on CMPS 160 work
 * OpenGL base program by Nathaniel Cesario
 *	Lab3 example by Kevin Meggs
 *  Further modified by Casey Scheide
 */

#include "shape.h"
#include "fileiocontroller.h"
#include "gameiocontroller.h"
#include "gamecontroller.h"


#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/glui.h"
//#include <GL/gl.h>

#define GLM_SWIZZLE
#define PICKINGSHAPE_ID 700
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

#include <cmath>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "shader.h"

using namespace std;
using namespace glm;

Shader *shader = NULL;

int WIN_WIDTH = 1280, WIN_HEIGHT = 720; //window width/height
mat4 modelView, projection, camera; //matrices for shaders
vec3 lightPos(0,0,1), viewPos(10,10,10);
mat4 modelTrans, mTrans, crTrans, csTrans, ctTrans; //Transformation matrices.
float animTime = 0.0f, deltaT = 0.0001f; //variables for animation

vector<Shape> shapes; //Stores all the currently rendered shapes

vector<float> raw;
vector<float> verts; //vertex array
vector<float> norms;

vector<float> color; //Model color array
size_t numVerts; //number of total vertices
size_t numIndex;

vec3 minpt;
vec3 maxpt;
vec3 centerpt;

int numCoords;
int numPolygons;
int upMouseXPos;
int downMouseXPos;
int upMouseYPos;
int downMouseYPos;
int mousePosDiff;
int mouseYPosDiff;
bool block = false;
bool c_down = false;
bool d_down = false;
bool r_down = false;
bool s_down = false;
bool x_down = false;
bool y_down = false;
bool z_down = false;
bool o_down = false;
bool leftbDown = false;
bool rightbDown = false;

//reshape function for GLUT
void reshape(int w, int h)
{
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
    	modelView = mat4();
    	mTrans = mat4();
    	crTrans = mat4();
    	csTrans = mat4();
    	ctTrans = mat4();
        r_down = false;
    }

    //Setup the modelview matrix
    //Mat4x4F modelCam = camera * modelView;
    camera = lookAt(vec3(10,10,10), vec3(0,0,0), vec3(0,0,1));
    camera = crTrans * csTrans * ctTrans * camera;
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


    block = false;
    glUniform1i(shader->lBlockLoc, block);



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
    glutPostRedisplay();
}

//captures keyboard input for GLUT
//Sets variables for doing transforms
void keyboard(unsigned char key, int x, int y)
{

	if (key == 27)
		exit(0);
	if (key == 'c')
	{
		if (c_down == false)
		{
		    c_down = true;
		    o_down = false;
		}
		else
			c_down = false;
	}
	if (key == 'o')
	{
		if (o_down == false)
		{
			c_down = false;
		    o_down = true;
		}
		else
			o_down = false;
	}
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

    modelView = mat4();

    if (o_down == true)
    {
        if (x_down == true && leftbDown == true)
            mTrans = translate(mTrans, vec3((static_cast<float>(mousePosDiff) / -10),0,0));
        else if (y_down == true && leftbDown == true)
            mTrans = translate(mTrans, vec3(0,(static_cast<float>(mousePosDiff) / 10),0));
        else if (z_down == true && leftbDown == true)
            mTrans = translate(mTrans, vec3(0,0,(static_cast<float>(mousePosDiff) / -10)));
        else if (s_down == true && leftbDown == true)
            mTrans = scale(mTrans, vec3(pow(1.1f, (static_cast<float>(mousePosDiff))),pow(1.1f, (static_cast<float>(mousePosDiff))),pow(1.1f, (static_cast<float>(mousePosDiff)))));
        else if (x_down == true && rightbDown == true)
            mTrans = rotate(mTrans, static_cast<float>(mousePosDiff), vec3(1,0,0));
        else if (y_down == true && rightbDown == true)
            mTrans = rotate(mTrans, static_cast<float>(mousePosDiff), vec3(0,1,0));
        else if (z_down == true && rightbDown == true)
            mTrans = rotate(mTrans, static_cast<float>(mousePosDiff), vec3(0,0,1));
    }
    else if (c_down == true)
    {
        if (x_down == true && leftbDown == true)
            ctTrans = translate(ctTrans, vec3((static_cast<float>(mousePosDiff) / -10),0,0));
        else if (y_down == true && leftbDown == true)
            ctTrans = translate(ctTrans, vec3(0,(static_cast<float>(mousePosDiff) / 10),0));
        else if (z_down == true && leftbDown == true)
            ctTrans = translate(ctTrans, vec3(0,0,(static_cast<float>(mousePosDiff) / -10)));
        else if (s_down == true && leftbDown == true)
            csTrans = scale(csTrans, vec3(pow(1.1f, (static_cast<float>(mouseYPosDiff))),pow(1.1f, (static_cast<float>(mouseYPosDiff))),pow(1.1f, (static_cast<float>(mouseYPosDiff)))));
        else if (x_down == true && rightbDown == true)
            crTrans = rotate(crTrans, static_cast<float>(mousePosDiff), vec3(1,0,0));
        else if (y_down == true && rightbDown == true)
            crTrans = rotate(crTrans, static_cast<float>(mousePosDiff), vec3(0,1,0));
        else if (z_down == true && rightbDown == true)
            crTrans = rotate(crTrans, static_cast<float>(mousePosDiff), vec3(0,0,1));
    }

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
	glutCreateWindow(programName);

    glutReshapeFunc(reshape);

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);

    glutMouseFunc(mouse);

    glutMotionFunc(mouseMove);

    glutIdleFunc(idle);
}

//initialize OpenGL background color and vertex/normal arrays
void setupGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

	vector<vec3> testShapeVerts;

	testShapeVerts.push_back(vec3(-4,-4,0));
	testShapeVerts.push_back(vec3(4,-4,0));
	testShapeVerts.push_back(vec3(4,4,0));
	testShapeVerts.push_back(vec3(-4,4,0));

	shapes.push_back(Shape(testShapeVerts, vec4(0,1,1,1)));

	testShapeVerts.clear();

	testShapeVerts.push_back(vec3(-4,-4,-1));
	testShapeVerts.push_back(vec3(4,-4,-1));
	testShapeVerts.push_back(vec3(4,4,-1));
	testShapeVerts.push_back(vec3(-4,4,-1));

	shapes.push_back(Shape(testShapeVerts, vec4(1,0,1,1)));

	reloadAllShapes(&verts, &color, &norms, &shapes);

    camera = lookAt(vec3(10,10,10), vec3(0,0,0), vec3(0,0,1));

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

	//A safeguard against Null errors.
	upMouseXPos = WIN_WIDTH / 2;
	downMouseXPos = WIN_WIDTH / 2;

    glutInit(&argc, argv);
    setupGLUT(programName);
    setupGL();

    glewInit();

    setupShaders();
    
	glutMainLoop();

	if (shader) delete shader;
}


int main(int argc, char **argv)
{
	// Initialize game engine
	FileIOController* fileIO = new FileIOController();
	GameIOController* gameIO = new GameIOController();
	GameController* game = new GameController();

    fileIO->createLevelFromFile(game, "hole.01.db"); // test code

	initializeGraphics(argc, argv, "MiniGolf", 1280, 720);

	// Clean-up
	delete fileIO;
	delete gameIO;
	delete game;

	return 0;
}

