#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <iostream>

#include "component.h"

using std::vector;
using std::string;
using std::cerr;
using std::cout;
using std::endl;

enum ENTITY_TYPE { eNONE_T, eSTATIC_GEOMETRY_T, eDYNAMIC_GEOMETRY_T, eLIGHT_T, eCAMERA_T };
const string entityTypes[] = { "NONE", "eSTATIC_GEOMETRY_T", "eDYNAMIC_GEOMETRY_T", "eLIGHT_T", "eCAMERA_T" };

class Entity{

public:
    Entity ();
    Entity (ENTITY_TYPE type);
    ~Entity();
	void update();
    void attachComponent(Component* component);
    void deleteComponent(COMPONENT_TYPE type);
    void printInfo();

private:
    string name; // Assigned name for entity
    ENTITY_TYPE type; // Entity type tag for object   
    vector<Component*> components; // Vector of pointers to attached components
       
};

#endif