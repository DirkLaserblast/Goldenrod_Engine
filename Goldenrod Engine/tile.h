#ifndef TILE_H
#define TILE_H

#include <vector>

using namespace std;

class Tile{

public:
    // Functions
	Tile(int ID, int numEdges, int numNeighbors, int locIndex, int colIndex,
         int normIndex);
	~Tile();

    void validate();
	bool isValid();

    int getID();

    int getNumEdges();
    int getLocIndex();
    int getColIndex();
    int getNormIndex();
    int getNumNeighbors();

private:
    // Local Variables
	bool valid;

	int ID;

    int numEdges,
        locIndex,
        colIndex,
        normIndex,
        numNeighbors;
	
    // Functions    

};

#endif
