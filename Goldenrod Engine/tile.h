#ifndef TILE_H
#define TILE_H

#include <vector>

using namespace std;

class Tile{

public:
    // Functions
	Tile(int ID, int numEdges, int numVerts, int posIndex, int colIndex,
         int normIndex, int numNeighbors, int neighborIndex);
	~Tile();

    void validate();
	bool isValid();

    int getID();

    int getNumEdges();
    int getNumVerts();
    int getPosIndex();
    int getColIndex();
    int getNormIndex();

    int getNumNeighbors();
    int getNeighborIndex();

private:
    // Local Variables
	bool valid;

	int ID;

    int numEdges,
        numVerts,
        posIndex,
        colIndex,
        normIndex;

    int numNeighbors,
        neighborIndex;
	
    // Functions    

};

#endif
