#ifndef LEVEL_CONVERSION_H
#define LEVEL_CONVERSION_H

#include <string>
#include <iostream>
#include <vector>

#include "component.h"
#include "component_VBO.h"
#include "component_tile.h"
#include "entity.h"
#include "processedinputline.h"

using std::string;
using std::cerr;
using std::cout;
using std::vector;

class Level{

public:
    Level();
    Level(vector<ProcessedInputLine>& inLines);
    ~Level();
    void update();
    void printInfo();

    void addTile();
    void deleteTile();
    void addCup();
    void deleteCup();
    void addTee();
    void deleteTee();

private:
    string name; // Assigned name for component
    string type; // Tag to identify object as a level
    int ID;
    bool valid;
    Entity* tee;
    Entity* cup;
    vector<Entity*> tiles;
    vector<Entity*> borders;

};

#endif