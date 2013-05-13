#include "system_test.h"

SystemTest::SystemTest(){

    this->type = sLEVEL_CONTROLLER_T;
    this->entities.clear();

};

SystemTest::~SystemTest(){
    
};

void SystemTest::update(int input){
    for(int i = 0; i < this->entities.size(); i++){
        this->entities.at(i)->update();
    }
};

void SystemTest::printInfo(){
    
};