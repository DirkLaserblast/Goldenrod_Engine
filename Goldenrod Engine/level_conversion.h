#ifndef LEVEL_CONVERSION_H
#define LEVEL_CONVERSION_H

#include <string>
#include <iostream>
#include <vector>

#include "entity.h"

#include "processedinputline.h"

#include "component.h"
#include "component_VBO.h"
#include "component_tile.h"
#include "component_border.h"
#include "component_cup.h"
#include "component_tee.h"
#include "component_shapes.h"

using std::string;
using std::cerr;
using std::cout;
using std::vector;

class Level{

public:
    Level();

    /* Level: constructor that create entities for level and add required components based on spec from file
       @param: processed data from file
    */
    Level(vector<ProcessedInputLine*>* inLines);
    ~Level();
    void update();
    void printInfo();

    void addTile(ProcessedInputLine& inLine);
    void deleteTile(); // Currently does nothing
    void addCup(ProcessedInputLine& inLine);
    void deleteCup(); // Currently does nothing
    void addTee(ProcessedInputLine& inLine);
    void deleteTee(); // Currently does nothing

    int getLevelCount();

    vector<Shape>* getCurrentLevelShapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs
    void updateCurrentLevelShapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs

private:
    vector<Shape>* currentLevelShapes; // REMOVE THIS AFTER CONVERT TO USING VBOs

    static int levelCount; // Total level objects

    string name; // Assigned name for component
    string type; // Tag to identify object as a level
    int ID;
    bool valid;
    Entity* tee;
    Entity* cup;
    vector<Entity*> tiles;
    vector<Entity*> borders;

    /* squareFromPoint: returns vertices for specified square (on x-z plane) from a given point
       @param: input point
       @param: square width (x-axis)
       @param: square height (z-axis)
       @param: offset from given point
    */
    vector<glm::vec3> squareFromPoint(glm::vec3 point, float width, float height, float offset);

};

#endif