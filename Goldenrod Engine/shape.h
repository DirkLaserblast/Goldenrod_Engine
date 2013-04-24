#ifndef SHAPE_H
#define SHAPE_H

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/glui.h"
//#include <GL/gl.h>

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
using namespace glm;


//Shapes hold the vertexes, colors, and normals needed to render
class Shape
{
public:
	//Constructor: takes a counterclockwise list of vertices and a RGBA color
	Shape(vector<vec3> vertices, vec4 color);
	//Apply a matrix transform to all the vertices in the shape
	void transform(mat4 matrix);
	void translate(float x, float y, float z);
	//Return vertices, normals, or colors as lists of vectors
	vector<vec3> vertices();
	vector<vec4> colors();
	vector<vec3> normals();
	//Return as raw lists of floats
	vector<float> rawVerts();
	vector<float> rawColors();
	vector<float> rawNormals();
	//Number of vertices in the shape
	int numVertices();
	
	//Stores the shape's starting position in the main vertex array
	int startIndex;

	private:
	vector<float> convert(vector<vec3> in);
	vector<float> convert(vector<vec4> in);
	vector<vec3> shapeVertices;
	vector<vec4> shapeColors;
	vector<vec3> shapeNormals;
};

//Erase and populate the rendering arrays
void reloadAllShapes(vector<float> * vertsVector, vector<float> * colorsVector, vector<float> * normsVector, vector<Shape> * shapesVector);

//Update a single shape in the rendering arrays without clearing them
void reloadShape(vector<float> * vertsVector, vector<float> * colorsVector, vector<float> * normsVector, Shape * shape);

//Remove a shape from the rendering arrays, leaving the rest intact
void deleteShape(vector<float> * vertsVector, vector<float> * colorsVector, vector<float> * normsVector, Shape * shape);

#endif