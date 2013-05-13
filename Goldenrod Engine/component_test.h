#ifndef COMPONENT_TEST_H
#define COMPONENT_TEST_H

#include "component.h"

class ComponentTest: public Component{

public:
    ComponentTest();
    ~ComponentTest();

    void update(int input);
    void printInfo();

private:

};

#endif