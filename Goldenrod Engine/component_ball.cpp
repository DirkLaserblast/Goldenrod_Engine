#include "component_ball.h"

Ball::Ball(){

    this->name = "NONE";
    this->type = cBALL_T;
    this->currentTileID = -1;

};

Ball::Ball(int tileID){

    this->name = "NONE";
    this->type = cBALL_T;
    this->currentTileID = tileID;

};

Ball::~Ball(){

};

void Ball::update(){

};

void Ball::printInfo(){

    this->Component::printInfo();

    // Print info specific to Ball

};
