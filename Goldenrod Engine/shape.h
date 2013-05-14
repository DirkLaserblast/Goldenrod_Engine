#ifndef SHAPE_H
#define SHAPE_H

#define GLM_SWIZZLE
#define PICKINGSHAPE_ID 700
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

#include <vector>
#include <string>

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
    void translate(vec3 deltaPos);
    void changeColor(vec4 color);
    void reload(); //Reloads the shape's verts in to main arrays
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
    float distanceToPlane(vec3 point); //Find the distance from a point to a shape, for collision detection with the ball
    bool checkIfInside(vec3 point); // Find out if point is inside of tile borders

    //Stores the shape's starting position in the main vertex array and normal array
    int startIndex;
    int colorStartIndex;
    vector<float> * vertsPointer, * colorsPointer, * normsPointer;

private:
    vector<float> convert(vector<vec3> in);
    vector<float> convert(vector<vec4> in);
    vector<vec3> shapeVertices;
    vector<vec4> shapeColors;
    vector<vec3> shapeNormals;
    vec3 center;
};

//Erase and populate the rendering arrays
void reloadAllShapes(vector<float> * vertsVector, vector<float> * colorsVector, vector<float> * normsVector, vector<Shape> * shapesVector);

//Remove a shape from the rendering arrays, leaving the rest intact
void deleteShape(vector<float> * vertsVector, vector<float> * colorsVector, vector<float> * normsVector, Shape * shape);

#endif