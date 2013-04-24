#include "cup.h"

Cup::Cup(int ID, int locIndex){

    this->ID = ID;    
    this->locIndex = locIndex;    

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
