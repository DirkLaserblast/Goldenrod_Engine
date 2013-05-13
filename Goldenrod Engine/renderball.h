#ifndef RENDERBALL_H
#define RENDERBALL_H

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

#include "component_ball.h"

using std::vector;

class RenderBall{

public:
    RenderBall();
    ~RenderBall();

    vector<glm::vec3> getVerts();
    vector<glm::vec3> getNorms();
    vector<glm::vec4> getVertColors();
    glm::mat4 getModelTransformMatrix();

    void rotate(float angle);
    void translate(glm::vec3 newPos);

private:
    vector<glm::vec3> verts;
    vector<glm::vec3> norms;
    vector<glm::vec4> vertColors;
    glm::mat4 modelTransMatrix;
    glm::mat4 modelRotMatrix;

};

#endif