/*graphics.cpp
Functions:
Initialize OpenGL
Draw points / lines / shapes
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

	/**
	 * Helper function used for debugging.
	 */
/*	void printLog(std::string label, GLint obj) {
		int infologLength = 1;
		int maxLength = 1;

		if(glIsShader(obj)) {
			glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
		} else {
			glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
		}

		char infoLog[maxLength];

		if (glIsShader(obj)) {
			glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
		} else {
			glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
		}

		if (infologLength > 0) {
			cerr << label << infoLog << endl;
		}
	}

	*/
	GLint program; //shader program
	GLint modelViewLoc; //location of the modelview matrix in the program (M)
	GLint projectionLoc; //location of the projection matrix in the program (P)
	GLint normalMatrixLoc; //location of the normal matrix in the program (M_n)
	GLint vertexLoc, normalLoc; //vertex attribute locations (pos and norm)
	//respectively
	GLint timeLoc; //location of time variable
	GLuint vertexBuffer, normalBuffer, indexBuffer; //used to keep track of GL buffer objects
};
Shader *shader = NULL;
Shader *shaderwhite = NULL;
//Shader *board = NULL;

int WIN_WIDTH = 1280, WIN_HEIGHT = 720; //window width/height
glm::mat4 modelView, projection, camera; //matrices for shaders
float animTime = 0.0f, deltaT = 0.0001; //variables for animation

vector<float> verts; //vertex array
vector<float> norms; //normal array
size_t numVerts; //number of total vertices

bool ySel = false;
bool sSel = false;

vector <float> COOR;
vector <vector <float> > POLY;
int n = 0;

glm::vec3 minpt;
glm::vec3 maxpt; //opposite pt from origin
float MAXFLOAT;
float MINFLOAT;
int numCoords;
float centerpt[3];
int numPolygons;
vector <short> indices;
int MAX_PTS = 696; //n
int numIndex;
bool xbool = false;
bool yBool = false;
bool zBool = false;
bool sBool = false;
bool mpress = false;
bool mpressRight = false;
bool boundingBox = false;
bool transformObject = true;

float down = 0.0;
float up = 0.0;
float downY = 0.0;
float upY = 0.0;

//Translation
float changeX = 0.0;
float changeY = 0.0;
float changeZ = 0.0;
//Scaling
float changeS = 100.0;
//Rotation
float rotateX = 0.0;
float rotateY = 0.0;
float rotateZ = 0.0;

