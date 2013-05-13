#include "renderball.h"

RenderBall::RenderBall(){
    
    glm::vec3 point = glm::vec3(0.0,0.0,0.0);

    // Setup ball points
    verts.push_back(point);
    verts.push_back(glm::vec3(point.x, point.y + BALL_OFFSET, point.z + BALL_RADIUS));
    verts.push_back(glm::vec3(point.x + 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z + 0.7*BALL_RADIUS));

    verts.push_back(point);
    verts.push_back(glm::vec3(point.x + 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z + 0.7*BALL_RADIUS));
    verts.push_back(glm::vec3(point.x + BALL_RADIUS, point.y + BALL_OFFSET, point.z));

    verts.push_back(point);
    verts.push_back(glm::vec3(point.x + BALL_RADIUS, point.y + BALL_OFFSET, point.z));
    verts.push_back(glm::vec3(point.x + 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z - 0.7*BALL_RADIUS));

    verts.push_back(point);
    verts.push_back(glm::vec3(point.x + 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z - 0.7*BALL_RADIUS));
    verts.push_back(glm::vec3(point.x, point.y + BALL_OFFSET, point.z - BALL_RADIUS));

    verts.push_back(point);
    verts.push_back(glm::vec3(point.x, point.y + BALL_OFFSET, point.z - BALL_RADIUS));
    verts.push_back(glm::vec3(point.x - 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z - 0.7*BALL_RADIUS));

    verts.push_back(point);
    verts.push_back(glm::vec3(point.x - 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z - 0.7*BALL_RADIUS));
    verts.push_back(glm::vec3(point.x - BALL_RADIUS, point.y + BALL_OFFSET, point.z));

    verts.push_back(point);
    verts.push_back(glm::vec3(point.x - BALL_RADIUS, point.y + BALL_OFFSET, point.z));
    verts.push_back(glm::vec3(point.x - 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z + 0.7*BALL_RADIUS));

    verts.push_back(point);
    verts.push_back(glm::vec3(point.x - 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z + 0.7*BALL_RADIUS));
    verts.push_back(glm::vec3(point.x, point.y + BALL_OFFSET, point.z + BALL_RADIUS));

    /*verts.push_back(glm::vec3(point.x, point.y + BALL_OFFSET, point.z + BALL_RADIUS));
    verts.push_back(glm::vec3(point.x + 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z + 0.7*BALL_RADIUS));
    verts.push_back(glm::vec3(point.x + BALL_RADIUS, point.y + BALL_OFFSET, point.z));
    verts.push_back(glm::vec3(point.x + 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z - 0.7*BALL_RADIUS));
    verts.push_back(glm::vec3(point.x, point.y + BALL_OFFSET, point.z - BALL_RADIUS));
    verts.push_back(glm::vec3(point.x - 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z - 0.7*BALL_RADIUS));
    verts.push_back(glm::vec3(point.x - BALL_RADIUS, point.y + BALL_OFFSET, point.z));
    verts.push_back(glm::vec3(point.x - 0.7*BALL_RADIUS, point.y + BALL_OFFSET, point.z + 0.7*BALL_RADIUS));*/

    // Setup norms
    for(int i = 0; i < 24; i++){
        this->norms.push_back(glm::vec3(0.0,1.0,0.0));
    }

    // Setup vertColors
    for(int i = 0; i < 24; i++){
        this->vertColors.push_back(BALL_COLOR);
    }

    // Set blank transformation matrix
    this->modelTransMatrix = glm::mat4();
    this->modelRotMatrix = glm::mat4();

};

RenderBall::~RenderBall(){

    this->verts.clear();

};

vector<glm::vec3> RenderBall::getVerts(){ return this->verts; };

vector<glm::vec3> RenderBall::getNorms(){ return this->norms; };

vector<glm::vec4> RenderBall::getVertColors(){ return this->vertColors; };

glm::mat4 RenderBall::getModelTransformMatrix(){

    return (this->modelTransMatrix * this->modelRotMatrix);

};

void RenderBall::rotate(float angle){

    this->modelRotMatrix = glm::mat4(); // reset matrix
    this->modelRotMatrix = modelRotMatrix*(glm::rotate(glm::mat4(), angle, glm::vec3(0, 1, 0)));

};

void RenderBall::translate(glm::vec3 newPos){
    
    this->modelTransMatrix = glm::translate(modelTransMatrix, glm::vec3(newPos.x, newPos.y, newPos.z));

};
