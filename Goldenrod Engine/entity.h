#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "gameobject.h"
#include "component.h"

using namespace std;

enum ENTITY_TYPE { eNONE_T, eSTATIC_GEOMETRY_T, eDYNAMIC_GEOMETRY_T, eLIGHT_T, eCAMERA_T };
const string entityTypes[] = { NONE, "eSTATIC_GEOMETRY_T", "eDYNAMIC_GEOMETRY_T", "eLEVEL_T", "eLIGHT_T", "eCAMERA_T" };

class Entity: public GameObject{

public:
    Entity ();
    ~Entity();
	void update();
    void attachComponent(Component* component);
    void deleteComponent(COMPONENT_TYPE type);
    void printInfo();

private:
    ENTITY_TYPE type; // Entity type tag for object   
    vector<Component*> components; // Vector of pointers to attached components
    
};

#endif