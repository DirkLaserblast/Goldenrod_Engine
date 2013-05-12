#include "component_collision.h"

Collision::Collision(){

    this->name = "NONE";
    this->type = cCOLLISION_T;
    this->center = glm::vec3(0.0,0.0,0.0);
    this->collisionVerts.clear();
    this->collisionShapes.clear();

};

Collision::Collision(glm::vec3 center, vector<glm::vec3> collisionVerts){

    this->name = "NONE";
    this->type = cCOLLISION_T;
    this->center = center;
    this->collisionVerts = collisionVerts;
    this->collisionShapes.clear();

};

Collision::~Collision(){

};

void Collision::update(){

};

void Collision::printInfo(){

    this->Component::printInfo();

    // Print info specific to Collision

};

void Collision::addBorderCollisionShapes(vector<glm::vec3> collisionVerts, float height, float thickness){
    
    this->generateBorderCollisionShapes(this->collisionShapes, collisionVerts, height, thickness);

};

void Collision::addBallCollisionShapes(vector<glm::vec3> collisionVerts, glm::vec4 color, float depth){
    
    this->generateBallCollisionShapes(this->collisionShapes, collisionVerts, color, depth);

};

void Collision::generateBorderCollisionShapes(vector<Shape*>& shapes, vector<glm::vec3> collisionVerts, float height, float thickness){
    
};

void Collision::generateBallCollisionShapes(vector<Shape*>& shapes, vector<glm::vec3> collisionVerts, glm::vec4 color, float depth){
    
};