//For camera transform
float panX = 0.0f;
float panY = 0.0f;
float panZ = 0.0f;
float tiltX = 0.0f;
float tiltY = 0.0f;
float tiltZ = 0.0f;
float zoom = 200.0f;


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

		verts.push_back(x);
		verts.push_back(y);
		verts.push_back(z);
		norms.push_back(0);norms.push_back(0.3);norms.push_back(1);
	}

	fclose(f);

	centerpt[0] =(maxpt[0] + minpt[0])/2;
	centerpt[1] =(maxpt[1] + minpt[1])/2;
	centerpt[2] =(maxpt[2] + minpt[2])/2;

	cout << "verts size = " << verts.size()/3 << endl;

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

	cout << verts.size()/3 << " = num coords" << endl;
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
			if (res)
			{
				if (ind > 0) // i don't want to repeat first element
				{
					indices.push_back(pnt-1);
				}
				else if (ind == 0 && indices.size() > 0)
				{
					indices.push_back(temp-1);
				}
				indices.push_back(pnt-1);

				if (ind == 0)
				{
					temp = pnt;
				}
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



//updates values based on some change in time
void update(float dt) {
	animTime += dt;

	modelView = glm::rotate(modelView, glm::float_t(0.01), glm::vec3(1,1,0));
}



//reshape function for GLUT
void reshape(int w, int h) {
	WIN_WIDTH = w;
	WIN_HEIGHT = h;
	/*projection = glm::perspective(
    			glm::float_t(45),
    			glm::float_t(WIN_WIDTH) / glm::float_t(WIN_HEIGHT),
    			glm::float_t(0.1),
    			glm::float_t(1000.0)
    	);*/
	projection = glm::ortho(glm::float_t(-WIN_WIDTH/100),
			glm::float_t(WIN_WIDTH/100),
			glm::float_t(-WIN_HEIGHT/100),
			glm::float_t(WIN_HEIGHT/100),
			glm::float_t(-100.0f),
			glm::float_t(2000.0f));
}

void useBasic(){
	glUseProgram(shader->program);
	glBindBuffer(GL_ARRAY_BUFFER, shader->vertexBuffer); //which buffer we want
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

	//same procedure for the normal array
	glBindBuffer(GL_ARRAY_BUFFER, shader->normalBuffer);
	glEnableVertexAttribArray(shader->normalLoc);
	glVertexAttribPointer(shader->normalLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}









//display function for GLUT
void display() {
	glViewport(0,0,WIN_WIDTH,WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Setup the modelview matrix
	//    Mat4x4F modelCam = camera * modelView;
	modelView = glm::mat4();
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


	glBindBuffer(GL_ARRAY_BUFFER, shader->vertexBuffer); //which buffer we want
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

	//same procedure for the normal array
	glBindBuffer(GL_ARRAY_BUFFER, shader->normalBuffer);
	glEnableVertexAttribArray(shader->normalLoc);
	glVertexAttribPointer(shader->normalLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);






	glBindBuffer(GL_ARRAY_BUFFER, shader->vertexBuffer); //which buffer we want
	//to use
	glEnableVertexAttribArray(shader->vertexLoc); //enable the attribute
	glVertexAttribPointer(
			shader->vertexLoc, //handle to variable in shader program
			3, //vector size (e.g. for texture coordinates this could be 2).
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL
	);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader->indexBuffer);

	//Draw the axes
	glDrawArrays(GL_LINES, 696, 2);
	glDrawArrays(GL_LINES, 698, 2);
	glDrawArrays(GL_LINES, 700, 2);



	//draw the vertices/normals we just specified.


	//Update modelView to move object
	modelView = glm::translate(glm::mat4(), glm::vec3(changeX / 100, changeY / 100, changeZ / 100))
		* glm::scale(glm::mat4(), glm::vec3(changeS / 100, changeS / 100, changeS / 100))
		* glm::rotate(glm::mat4(), rotateX / 5, glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(), rotateY / 5, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(), rotateZ / 5, glm::vec3(0, 0, 1));

	camera = glm::translate(camera, glm::vec3(panX / 50, panY / 50, panZ / 50))
		* glm::rotate(glm::mat4(), tiltX / 5, glm::vec3(1, 0, 0))
		* glm::rotate(glm::mat4(), tiltY / 5, glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(), tiltZ / 5, glm::vec3(0, 0, 1));

	panX = 0.0f;
	panY = 0.0f;
	panZ = 0.0f;
	tiltX = 0.0f;
	tiltY = 0.0f;
	tiltZ = 0.0f;

	projection = glm::ortho(glm::float_t(-WIN_WIDTH/100 * (zoom/200)),
			glm::float_t(WIN_WIDTH/100 * (zoom/200)),
			glm::float_t(-WIN_HEIGHT/100 * (zoom/200)),
			glm::float_t(WIN_HEIGHT/100 * (zoom/200)),
			glm::float_t(-100.0f),
			glm::float_t(2000.0f));

	modelCam = camera * modelView;

	glUniformMatrix4fv(
			shader->modelViewLoc, //handle to variable in the shader program
			1, //how many matrices we want to send
			GL_FALSE, //transpose the matrix
			glm::value_ptr(modelCam) //a pointer to an array containing the entries for
	//the matrix
	);
	if (boundingBox)
	{
		glDrawArrays(GL_POINTS, 0, 696);
		glDrawArrays(GL_LINES, 702, 2);
		glDrawArrays(GL_LINES, 704, 2);
		glDrawArrays(GL_LINES, 706, 2);
	}
	
	glDrawElements(GL_LINES, numIndex, GL_UNSIGNED_SHORT, NULL);
	if (boundingBox) glDrawArrays(GL_LINES, 708, 18);





	glutSwapBuffers();
}

//idle function for GLUT
void idle() {
	glutPostRedisplay();
}

//Mouse function, handles mouse buttons and cursor position
void MPress(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (xbool || yBool || zBool || sBool)){
		mpress = true;
		down = x;
		downY = y;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && (xbool || yBool || zBool || sBool))
	{
		mpressRight = true;
		down = x;
		downY = y;
	}
	else{
		mpress = false;
		mpressRight = false;
	}
}

void Move(int x, int y){
	if(mpress == true)
	{
		if (transformObject)
		{
			up = x - down;
			if (xbool) changeX += up;
			if (yBool) changeY += up;
			if (zBool) changeZ += up;
			if (sBool) changeS += up;
			down = x;
		}
		else
		{
			up = x - down;
			upY = y - downY;
			if (xbool) panX += up;
			if (yBool) panY += up;
			if (zBool) panZ += up;
			if (sBool) zoom += upY;

			down = x;
			downY = y;
		}
	}
	else if (mpressRight == true)
	{
		if (transformObject)
		{
			up = x - down;
			if (xbool) rotateX += up;
			if (yBool) rotateY += up;
			if (zBool) rotateZ += up;
			down = x;
		}
		else
		{
			up = x - down;
			if (xbool) tiltX += up;
			if (yBool) tiltY += up;
			if (zBool) tiltZ += up;
			down = x;
		}
	}
}


void KeyUp(unsigned char key, int x, int y){
	if (key=='x') xbool = false, mpress = false;
	if (key=='y') yBool = false, mpress = false;
	if (key=='z') zBool = false, mpress = false;
	if (key=='s') sBool = false, mpress = false;
}


//captures keyborad input for GLUT
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'x':
		xbool = true;
		break;
	case 'y':
		yBool = true;
		break;
	case 'z':
		zBool = true;
		break;
	case 's':
		sBool = true;
		break;
	case 'r':
		changeX = 0.0;
		changeY = 0.0;
		changeZ = 0.0;
		changeS = 100.0;
		rotateX = 0.0;
		rotateY = 0.0;
		rotateZ = 0.0;
		break;
	case 'b':
		boundingBox = !boundingBox;
		break;
	case 'o':
		transformObject = true;
		break;
	case 'c':
		transformObject = false;
		break;
	}
	glutPostRedisplay();
}

