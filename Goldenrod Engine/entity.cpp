#include "entity.h"

Entity::Entity(){

    this->name = "NONE";
    this->type = eNONE_T;

};

Entity::Entity(ENTITY_TYPE type){

    this->name = "NONE";
    this->type = type;

};

Entity::~Entity(){

};

void  Entity::update(){

};

void Entity::attachComponent(Component* component){

    this->components.push_back(component);

};

void Entity::deleteComponent(COMPONENT_TYPE type){

    for(int i = 0; i < this->components.size(); i++){
        if((this->components[i])->getType() == type){
            this->components.erase(this->components.begin() + i);
        }
    }

};

void Entity::printInfo(){

    // Print entity name
    cerr << endl << "Entity NAME: " << this->name << endl;

    // Print entity type
    cerr << "TYPE: " << entityTypes[this->type] << endl;

    // Print info for each component
    cerr << "COMPONENTS:" << endl;
    for(int i = 0; i < this->components.size(); i++){
        (this->components[i])->printInfo();
    }
    cerr << endl;

};

Component* Entity::getComponent(int index){

    if(index > this->components.size()-1){
        return NULL;
    }
    else{
        return this->components.at(index);
    }

};
