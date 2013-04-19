/*graphics.cpp
*/

/*	OpenGL base program by Nathaniel Cesario
 *	Lab3 example by Kevin Meggs
 *  Further modified by Casey Scheide
 */

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
using namespace std;

#define MAX_PTS 40
#define MAXFLOAT ((float)3.40282346638528860e+38) // taken from math.h
#define MINFLOAT (-MAXFLOAT)

/**
 * Simple class for keeping track of shader program and vertex attribute
 * locations.
 */
class Shader {
public:
    Shader(string vertFile, string fragFile) { fromFiles(vertFile, fragFile); }

    /**
     * Creates a shader program based on vertex and fragment source.
     *
     * @param vertFile Path to vertex source
     * @param fragFile Path to fragment source
     */
    void fromFiles(string vertFile, string fragFile) {
        //These are shader objects containing the shader source code
        GLint vSource = setShaderSource(vertFile, GL_VERTEX_SHADER);
        GLint fSource = setShaderSource(fragFile, GL_FRAGMENT_SHADER);

        //Create a new shader program
        program = glCreateProgram();

        //Compile the source code for each shader and attach it to the program.
        glCompileShader(vSource);
        //printLog("vertex compile log: ", vSource);
        glAttachShader(program, vSource);

        glCompileShader(fSource);
        //printLog("fragment compile log: ", fSource);
        glAttachShader(program, fSource);

        //we could attach more shaders, such as a geometry or tessellation
        //shader here.

        //link all of the attached shader objects
        glLinkProgram(program);
    }

    /**
     * Helper method for reading in the source for a shader and creating a
     * shader object.
     *
     * @param file Filename of shader source
     * @param type Type of shader-> Only GL_VERTEX_SHADER and GL_FRAGMENT_SHADER
     *   are supported here.
     */
    GLint setShaderSource(string file, GLenum type) {
        //read source code
        ifstream fin(file.c_str());
        if (fin.fail()) {
            cerr << "Could not open " << file << " for reading" << endl;
            return -1;
        }
        fin.seekg(0, ios::end);
        int count  = fin.tellg();
        char *data = NULL;
        if (count > 0) {
            fin.seekg(ios::beg);
            data = new char[count+1];
            fin.read(data,count);
            data[count] = '\0';
        }
        fin.close();

        //create the shader
        GLint s = glCreateShader(type);
        glShaderSource(s, 1, const_cast<const char **>(&data), NULL);
        delete [] data;
        return s;
    }



    GLint program; //shader program
    GLint modelViewLoc; //location of the modelview matrix in the program (M)
    GLint projectionLoc; //location of the projection matrix in the program (P)
    GLint normalMatrixLoc; //location of the normal matrix in the program (M_n)
    GLint lBlockLoc; //uniform for blocking lighting for axes
    GLint vertexLoc, normalLoc; //vertex attribute locations (pos and norm)
      //respectively
    GLint texCoordLoc; //location of texture coordinate vertex attribute
    GLint textureLoc; //location of handle to texture data
    GLint colorLoc; //Model color
    GLint indexLoc;
    GLint timeLoc; //location of time variable
    GLint lightPosLoc;
    GLint viewPosLoc;
    GLuint vertexBuffer, normalBuffer, colorBuffer; //used to keep track of GL buffer objects
	GLuint texBuffer; //Texture buffer
    GLuint indexBuffer, axesBuffer, aNormalBuffer, aColorBuffer; //index, axes, and axes colors
};
Shader *shader = NULL;

//UBTextureGL texture;

int WIN_WIDTH = 1280, WIN_HEIGHT = 720; //window width/height
glm::mat4 modelView, projection, camera; //matrices for shaders
glm::vec3 lightPos(0,0,1), viewPos(10,10,10);
glm::mat4 modelTrans, mTrans, crTrans, csTrans, ctTrans; //Transformation matrices.
float animTime = 0.0f, deltaT = 0.0001; //variables for animation

vector<float> raw;
vector<float> verts; //vertex array
vector<float> norms;
vector<float> aNorms;
vector<float> axes;
vector<float> color; //Model color array
vector<float> aColor; //Axis and Bounding Box color array
vector<unsigned short> indices;
vector<float> texs; //texture coordinates
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

//updates values based on some change in time
void update(float dt) {
    animTime += dt;

    modelView = glm::rotate(modelView, glm::float_t(0.01), glm::vec3(1,1,0));
}

