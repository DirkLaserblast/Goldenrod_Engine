#ifndef CUP_H
#define CUP_H

#include <vector>

using namespace std;

class Cup{

public:
    // Functions
	Cup(int ID, int locIndex);
	~Cup();

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