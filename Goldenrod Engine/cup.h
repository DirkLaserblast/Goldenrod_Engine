#ifndef CUP_H
#define CUP_H

#include <vector>

#define CUP_RADIUS 0.1f
#define CUP_PLANE_OFFSET 0.001f
#define CUP_COLOR glm::vec4(1.0,0.0,0.0,1.0)

using namespace std;

class Cup{

public:
    // Functions
	Cup(int ID, int locIndex, int shapeIndex);
	~Cup();

    void validate();
	bool isValid();
    int getID();    
    int getLocIndex();
    int getShapeIndex();

private:
    // Local Variables
	bool valid;
	int ID;
    int locIndex;   
    int shapeIndex;

};

#endif