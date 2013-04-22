/*graphics.cpp
*/

/*	OpenGL base program by Nathaniel Cesario
 *	Lab3 example by Kevin Meggs
 *  Further modified by Casey Scheide
 */

#include "graphics.h"

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

#define MAX_PTS 40
#define MAXFLOAT ((float)3.40282346638528860e+38) // taken from math.h
#define MINFLOAT (-MAXFLOAT)

Shader *shader = NULL;

//Adds a colored vertex (consisting of three points) to the vertex, color, and normal vectors
void addVertex(vector<float> *vertexVector, vector<float> *colorVector, vector<float> *normalVector, vec3 coords, vec4 rgba)
{
	vertexVector->push_back(coords.x);
	vertexVector->push_back(coords.y);
	vertexVector->push_back(coords.z);
	colorVector->push_back(rgba.x);
	colorVector->push_back(rgba.y);
	colorVector->push_back(rgba.z);
	colorVector->push_back(rgba.w);
	normalVector->push_back(1);
	normalVector->push_back(1);
	normalVector->push_back(1);
}

//Creates a shape made from vertexes, to be drawn with TRIANGLE_FAN
//Assumes vertexes are given in counterclockwise order
class Shape
{
public:
	Shape(vector<float> *vertexVector, vector<float> *colorVector, vector<float> *normalVector, vector<vec3> vertices, vec4 color);
//	~Shape();
	vec4 shapeColor;
	int startIndex, endIndex, numVertices;
private:
	vector<float> * vertexPointer;
	vector<float> * colorPointer;
	vector<float> * normalPointer;
};

Shape::Shape(vector<float> *vertexVector, vector<float> *colorVector, vector<float> *normalVector, vector<vec3> vertices, vec4 color)
{
	numVertices = vertices.size();
	vertexPointer = vertexVector;
	colorPointer = colorVector;
	normalPointer = normalVector;

	shapeColor = color;

	startIndex = vertexVector->size();
//	if (startIndex != 0) startIndex--;

	for (int i = 0; i < (int) vertices.size(); i++)
	{
		addVertex(vertexVector, colorVector, normalVector, vertices[i], color);
	}

	endIndex = vertexVector->size();
	if (endIndex != 0) endIndex--;
}

//Shape::~Shape()
//{
//	printf("dtor %p\n", this);
//
//	if (vertexPointer->size() > 0)
//	{
//		//cout << "Erasing from " << startIndex << " to " << endIndex << ". Size is " << vertexPointer->size() << ".\n";
//		vertexPointer->erase(vertexPointer->begin(), vertexPointer->begin() + endIndex + 1);
//		for (int i = 0; i < (int) vertexPointer->size(); i++)
//		{
//			cout << vertexPointer->at(i) << " ";
//		}
//		cout << "\n";
//	}
//}

//UBTextureGL texture;

int WIN_WIDTH = 1280, WIN_HEIGHT = 720; //window width/height
glm::mat4 modelView, projection, camera; //matrices for shaders
glm::vec3 lightPos(0,0,1), viewPos(10,10,10);
glm::mat4 modelTrans, mTrans, crTrans, csTrans, ctTrans; //Transformation matrices.
float animTime = 0.0f, deltaT = 0.0001f; //variables for animation

vector<Shape> shapes; //Stores all the currently rendered shapes

vector<float> raw;
vector<float> verts; //vertex array
vector<float> norms;
vector<float> aNorms;
vector<float> axes;
vector<float> color; //Model color array
vector<float> aColor; //Axis and Bounding Box color array
vector<unsigned short> indices;
//vector<float> texs; //texture coordinates
size_t numVerts; //number of total vertices
size_t numIndex;
size_t numAxes;

glm::vec3 minpt;
glm::vec3 maxpt;
glm::vec3 centerpt;

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

void addShape(vector<vec3> vertices, vec4 inColor)
{
	shapes.push_back(Shape(&verts, &color, &norms, vertices, inColor));
}

