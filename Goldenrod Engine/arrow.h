#ifndef ARROW_H
#define ARROW_H

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"

#include "component_ball.h"

#define ARROW_COLOR glm::vec4(0.0,0.0,0.0,1.0) // BLACK
#define ARROW_LENGTH 0.3f
#define ARROW_HEAD_LENGTH 0.05f // sbutracted from ARROW_LENGTH for x-pos

using std::vector;

class Arrow{

public:
    Arrow();
    ~Arrow();

    vector<glm::vec3> getVerts();
    vector<glm::vec4> getVertColors();
    glm::mat4 getModelTransformMatrix();

    void rotate(float angle);
    void translate(glm::vec3 newPos);

private:
    vector<glm::vec3> verts;
    vector<glm::vec4> vertColors;
    glm::mat4 modelTransMatrix;
    glm::mat4 modelRotMatrix;

};

#endif