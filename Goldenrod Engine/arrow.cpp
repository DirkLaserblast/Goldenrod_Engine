#include "arrow.h"

Arrow::Arrow(glm::vec4 color){

    // Setup arrow pointing along z-axis
    this->verts.push_back(glm::vec3(0.0,0.0,0.0));
    this->verts.push_back(glm::vec3(0.0,0.0,ARROW_LENGTH));

    this->verts.push_back(glm::vec3(0.0,0.0,ARROW_LENGTH));
    this->verts.push_back(glm::vec3(-(ARROW_HEAD_LENGTH),0.0,ARROW_LENGTH-ARROW_HEAD_LENGTH));

    this->verts.push_back(glm::vec3(0.0,0.0,ARROW_LENGTH));
    this->verts.push_back(glm::vec3(ARROW_HEAD_LENGTH,0.0,ARROW_LENGTH-ARROW_HEAD_LENGTH));

    // Setup vertColors
    for(int i = 0; i < 6; i++){
        this->vertColors.push_back(color);
    }

    // Set blank transformation matrix
    this->modelTransMatrix = glm::mat4();
    this->modelRotMatrix = glm::mat4();

};

Arrow::~Arrow(){

    this->verts.clear();

};

vector<glm::vec3> Arrow::getVerts(){ return this->verts; };

vector<glm::vec4> Arrow::getVertColors(){ return this->vertColors; };

glm::mat4 Arrow::getModelTransformMatrix(){

    return (this->modelTransMatrix * this->modelRotMatrix);

};

void Arrow::rotate(float angle){

    this->modelRotMatrix = glm::mat4(); // reset matrix
    this->modelRotMatrix = glm::rotate(glm::mat4(), angle, glm::vec3(0, 1, 0));

};

void Arrow::translate(glm::vec3 newPos){

    this->modelTransMatrix = glm::translate(modelTransMatrix, glm::vec3(newPos.x, newPos.y, newPos.z));

};

void Arrow::resetTransformationMatrix(){

    this->modelRotMatrix = glm::mat4();
    this->modelTransMatrix = glm::mat4();

};
