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

void Shape::translate(vec3 deltaPos)
{
    for (int i = 0; i < shapeVertices.size(); i++)
    {
        shapeVertices[i] += deltaPos;
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
        shapesVector->at(i).vertsPointer = vertsVector;
        shapesVector->at(i).colorsPointer = colorsVector;
        shapesVector->at(i).normsPointer = normsVector;

        Shape currentShape = shapesVector->at(i);
        vector<float> currentVerts = currentShape.rawVerts();
        vector<float> currentColors = currentShape.rawColors();
        vector<float> currentNormals = currentShape.rawNormals();


        vertsVector->insert(vertsVector->end(), currentVerts.begin(), currentVerts.end());
        colorsVector->insert(colorsVector->end(), currentColors.begin(), currentColors.end());
        normsVector->insert(normsVector->end(), currentNormals.begin(), currentNormals.end());
    }


}