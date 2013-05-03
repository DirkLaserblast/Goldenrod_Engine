#include "entity.h"

Entity::Entity(){

    this->name = NONE;
    this->type = eNONE_T;

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
            delete (this->components[i]);
        }
    }

};

void Entity::printInfo(){

    // Print name from super class
    this->GameObject::printInfo();

    // Print entity type
    cout << "ENTITY TYPE: " << entityTypes[this->type] << endl;

    // Print info for each component
    cout << "COMPONENTS:" << endl;
    for(int i = 0; i < this->components.size(); i++){
        (this->components[i])->printInfo();
    }
    cout << endl;

};
