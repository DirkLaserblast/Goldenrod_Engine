#ifndef COMPONENT_BALL_H
#define COMPONENT_BALL_H

#include "component.h"

#define BALL_COLOR glm::vec4(1.0,1.0,1.0,1.0) // WHITE
#define BALL_DEPTH 0.0f
#define BALL_RADIUS 0.1f
#define BALL_OFFSET 0.005f
#define BALL_DEGREE 45 // Lower than 45 doesn't work; maybe an issue with shapes

class Ball: public Component{

public:
    Ball();
    Ball(int tileID);
    ~Ball();

    void update();
    void printInfo();

    int getCurrentTileID();

private:
    int currentTileID;

};

#endif