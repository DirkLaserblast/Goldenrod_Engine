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

	vec3 v1 = vec3(shapeVertices[1] - shapeVertices[0]);
	vec3 v2 = vec3(shapeVertices[2] - shapeVertices[0]);
	vec3 normal = normalize(cross(v1, v2));

	shapeNormals = vector<vec3>(vertices.size(), normal);


}

void Shape::transform(mat4 matrix)
{
	for (int i = 0; i < shapeVertices.size(); i++)
	{
		shapeVertices[i] = (vec3) (vec4(shapeVertices[i], 1.0) * matrix);
		shapeNormals[i] = (vec3) (vec4(shapeNormals[i], 1.0) * matrix);
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

void Shape::changeColor(vec4 color)
{
	for (int i = 0; i < shapeColors.size(); i++)
	{
		shapeColors[i] = color;
	}
}

//Converts vector of vec3 to vector of floats
vector<float> Shape::convert(vector<vec3> in)
{
	vector<float> out;
	for (int i = 0; i < in.size(); i++)
	{
		out.push_back(in[i].x);
		out.push_back(in[i].y);
		out.push_back(in[i].z);
	}
	return out;
}

vector<float> Shape::convert(vector<vec4> in)
{
	vector<float> out;
	for (int i = 0; i < in.size(); i++)
	{
		out.push_back(in[i].r);
		out.push_back(in[i].g);
		out.push_back(in[i].b);
		out.push_back(in[i].a);
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

void Shape::reload(vector<float> * vertsVector, vector<float> * colorsVector, vector<float> * normsVector)
{
	int pos = this->startIndex;
	//Update vectors
	for(int i = 0; i < this->shapeVertices.size(); i++)
	{
		vec3 newVec = shapeVertices[i];
		vertsVector->at(pos) = newVec.x;
		vertsVector->at(pos+1) = newVec.y;
		vertsVector->at(pos+2) = newVec.z;
		pos +=3;
	}
	pos = this->colorStartIndex;

	//Update colors
	for(int i = 0; i < this->shapeColors.size(); i++)
	{
		vec4 newColor = shapeColors[i];
		colorsVector->at(pos) = newColor.r;
		colorsVector->at(pos+1) = newColor.g;
		colorsVector->at(pos+2) = newColor.b;
		colorsVector->at(pos+3) = newColor.a;
		pos += 4;
	}
	pos = this->startIndex;

	//Update vectors
	for(int i = 0; i < this->shapeNormals.size(); i++)
	{
		vec3 newNorm = shapeNormals[i];
		normsVector->at(pos) = newNorm.x;
		normsVector->at(pos+1) = newNorm.y;
		normsVector->at(pos+2) = newNorm.z;
	}
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
		shapesVector->at(i).colorStartIndex = colorsVector->size();

		Shape currentShape = shapesVector->at(i);
		vector<float> currentVerts = currentShape.rawVerts();
		vector<float> currentColors = currentShape.rawColors();
		vector<float> currentNormals = currentShape.rawNormals();


		vertsVector->insert(vertsVector->end(), currentVerts.begin(), currentVerts.end());
		colorsVector->insert(colorsVector->end(), currentColors.begin(), currentColors.end());
		normsVector->insert(normsVector->end(), currentNormals.begin(), currentNormals.end());
	}


}