//do some GLUT initialization
void setupGLUT() {
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("Surfboard");

	glutReshapeFunc(reshape);

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard);

	glutKeyboardUpFunc(KeyUp);

	glutMouseFunc(MPress);

	glutMotionFunc(Move);

	glutIdleFunc(idle);
}

//initialize OpenGL background color and vertex/normal arrays
void setupGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//initiallize vertex and normal arrays
	//this is where you might want to read in your model


	load();

	verts.push_back(-20);
	verts.push_back(0);
	verts.push_back(0);
	norms.push_back(1);norms.push_back(0);norms.push_back(0);

	verts.push_back(20);
	verts.push_back(0);
	verts.push_back(0);
	norms.push_back(1);norms.push_back(0);norms.push_back(0);

	verts.push_back(0);
	verts.push_back(-20);
	verts.push_back(0);
	norms.push_back(0);norms.push_back(1);norms.push_back(0);

	verts.push_back(0);
	verts.push_back(20);
	verts.push_back(0);
	norms.push_back(0);norms.push_back(1);norms.push_back(0);

	verts.push_back(0);
	verts.push_back(0);
	verts.push_back(-20);
	norms.push_back(0);norms.push_back(0);norms.push_back(1);

	verts.push_back(0);
	verts.push_back(0);
	verts.push_back(20);
	norms.push_back(0);norms.push_back(0);norms.push_back(1);


	//Bounding Box
	int bb_index = verts.size()/3;
	cout << bb_index << endl;
	cout << minpt.x << ", " << minpt.y << ", " << minpt.z << endl;
	cout << maxpt.x << ", " << maxpt.y << ", " << maxpt.z << endl;

	verts.push_back(minpt.x);
	verts.push_back(minpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(maxpt.x);
	verts.push_back(minpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(minpt.x);
	verts.push_back(minpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(minpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);




	verts.push_back(minpt.x);
	verts.push_back(minpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(minpt.x);
	verts.push_back(minpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);


	verts.push_back(maxpt.x);
	verts.push_back(minpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(maxpt.x);
	verts.push_back(minpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);



	verts.push_back(maxpt.x);
	verts.push_back(minpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(minpt.x);
	verts.push_back(minpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);



	verts.push_back(maxpt.x);
	verts.push_back(minpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(maxpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);



	verts.push_back(maxpt.x);
	verts.push_back(minpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(maxpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);



	verts.push_back(minpt.x);
	verts.push_back(minpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(minpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);



	verts.push_back(minpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(maxpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);




	verts.push_back(maxpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(maxpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);



	verts.push_back(maxpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(minpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);




	verts.push_back(minpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(minpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);

	verts.push_back(minpt.x);
	verts.push_back(maxpt.y);
	verts.push_back(maxpt.z);
	norms.push_back(1);norms.push_back(1);norms.push_back(1);



	camera = glm::lookAt(glm::vec3(5,5,7), glm::vec3(0,0,0), glm::vec3(0,1,0));


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

	//notice that, since the vertex attribute norm is not used in the shader
	//program, shader->normalLoc = -1. If we access norm in the shader program,
	//then this value will be >= 0.
	shader->vertexLoc = glGetAttribLocation(shader->program, "pos");
	shader->normalLoc = glGetAttribLocation(shader->program, "norm");

	//Create buffers for the vertex and normal attribute arrays
	GLuint bufs[3];
	glGenBuffers(3, bufs);

	shader->vertexBuffer = bufs[0];
	shader->normalBuffer = bufs[1];
	shader->indexBuffer = bufs[2];
	//shader->lineBuffer = bufs[2];

	//This is where we pass the vertex/normal data to the GPU.
	//In genereal, the procedure for working with buffers is:
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
			GL_STATIC_DRAW //how we intend to use the buffer
	);

	glBindBuffer(GL_ARRAY_BUFFER, shader->normalBuffer);
	glBufferData(
			GL_ARRAY_BUFFER,
			norms.size() * sizeof(float),
			norms.data(),
			GL_STATIC_DRAW
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader->indexBuffer);
	glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			indices.size() * sizeof(GLushort),
			indices.data(),
			GL_STATIC_DRAW
	);

}

void initializeGraphics(int argc, char** argv)
{
	glutInit(&argc, argv);
	setupGLUT();
	setupGL();

	glewInit();

	setupShaders();

	glutMainLoop();

	if (shader) delete shader;
}