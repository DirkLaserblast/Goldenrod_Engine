#include "shape.h"
#include "shader.h"

//Creates a shape made from vertexes, to be drawn with TRIANGLE_FAN
//Assumes vertexes are given in counterclockwise order

vector<vec3> Shape::vertices(){return shapeVertices;};
vector<vec4> Shape::colors(){return shapeColors;};
vector<vec3> Shape::normals(){return shapeNormals;};
int Shape::numVertices(){return shapeVertices.size();};


Shape::Shape(vector<vec3> vertices, vec4 color)
{
	shapeVertices = vertices;
	shapeColors = vector<vec4>(vertices.size(), color); //Fill colors vector with input color
	shapeNormals = vector<vec3>(vertices.size(), vec3(1)); //Fill normals vector with 1's
}

void Shape::transform(mat4 matrix)
{
	for (int i = 0; i < shapeVertices.size(); i++)
	{
		shapeVertices[i] = (vec3) (vec4(shapeVertices[i], 1.0) * matrix);
	}
}

void Shape::translate(float x, float y, float z)
{
	vec3 delta = vec3(x, y, z);

	for (int i = 0; i < shapeVertices.size(); i++)
	{
		shapeVertices[i] += delta;
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




//Create the three vectors needed to render, using a list of shapes
void reloadAllShapes(vector<float> * vertsVector, vector<float> * colorsVector, vector<float> * normsVector, vector<Shape> * shapesVector)
{
	vertsVector->clear();
	colorsVector->clear();
	normsVector->clear();

	for(int i = 0; i < shapesVector->size(); i++)
	{
		shapesVector->at(i).startIndex = vertsVector->size(); //Stores the current position in the verts vector into the current shape

		Shape currentShape = shapesVector->at(i);
		vector<float> currentVerts = currentShape.rawVerts();
		vector<float> currentColors = currentShape.rawColors();
		vector<float> currentNormals = currentShape.rawNormals();


		vertsVector->insert(vertsVector->end(), currentVerts.begin(), currentVerts.end());
		colorsVector->insert(colorsVector->end(), currentColors.begin(), currentColors.end());
		normsVector->insert(normsVector->end(), currentNormals.begin(), currentNormals.end());
	}
}

void reloadShape(vector<float> * vertsVector, vector<float> * colorsVector, vector<float> * normsVector, Shape * shape)
{
	
}
