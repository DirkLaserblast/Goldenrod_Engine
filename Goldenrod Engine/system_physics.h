#ifndef SYSTEM_PHYSICS_H
#define SYSTEM_PHYSICS_H

#include "system.h"
#include "entity.h"

class Physics: public System{

public:   
    Physics();
    ~Physics();

    void update(float friction);
    void printInfo();

    void addEntity(Entity* newEntity);
    Entity* getEntity(int index);
    
private:
    vector<Entity*> entities;

};

#endif