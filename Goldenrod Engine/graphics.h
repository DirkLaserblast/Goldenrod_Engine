#ifndef GRAPHICS_H
#define GRAPHICS_H

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

#define MAX_PTS 40
#define MAXFLOAT ((float)3.40282346638528860e+38) // taken from math.h
#define MINFLOAT (-MAXFLOAT)

//Starts up OpenGL
void initializeGraphics(int argc, char** argv, char* programName, int windowWidth, int windowHeight);

//Add a shape, made up of a list of vertices (counterclockwise order), and a single RGBA color
void addShape (vector<vec3> vertices, vec4 color);

#endif