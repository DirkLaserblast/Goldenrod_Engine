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

    // Calculate center -- debug
    float x = 0,y = 0,z = 0;
    for(int i = 0; i < this->shapeVertices.size(); i++){
        x += this->shapeVertices.at(i).x;
        y += this->shapeVertices.at(i).y;
        z += this->shapeVertices.at(i).z;
    }
    x = x / this->shapeVertices.size();
    y = y / this->shapeVertices.size();
    z = z / this->shapeVertices.size();
    this->center = vec3(x,y,z);

	// Calculate min and max y value
	this->minY = calculateMinY(vertices);
	this->maxY = calculateMaxY(vertices);

}

void Shape::transform(mat4 matrix)
{
    for (int i = 0; i < shapeVertices.size(); i++)
    {
        shapeVertices[i] = (vec3) (vec4(shapeVertices[i], 1.0) * matrix);
        shapeNormals[i] = (vec3) (vec4(shapeNormals[i], 1.0) * matrix);
    }
}

void Shape::translate(vec3 deltaPos)
{
    for (int i = 0; i < shapeVertices.size(); i++)
    {
        shapeVertices[i] += deltaPos;
    }

    // Update center -- debug
    center += deltaPos;
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

void Shape::reload()
{
    int pos = this->startIndex;
    //Update Pointers
    for(int i = 0; i < this->shapeVertices.size(); i++)
    {
        vec3 newVec = shapeVertices[i];
        vertsPointer->at(pos) = newVec.x;
        vertsPointer->at(pos+1) = newVec.y;
        vertsPointer->at(pos+2) = newVec.z;
        pos +=3;
    }
    pos = this->colorStartIndex;

    //Update colors
    for(int i = 0; i < this->shapeColors.size(); i++)
    {
        vec4 newColor = shapeColors[i];
        colorsPointer->at(pos) = newColor.r;
        colorsPointer->at(pos+1) = newColor.g;
        colorsPointer->at(pos+2) = newColor.b;
        colorsPointer->at(pos+3) = newColor.a;
        pos += 4;
    }
    pos = this->startIndex;

    //Update Pointers
    for(int i = 0; i < this->shapeNormals.size(); i++)
    {
        vec3 newNorm = shapeNormals[i];
        normsPointer->at(pos) = newNorm.x;
        normsPointer->at(pos+1) = newNorm.y;
        normsPointer->at(pos+2) = newNorm.z;
    }
}

//Calculates signed distance between a point and a shape (as a plane)
float Shape::distanceToPlane(vec3 point)
{
    vec3 planeNormal, planePoint, originVector; //Originvector is the vector from the original point to the plane's point
    planePoint = this->vertices().at(0);
    planeNormal = this->normals().at(0);
    originVector = planePoint - point;

    return abs(dot(originVector, normalize(planeNormal)));
}

bool Shape::checkIfInside(vec3 point, float offset){

    // Setup input for pnpoly
    int i,j;
    bool c = false;
    vector<float> xCoords;
    for(int i = 0; i < this->shapeVertices.size(); i++){
        xCoords.push_back(this->shapeVertices[i].x);
    }
    vector<float> zCoords;
    for(int i = 0; i < this->shapeVertices.size(); i++){
        zCoords.push_back(this->shapeVertices[i].z);
    }
    // pnpoly
    for (i = 0, j = this->numVertices()-1; i < this->numVertices(); j = i++) {
        if ( ((zCoords[i]>point.z) != (zCoords[j]>point.z)) &&
            (point.x < (xCoords[j]-xCoords[i]) * (point.z-zCoords[i]) / (zCoords[j]-zCoords[i]) + xCoords[i]) )
            c = !c;
    }

	if(c == true && (point.y > this->maxY + offset || point.y < this->minY - offset)){
		c = !c;
	}

    return c;

};

//Create the three vectors needed to render, using a list of shapes
void reloadAllShapes(vector<float> * vertsVector, vector<float> * colorsVector, vector<float> * normsVector, vector<Shape*> shapesVector)
{
    vertsVector->clear();
    colorsVector->clear();
    normsVector->clear();

    for(int i = 0; i < shapesVector.size(); i++)
    {
        shapesVector.at(i)->startIndex = vertsVector->size(); //Stores the current position in the verts vector into the current shape
        shapesVector.at(i)->colorStartIndex = colorsVector->size();
        shapesVector.at(i)->vertsPointer = vertsVector;
        shapesVector.at(i)->colorsPointer = colorsVector;
        shapesVector.at(i)->normsPointer = normsVector;

        Shape* currentShape = shapesVector.at(i);
        vector<float> currentVerts = currentShape->rawVerts();
        vector<float> currentColors = currentShape->rawColors();
        vector<float> currentNormals = currentShape->rawNormals();


        vertsVector->insert(vertsVector->end(), currentVerts.begin(), currentVerts.end());
        colorsVector->insert(colorsVector->end(), currentColors.begin(), currentColors.end());
        normsVector->insert(normsVector->end(), currentNormals.begin(), currentNormals.end());
    }


}

float Shape::yValueAtPoint(float x, float z){

	vec3 normal = this->shapeNormals[0];
	vec3 knownPoint = this->shapeVertices[0];

	float sum = ((normal.x*knownPoint.x)-(normal.x*x)) + ((normal.z*knownPoint.z)-(normal.z*z)) + (normal.y*knownPoint.y);

	return (sum/normal.y);

};

float Shape::getMinY(){ return this->minY; };

float Shape::getMaxY(){ return this->maxY; };

vec3 Shape::getCenter(){ return this->center; };

float Shape::calculateMinY(vector<vec3> verts){

	float minY;
    for(int i = 0; i < verts.size(); i++){
		if(i == 0){
			minY = verts.at(i).y;
		}
		else if(minY > verts.at(i).y){
			minY = verts.at(i).y;
		}
    }
    
	return minY;

};

float Shape::calculateMaxY(vector<vec3> verts){

	float maxY;
    for(int i = 0; i < verts.size(); i++){
		if(i == 0){
			maxY = verts.at(i).y;
		}
		else if(maxY < verts.at(i).y){
			maxY = verts.at(i).y;
		}
    }
    
	return maxY;

};
