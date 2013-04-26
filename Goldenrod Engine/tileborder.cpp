#include "tileborder.h"

Border::Border(int ID, int numShapes, int locIndex, int colIndex, int normIndex, int shapeIndex){

    this->ID = ID;
    this->numShapes = numShapes;
    this->locIndex = locIndex;
    this->colIndex = colIndex;
    this->normIndex = normIndex;
    this->shapeIndex = shapeIndex;

    this->validate();

};

Border::~Border(){   

};

void Border::validate(){

    // Need to add this function

};

bool Border::isValid(){

	return this->valid;

};

int Border::getID(){ return ID; };

int Border::getNumShapes(){ return numShapes; };

int Border::getLocIndex(){ return locIndex; };

int Border::getColIndex(){ return colIndex; };

int Border::getNormIndex(){ return normIndex; };

int Border::getShapeIndex(){ return shapeIndex; };
