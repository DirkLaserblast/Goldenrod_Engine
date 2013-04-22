/*
 * File:			Shader.h
 * Author:			Taran Lu
 * Description:		Header file for Shader.cpp
 *
 * Created on:		Oct 10, 2012
 */

#ifndef __SHADER__H__
#define __SHADER__H__

#include "GL/glew.h"
#include "GL/freeglut.h"

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

/**
 * Simple class for keeping track of shader program and vertex attribute
 * locations.
 */
class Shader {

public:

	Shader(string vertFile, string fragFile);

	/**
     * Creates a shader program based on vertex and fragment source.
     * @param vertFile Path to vertex source
     * @param fragFile Path to fragment source
     */
    void fromFiles(string vertFile, string fragFile);

	/**
     * Helper method for setting the shader source and creating a
     * shader object.
     * @param type Type of shader-> Only GL_VERTEX_SHADER and GL_FRAGMENT_SHADER
     *   are supported here.
     */
    GLint setShaderSource(string file, GLenum type);

	/**
     * Helper function used for debugging.
     */
    void printLog(std::string label, GLint obj);

    GLint program; //shader program
    GLint modelViewLoc; //location of the modelview matrix in the program (M)
    GLint projectionLoc; //location of the projection matrix in the program (P)
    GLint normalMatrixLoc; //location of the normal matrix in the program (M_n)
    GLint lBlockLoc; //uniform for blocking lighting for axes
    GLint vertexLoc, normalLoc; //vertex attribute locations (pos and norm) respectively
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

#endif