//updates values based on some change in time
void update(float dt)
{
    animTime += dt;

    modelView = glm::rotate(modelView, glm::float_t(0.01), glm::vec3(1,1,0));
}

//reshape function for GLUT
void reshape(int w, int h)
{
    WIN_WIDTH = w;
    WIN_HEIGHT = h;
    projection = glm::perspective(
            glm::float_t(45),
            glm::float_t(WIN_WIDTH) / glm::float_t(WIN_HEIGHT),
            glm::float_t(0.1),
            glm::float_t(1000.0)
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
    	crTrans = glm::mat4();
    	csTrans = glm::mat4();
    	ctTrans = glm::mat4();
        r_down = false;
    }

    //Setup the modelview matrix
    //Mat4x4F modelCam = camera * modelView;
    camera = glm::lookAt(glm::vec3(10,10,10), glm::vec3(0,0,0), glm::vec3(0,0,1));
    camera = crTrans * csTrans * ctTrans * camera;
    glm::mat4 modelCam = camera * modelView;

    //grab the normal matrix from the modelview matrix (upper 3x3 entries of
    //modelview).
    glm::mat3 normalMatrix(modelCam);
    normalMatrix = glm::inverse(normalMatrix);
    normalMatrix = glm::transpose(normalMatrix);

    //Tell OpenGL which shader program we want to use. In this case, we are only
    //using one, but in general we might have many shader programs.
    glUseProgram(shader->program);

    //Pass the matrices and animation time to GPU
    glUniformMatrix4fv(
            shader->modelViewLoc, //handle to variable in the shader program
            1, //how many matrices we want to send
            GL_FALSE, //transpose the matrix
            glm::value_ptr(modelCam) //a pointer to an array containing the entries for
              //the matrix
            );
    glUniformMatrix4fv(shader->projectionLoc, 1, GL_FALSE,
            glm::value_ptr(projection));
    glUniformMatrix3fv(shader->normalMatrixLoc, 1, GL_FALSE,
            glm::value_ptr(normalMatrix));
    glUniform1f(shader->timeLoc, animTime);
    glUniform3fv(shader->lightPosLoc, 1, glm::value_ptr(lightPos));
    glUniform3fv(shader->viewPosLoc, 1, glm::value_ptr(viewPos));

    block = true;
    glUniform1i(shader->lBlockLoc, block);

    glBindBuffer(GL_ARRAY_BUFFER, shader->axesBuffer); //which buffer we want
    //to use
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

    glBindBuffer(GL_ARRAY_BUFFER, shader->aNormalBuffer); //which buffer we want
    //to use
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

//	glBindBuffer(GL_ARRAY_BUFFER, shader->texBuffer);
//    glEnableVertexAttribArray(shader->texCoordLoc);
//    glVertexAttribPointer(shader->texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, shader->aColorBuffer);
    glBufferData(
            GL_ARRAY_BUFFER,
            aColor.size() * sizeof(float),
            aColor.data(),
            GL_DYNAMIC_DRAW
            );
    glEnableVertexAttribArray(shader->colorLoc);
    glVertexAttribPointer(shader->colorLoc, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    //glDrawArrays(GL_LINES, 6, numAxes); //Draw bounding box

    //Have to reinsert the modelView matrix, as an identity matrix,
    //    in order to properly draw axes.
    modelTrans = modelView;
	modelView = glm::mat4();
    modelCam = camera * modelView;
    glUniformMatrix4fv(
            shader->modelViewLoc, //handle to variable in the shader program
            1, //how many matrices we want to send
            GL_FALSE, //transpose the matrix
            glm::value_ptr(modelCam) //a pointer to an array containing the entries for
              //the matrix
            );
    glDrawArrays(GL_LINES, 0, 6);

    modelView = modelTrans;
    modelCam = camera * modelView;
    glUniformMatrix4fv(
            shader->modelViewLoc, //handle to variable in the shader program
            1, //how many matrices we want to send
            GL_FALSE, //transpose the matrix
            glm::value_ptr(modelCam) //a pointer to an array containing the entries for
              //the matrix
            );
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
		glDrawArrays(GL_TRIANGLE_FAN, shapes[i].startIndex / 3, shapes[i].numVertices);
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

    modelView = glm::mat4();

    if (o_down == true)
    {
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
    }
    else if (c_down == true)
    {
        if (x_down == true && leftbDown == true)
            ctTrans = glm::translate(ctTrans, glm::vec3((static_cast<float>(mousePosDiff) / -10),0,0));
        else if (y_down == true && leftbDown == true)
            ctTrans = glm::translate(ctTrans, glm::vec3(0,(static_cast<float>(mousePosDiff) / 10),0));
        else if (z_down == true && leftbDown == true)
            ctTrans = glm::translate(ctTrans, glm::vec3(0,0,(static_cast<float>(mousePosDiff) / -10)));
        else if (s_down == true && leftbDown == true)
            csTrans = glm::scale(csTrans, glm::vec3(pow(1.1f, (static_cast<float>(mouseYPosDiff))),pow(1.1f, (static_cast<float>(mouseYPosDiff))),pow(1.1f, (static_cast<float>(mouseYPosDiff)))));
        else if (x_down == true && rightbDown == true)
            crTrans = glm::rotate(crTrans, static_cast<float>(mousePosDiff), glm::vec3(1,0,0));
        else if (y_down == true && rightbDown == true)
            crTrans = glm::rotate(crTrans, static_cast<float>(mousePosDiff), glm::vec3(0,1,0));
        else if (z_down == true && rightbDown == true)
            crTrans = glm::rotate(crTrans, static_cast<float>(mousePosDiff), glm::vec3(0,0,1));
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

    //initialize vertex and normal arrays

	addVertex(&axes, &aColor, &aNorms, vec3(0,0,0), vec4(1,0,0,1));
	addVertex(&axes, &aColor, &aNorms, vec3(5,0,0), vec4(1,0,0,1));

	//addVertex(&axes, &aColor, &aNorms, vec3(0,0,0), vec4(1,0,0,1));

    axes.push_back(0);
    axes.push_back(0);
    axes.push_back(0);
    aColor.push_back(0);aColor.push_back(1);aColor.push_back(0);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(0);
    axes.push_back(5);
    axes.push_back(0);
    aColor.push_back(0);aColor.push_back(1);aColor.push_back(0);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(0);
    axes.push_back(0);
    axes.push_back(0);;
    aColor.push_back(0);aColor.push_back(0);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(0);
    axes.push_back(0);
    axes.push_back(5);
    aColor.push_back(0);aColor.push_back(0);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    numAxes = axes.size() / 3;



	//vector<vec3> testShapeVerts;

	//testShapeVerts.push_back(vec3(0,0,0));
	//testShapeVerts.push_back(vec3(1,0,0));
	//testShapeVerts.push_back(vec3(1,1,0));
	//testShapeVerts.push_back(vec3(0,1,0));
	//testShapeVerts.push_back(vec3(0,0,0));

	//Shape testShape = Shape(&verts, &color, &norms, testShapeVerts, vec4(1,1,1,1));
	//shapes.push_back(testShape);

	//for (int i = 0; i < (int) verts.size(); i++)
	//{
	//	cout << verts[i] << " ";
	//}
	//cout << "\n";

	//testShapeVerts.clear();

	//testShapeVerts.push_back(vec3(0,0,4));
	//testShapeVerts.push_back(vec3(1,0,4));
	//testShapeVerts.push_back(vec3(1,1,4));
	//testShapeVerts.push_back(vec3(0,1,4));
	//testShapeVerts.push_back(vec3(0,0,4));


	//testShape = Shape(&verts, &color, &norms, testShapeVerts, vec4(1,1,1,1));
	//shapes.push_back(testShape);

	//for (int i = 0; i < (int) verts.size(); i++)
	//{
	//	cout << verts[i] << " ";
	//}
	//cout << "\n";


	numVerts = verts.size() / 3;

    camera = glm::lookAt(glm::vec3(10,10,10), glm::vec3(0,0,0), glm::vec3(0,0,1));

    projection = glm::perspective(
            glm::float_t(45),
            glm::float_t(WIN_WIDTH) / glm::float_t(WIN_HEIGHT),
            glm::float_t(0.1),
            glm::float_t(1000.0)
    );
}

//setup the shader program
void setupShaders()
{
    //create the shader program from a vertex and fragment shader
    shader = new Shader("shaders/vertex.txt", "shaders/fragment.txt");

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
//	shader->texCoordLoc = glGetAttribLocation(shader->program, "tex");

    //Create buffers for the vertex and normal attribute arrays
    GLuint bufs[8];
    glGenBuffers(8, bufs);

    shader->vertexBuffer = bufs[0];
    shader->normalBuffer = bufs[1];
    shader->colorBuffer = bufs[2];
    shader->axesBuffer = bufs[3];
    shader->aNormalBuffer = bufs[4];
    shader->aColorBuffer = bufs[5];
    shader->indexBuffer = bufs[6];
//	shader->texBuffer = bufs[7];

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

    glBindBuffer(GL_ARRAY_BUFFER, shader->axesBuffer);
    glBufferData(
            GL_ARRAY_BUFFER, //what kind of buffer (an array)
            axes.size() * sizeof(float), //size of the buffer in bytes
            axes.data(), //pointer to data we want to fill the buffer with
            GL_DYNAMIC_DRAW //how we intend to use the buffer
            );

    glBindBuffer(GL_ARRAY_BUFFER, shader->aNormalBuffer);
    glBufferData(
            GL_ARRAY_BUFFER, //what kind of buffer (an array)
            aNorms.size() * sizeof(float), //size of the buffer in bytes
            aNorms.data(), //pointer to data we want to fill the buffer with
            GL_STATIC_DRAW //how we intend to use the buffer
            );

    glBindBuffer(GL_ARRAY_BUFFER, shader->aColorBuffer);
    glBufferData(
            GL_ARRAY_BUFFER,
            aColor.size() * sizeof(float),
            aColor.data(),
            GL_DYNAMIC_DRAW
            );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader->indexBuffer);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(GLushort),
            indices.data(),
            GL_STATIC_DRAW
            );

	//glBindBuffer(GL_ARRAY_BUFFER, shader->texBuffer);
 //   glBufferData(
 //           GL_ARRAY_BUFFER,
 //           texs.size() * sizeof(float),
 //           texs.data(),
 //           GL_STATIC_DRAW
 //           );
}

