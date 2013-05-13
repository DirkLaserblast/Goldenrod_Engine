#include "cup.h"

Cup::Cup(int ID, int locIndex, int shapeIndex){

    this->ID = ID;    
    this->locIndex = locIndex;
    this->shapeIndex = shapeIndex;

    this->validate();

};

Cup::~Cup(){   

};

void Cup::validate(){

    // Need to add this function

};

bool Cup::isValid(){

	return this->valid;

};

int Cup::getID(){ return ID; };

int Cup::getLocIndex(){ return locIndex; };

int Cup::getShapeIndex(){ return shapeIndex; };
