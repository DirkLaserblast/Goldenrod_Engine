#ifndef TEE_H
#define TEE_H

#include <vector>

#define TEE_RADIUS 0.1f
#define TEE_PLANE_OFFSET 0.001f
#define TEE_COLOR glm::vec4(0.0,0.0,1.0,1.0)

using namespace std;

class Tee{

public:
    // Functions
	Tee(int ID, int locIndex, int shapeIndex);
	~Tee();

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