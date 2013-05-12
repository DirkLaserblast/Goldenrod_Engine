#ifndef SYSTEM_LEVELCONTROLLER_H
#define SYSTEM_LEVELCONTROLLER_H

#include "system.h"
#include "entity.h"
#include "level.h"
#include "processedinputline.h"

class LevelController: public System{

public:   
    LevelController();
    ~LevelController();
    void update();
    void printInfo();
    void addLevel();
    void addLevel(vector<ProcessedInputLine*>* inLines);
    void deleteLevel();
    void nextLevel();
    void prevLevel();
    void goToLevel();

    vector<Shape>* getCurrentLevelShapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs
    void updateCurrentLevelShapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs

private:
    Level* currentLevel;
    vector<Level*> levels;
};

#endif