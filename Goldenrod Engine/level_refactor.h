#ifndef LEVEL_H
#define LEVEL_H

// Includes
#include <string>
#include <iostream>
#include <vector>

#include "glm/glm.hpp"

#include "processedinputline.h"

#include "tile.h"
#include "ball.h"
#include "cup.h"
#include "tee.h"

// Level macros
#define DEFAULT_LEVEL "hole.00.db"
#define DEFAULT_COURSE "course.db"

// Using
using std::string;
using std::cerr;
using std::cout;
using std::vector;

class Level{

public:
    // Constructors/Destructor
    Level(vector<ProcessedInputLine*> inLines, int ID);
    ~Level();

    // Print info
    void printInfo();

    // Add/delete/set level elements
    void addTile(ProcessedInputLine* inLine);
    void deleteTile(); // Currently does nothing
    void addCup(ProcessedInputLine* inLine);
    void deleteCup(); // Currently does nothing
    void addTee(ProcessedInputLine* inLine);
    void deleteTee(); // Currently does nothing
    void addBall(ProcessedInputLine* inLine);
    void deleteBall(); // Currently does nothing
	void setCourseName(string name);
	void setLevelName(string name);
	void setPar(int par);

    // Access methods
    int getLevelCount();

    int getLevelID();
	string getCourseName();
    string getLevelName();
	int getPar();
    vector<Tile*> getTiles();
    Tile* getTile(int tileID);
    Cup* getCup();
    Tee* getTee();
    Ball* getBall();

    vector<Shape*> getLevelShapes();

    // Update level shapes
    void updateLevelShapes();

private:
    static int levelCount; // Total level objects

    int levelID;
	string courseName;
    string levelName; // Assigned name for level
	int par;
    vector<Tile*> tiles;
    Cup* cup;
    Tee* tee;
    Ball* ball;

    vector<Shape*> levelShapes;

	
};

#endif