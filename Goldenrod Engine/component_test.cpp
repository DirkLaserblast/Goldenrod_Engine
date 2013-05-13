#include "component_test.h"

ComponentTest::ComponentTest(){

    this->name = "NONE";
    this->type = cTEST_T;

};

ComponentTest::~ComponentTest(){

};

void ComponentTest::update(int input){

    cout << input << endl;

};

void ComponentTest::printInfo(){

    this->Component::printInfo();

    // Print info specific to Ball

};