void findNorms()
{
	glm::vec3 nVec1;
	glm::vec3 nVec2;
	glm::vec3 rVec;
	for (int niter = 0; niter < (int) verts.size(); niter+=9)
	{
		nVec1.x = verts[niter] - verts[niter + 3];
		nVec1.y = verts[niter + 1] - verts[niter + 4];
		nVec1.z = verts[niter + 2] - verts[niter + 5];

		nVec2.x = verts[niter + 6] - verts[niter + 3];
		nVec2.y = verts[niter + 7] - verts[niter + 4];
		nVec2.z = verts[niter + 8] - verts[niter + 5];

		rVec = glm::cross(nVec2, nVec1);
		norms.push_back(rVec.x);
		norms.push_back(rVec.y);
		norms.push_back(rVec.z);
		norms.push_back(rVec.x);
		norms.push_back(rVec.y);
		norms.push_back(rVec.z);
		norms.push_back(rVec.x);
		norms.push_back(rVec.y);
		norms.push_back(rVec.z);
	}
}

void initializeGraphics(int argc, char** argv, char* programName, int windowWidth, int windowHeight)
{
	WIN_WIDTH = windowWidth;
	WIN_HEIGHT = windowHeight;

	//A safeguard against Null errors.
	upMouseXPos = WIN_WIDTH / 2;
	downMouseXPos = WIN_WIDTH / 2;

	findNorms();
    glutInit(&argc, argv);
    setupGLUT(programName);
    setupGL();

    glewInit();

    setupShaders();

    glutMainLoop();

	if (shader) delete shader;
}