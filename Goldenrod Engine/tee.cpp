#include "tee.h"

Tee::Tee(int ID, int locIndex, int shapeIndex){

    this->ID = ID;    
    this->locIndex = locIndex;
    this->shapeIndex = shapeIndex;

    this->validate();

};

Tee::~Tee(){   

};

void Tee::validate(){

    // Need to add this function

};

bool Tee::isValid(){

	return this->valid;

};

int Tee::getID(){ return ID; };

int Tee::getLocIndex(){ return locIndex; };

int Tee::getShapeIndex(){ return shapeIndex; };