//reshape function for GLUT
void reshape(int w, int h) {
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
void display() {
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

	glBindBuffer(GL_ARRAY_BUFFER, shader->texBuffer);
    glEnableVertexAttribArray(shader->texCoordLoc);
    glVertexAttribPointer(shader->texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, NULL);

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
    glDrawArrays(GL_TRIANGLES, 0, numVerts);

    //update animation variables.
    //have time oscillate between 0.0 and 1.0.
    if ((animTime >= 1.0 && deltaT > 0.0) ||
            (animTime <= 0.0 && deltaT < 0.0)) {
        deltaT = -deltaT;
    }

    glutSwapBuffers();
}

//idle function for GLUT
void idle() {
    glutPostRedisplay();
}

//captures keyboard input for GLUT
//Sets variables for doing transforms
void keyboard(unsigned char key, int x, int y) {

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
void setupGLUT() {
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("Program 4");

    glutReshapeFunc(reshape);

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);

    glutMouseFunc(mouse);

    glutMotionFunc(mouseMove);

    glutIdleFunc(idle);
}

//initialize OpenGL background color and vertex/normal arrays
void setupGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    //initialize vertex and normal arrays
    //this is where you might want to read in your model
    axes.push_back(0);
    axes.push_back(0);
    axes.push_back(0);
    aColor.push_back(1);aColor.push_back(0);aColor.push_back(0);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(5);
    axes.push_back(0);
    axes.push_back(0);
    aColor.push_back(1);aColor.push_back(0);aColor.push_back(0);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

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

    axes.push_back(minpt.x);
    axes.push_back(minpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
	//texs.push_back(minpt.x);texs.push_back(minpt.y);

    axes.push_back(minpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
	//texs.push_back(minpt.x);texs.push_back(maxpt.y);

    axes.push_back(minpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
	//texs.push_back(minpt.x);texs.push_back(maxpt.y);

    axes.push_back(maxpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
	//texs.push_back(maxpt.x);texs.push_back(maxpt.y);

    axes.push_back(maxpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
	//texs.push_back(maxpt.x);texs.push_back(maxpt.y);

    axes.push_back(maxpt.x);
    axes.push_back(minpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
	//texs.push_back(maxpt.x);texs.push_back(minpt.y);

    axes.push_back(maxpt.x);
    axes.push_back(minpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
	//texs.push_back(maxpt.x);texs.push_back(minpt.y);

    axes.push_back(minpt.x);
    axes.push_back(minpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
	//texs.push_back(minpt.x);texs.push_back(minpt.y);

    axes.push_back(minpt.x);
    axes.push_back(minpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
	//texs.push_back(minpt.x);texs.push_back(minpt.y);

    axes.push_back(minpt.x);
    axes.push_back(minpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(maxpt.x);
    axes.push_back(minpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
    //texs.push_back(maxpt.x);texs.push_back(minpt.y);

    axes.push_back(maxpt.x);
    axes.push_back(minpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(minpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(minpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(maxpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(minpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);
    //texs.push_back(maxpt.x);texs.push_back(maxpt.y);
    //texs.push_back(1.0);texs.push_back(1.0);

    axes.push_back(maxpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(minpt.x);
    axes.push_back(minpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(maxpt.x);
    axes.push_back(minpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(maxpt.x);
    axes.push_back(minpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(maxpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(maxpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(minpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(minpt.x);
    axes.push_back(maxpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    axes.push_back(minpt.x);
    axes.push_back(minpt.y);
    axes.push_back(maxpt.z);
    aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);aColor.push_back(1);
    aNorms.push_back(1);aNorms.push_back(1);aNorms.push_back(1);

    numAxes = axes.size() / 3;

	//load texture from file
//    texture.loadFromFile("warning.jpg");
//    texture.init();

    float x_range = (maxpt.x + abs(minpt.x));
    float z_range = (maxpt.x + abs(minpt.x));

    //cout << z_range << "\n";

/*    for (int i = 0; i < verts.size(); i += 3)
    {
    	//cout << x << ", " << z << "\n";
    	//cout << ((verts[x] + abs(minpt.x))) << ", " << verts[z] << " + " << abs(minpt.z) << " = " << ((verts[z] + abs(minpt.z))) <<"\n";
    	texs.push_back((verts[i] + abs(minpt.x)) / x_range);
    	texs.push_back((verts[i+2] + abs(minpt.z)) / x_range);
    }
	*/

    camera = glm::lookAt(glm::vec3(10,10,10), glm::vec3(0,0,0), glm::vec3(0,0,1));

    projection = glm::perspective(
            glm::float_t(45),
            glm::float_t(WIN_WIDTH) / glm::float_t(WIN_HEIGHT),
            glm::float_t(0.1),
            glm::float_t(1000.0)
    );
}

//setup the shader program
void setupShaders() {
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
//	shader->textureLoc = glGetUniformLocation(shader->program, "tex0");

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


void readCoords(const char *filename )
{
    int     i, j;
    float   x, y, z;
    FILE    *f;

    minpt = glm::vec3(MAXFLOAT,MAXFLOAT,MAXFLOAT);
    maxpt = glm::vec3(MINFLOAT,MINFLOAT,MINFLOAT);

    f = fopen(filename, "r");
    if (!f)
    {
        cout << "Can not open the file " <<  filename << endl;
        exit(-1);
    }

    if (fscanf(f, "%d", &(numCoords)) != 1)
    {
        cout << "Can not read the first line of  " <<  filename << endl;
        exit(-1);
    }

    printf("NUMCOORDS %d\n", numCoords);

    for(i = 0; i < numCoords; i++)
    {

        if (fscanf(f, "%d,%f,%f,%f", &j, &x, &y, &z) != 4)
        {
        	cout << "Can't read from " << endl;
            exit(-1);
        }
        if ((j < 0) || (j > numCoords))
        {
        	cout << "Illegal index" << endl;
            exit(-1);
        }

        if (x > maxpt[0]) maxpt[0] = x;
        if (x < minpt[0]) minpt[0] = x;
        if (y > maxpt[1]) maxpt[1] = y;
        if (y < minpt[1]) minpt[1] = y;
        if (z > maxpt[2]) maxpt[2] = z;
        if (z < minpt[2]) minpt[2] = z;

        raw.push_back(x);
        raw.push_back(y);
        raw.push_back(z);
    }

    fclose(f);

    centerpt[0] =(maxpt[0] + minpt[0])/2;
    centerpt[1] =(maxpt[1] + minpt[1])/2;
    centerpt[2] =(maxpt[2] + minpt[2])/2;

    cout << "verts size = " << raw.size()/3 << endl;

}

// read .poly files
void  readPolygons(const char *filename)
{
    int     i;
    int pnt;
    int res;
    int ind;
    char    c;
    FILE    *f;
    int temp;
    int prev;


    cout << "inside read poly " << endl;

    f = fopen(filename, "r");
    if (!f)
    {
    	cout << "Can't read the file " <<  filename << endl;
    	exit(-1);
    }
    if ( fscanf(f, "%d", &(numPolygons)) != 1)
    {
    	cout << "Can't read first line of " <<  filename << endl;
    	exit(-1);
    }

    cout << raw.size()/3 << " = num coords" << endl;
    cout << numPolygons << " num poly" << endl;

    for(i = 0; i < numPolygons; i++)
    {
        do
            c = fgetc(f);
        while(!feof(f) && (c != ' '));
        ind = 0;

        do
        {
            res = fscanf(f, "%d", &pnt);
        	if (res && ind >= 3)
        	{
        	    indices.push_back(temp - 1);
        	    indices.push_back(prev - 1);
        	    verts.push_back(raw[(temp - 1) * 3]);
        	    verts.push_back(raw[((temp - 1) * 3) + 1]);
        	    verts.push_back(raw[((temp - 1) * 3) + 2]);
        	    verts.push_back(raw[(prev - 1) * 3]);
        	    verts.push_back(raw[((prev - 1) * 3) + 1]);
        	    verts.push_back(raw[((prev - 1) * 3) + 2]);
                color.push_back(1);color.push_back(1);color.push_back(1);color.push_back(1);
                color.push_back(1);color.push_back(1);color.push_back(1);color.push_back(1);
        	}
            if (res)
            {
            	indices.push_back(pnt - 1);
        	    verts.push_back(raw[(pnt - 1) * 3]);
        	    verts.push_back(raw[((pnt - 1) * 3) + 1]);
        	    verts.push_back(raw[((pnt - 1) * 3) + 2]);
                color.push_back(1);color.push_back(1);color.push_back(1);color.push_back(1);

                if (ind == 0)
                	{
                		temp = pnt;
                	}
                prev = pnt;
                ind++;
            }
            if (ind >= MAX_PTS)
			{
				cout << "Too many indices\n";
				cout << "polygon "<<  i << endl;
				exit(-1);
			}
        }
        while(!feof(f) && res);
    }

    fclose(f);
    cout <<" Number of polygons " << numPolygons << endl;
}


void load()
{
    readCoords("surfboard.coor");
    cout << "read coords file successfully" << endl;
    readPolygons("surfboard.poly");
    cout << "read surfboard file successfully" << endl;

    numIndex = indices.size();
    numVerts = verts.size()/3;
}

void findNorms()
{
	glm::vec3 nVec1;
	glm::vec3 nVec2;
	glm::vec3 rVec;
	for (int niter = 0; niter < verts.size(); niter+=9)
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

void initializeGraphics(int argc, char** argv)
{
	//A safeguard against Null errors.
	upMouseXPos = WIN_WIDTH / 2;
	downMouseXPos = WIN_WIDTH / 2;

	load();
	findNorms();
    glutInit(&argc, argv);
    setupGLUT();
    setupGL();

    glewInit();

    setupShaders();

    glutMainLoop();

	if (shader) delete shader;
}