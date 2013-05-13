#ifndef SYSTEM_TEST_H
#define SYSTEM_TEST_H

#include "system.h"
#include "entity.h"

class SystemTest: public System{

public:   
    SystemTest();
    ~SystemTest();

    void update(int input);
    void printInfo();
    
private:
    vector<Entity*> entities;

};

#endif