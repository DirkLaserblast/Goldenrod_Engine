#ifndef SYSTEM_LEVELCONTROLLER_H
#define SYSTEM_LEVELCONTROLLER_H

#include "system.h"
#include "entity.h"
#include "level_conversion.h"
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

private:
    Level* currentLevel;
    vector<Level*> levels;
};

#endif