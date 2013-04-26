#ifndef LEVEL_H
#define LEVEL_H

#include <cstdlib>
#include <vector>

#include "glm/glm.hpp"

#include "shape.h"
#include "tile.h"
#include "tileborder.h"
#include "tee.h"
#include "cup.h"

#define TILE_DEPTH_FLAG true // Determine if tiles will be generated with depth
#define TILE_BORDER_FLAG false // Determine if tile will be generated with borders

using namespace std;

class Level{

public:
	Level();
	~Level();

	static int getTotalLevels();
	int getID();
    vector<Shape>* getShapes();
    void validate();
	bool isValid();
    bool addTile(int ID, int numEdges, int numNeighbors, vector<glm::vec3> verts,
        vector<int> neighborIDs, bool addBorderFlag = TILE_BORDER_FLAG, bool addDepthFlag = TILE_DEPTH_FLAG);
    bool addTee(int ID, glm::vec3 loc);
    bool addCup(int ID, glm::vec3 loc);

    /* generateDepthShapes: Creates shapes to diplay tile as 3D slab.
     *                      Does not work for verticle tiles.
     */
    vector<Shape> generateDepthShapes(vector<glm::vec3> verts, glm::vec4 color);

private:
	static int totalLevels;
	int ID;
	bool valid;
    Tee* tee;
    Cup* cup;
	vector<Tile> tiles;
    vector<Border> borders;
    vector<glm::vec3> verts;
    vector<glm::vec3> cols;
    vector<glm::vec3> norms;
    vector<Shape> shapes;
	
    bool addBorder(int ID, vector<glm::vec3> edgeVerts, bool addDepthFlag = TILE_DEPTH_FLAG);

};

#endif
