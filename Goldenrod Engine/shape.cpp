#include "shape.h"

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

//Creates a shape made from vertexes, to be drawn with TRIANGLE_FAN
//Assumes vertexes are given in counterclockwise order

vector<vec3> Shape::vertices(){return shapeVertices;};
vector<vec4> Shape::colors(){return shapeColors;};
vector<vec3> Shape::normals(){return shapeNormals;};
int Shape::numVertices(){return shapeVertices.size();};


Shape::Shape(vector<vec3> vertices, vec4 color)
{
	shapeVertices = vertices;
	shapeColors = vector<vec4>(vertices.size(), color);
	shapeNormals = vector<vec3>(vertices.size(), vec3(1));
}

void Shape::transform(mat4 matrix)
{
	for (int i = 0; i < shapeVertices.size(); i++)
	{
		shapeVertices[i] = (vec3) (vec4(shapeVertices[i], 1.0) * matrix);
	}
}

//Converts vector of vec3 to vector of floats
vector<float> Shape::convert(vector<vec3> in)
{
	vector<float> out;
	for (int i = 0; i < shapeVertices.size(); i++)
	{
		out.push_back(shapeVertices[i].x);
		out.push_back(shapeVertices[i].y);
		out.push_back(shapeVertices[i].z);
	}
	return out;
}

vector<float> Shape::convert(vector<vec4> in)
{
	vector<float> out;
	for (int i = 0; i < shapeColors.size(); i++)
	{
		out.push_back(shapeColors[i].r);
		out.push_back(shapeColors[i].g);
		out.push_back(shapeColors[i].b);
		out.push_back(shapeColors[i].a);
	}
	return out;
}

vector<float> Shape::rawVerts()
{
	return convert(shapeVertices);
}

vector<float> Shape::rawColors()
{
	return convert(shapeColors);
}

vector<float> Shape::rawNormals()
{
	return convert(shapeNormals);
}

void reloadShapes(vector<float> * vertsVector, vector<float> * colorsVector, vector<float> * normsVector, vector<Shape> * shapesVector)
{
	vertsVector->clear();
	colorsVector->clear();
	normsVector->clear();

	for(int i = 0; i < shapesVector->size(); i++)
	{
		shapesVector->at(i).startIndex = vertsVector->size();
		vertsVector->insert(vertsVector->end(), shapesVector->at(i).rawVerts().begin(), shapesVector->at(i).rawVerts().end());
//		colorsVector->insert(colorsVector->end(), shapesVector->at(i).rawColors().begin(), shapesVector->at(i).rawColors().end());
//		normsVector->insert(normsVector->end(), shapesVector->at(i).rawNormals().begin(), shapesVector->at(i).rawNormals().end());
	}
}
