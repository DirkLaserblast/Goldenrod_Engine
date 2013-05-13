#include "system_physics.h"

Physics::Physics(){

    this->type = sPHYSICS_T;
    this->entities.clear();

};

Physics::~Physics(){
    
};

void Physics::update(float friction){
    for(int i = 0; i < this->entities.size(); i++){
        this->entities.at(i)->publicPhysics->update(friction);
    }
};

void Physics::printInfo(){
    
};

void Physics::addEntity(Entity* newEntity){
    
    this->entities.push_back(newEntity);

};

Entity* Physics::getEntity(int index){
    
    if(index > (this->entities.size() - 1) || index < 0){
        return NULL;
    }
    else{
        return this->entities.at(index);
    }
};