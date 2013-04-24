#ifndef TEE_H
#define TEE_H

#include <vector>

using namespace std;

class Tee{

public:
    // Functions
	Tee(int ID, int locIndex);
	~Tee();

    void validate();
	bool isValid();
    int getID();    
    int getLocIndex();    

private:
    // Local Variables
	bool valid;
	int ID;
    int locIndex;   

};

#endif