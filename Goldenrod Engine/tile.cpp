#include "tile.h"

// Constructors/Destructor
Tile::Tile(ProcessedInputLine* inLine){

    this->tileID = inLine->getID();
    this->friction = TILE_DEFAULT_FRICTION;
    this->neighborIDs = inLine->getNeighborIDs();
    this->addTileShapes(inLine->getVerts());
	this->physics = new Physics(calculateCenter(inLine->getVerts()));

	// Caluculate slope
	this->slope = abs(this->shapes[0]->getMaxY() - this->shapes[0]->getMinY());

	// Calculate roll direction
	if(this->slope != 0.0){
		vec3 upVec = vec3(0.0,1.0,0.0);
		vec3 norm = this->shapes[0]->normals()[0];
		vec3 xVec = cross(upVec, norm);
		this->rollDirection = normalize(cross(xVec, norm));
	}
	else{
		this->rollDirection = vec3(0.0,0.0,0.0);
	}

    if(TILE_USE_BORDER){
        this->borders = new Border(inLine);
    }
    else{
        this->borders = NULL;
    }

};

Tile::~Tile(){

    this->neighborIDs.clear();
    for(int i = 0; i < this->shapes.size(); i++){
        delete this->shapes.at(i);
    }
    this->shapes.clear();
    delete this->borders;

};

// Print info
void Tile::printInfo(){

};

// Access methods
int Tile::getID(){ return this->tileID; };

double Tile::getFriction(){ return this->friction; };

vector<int> Tile::getNeighborIDs(){ return this->neighborIDs; };

vector<Shape*> Tile::getShapes(){ return this->shapes; };

Physics* Tile::getPhysics(){ return this->physics; };

Border* Tile::getBorders()
{
	return this->borders;
};

vec3 Tile::getRollDirection(){ return this->rollDirection; };

float Tile::getSlope(){ return this->slope; };

void Tile::setID(int newID){ 

    this->tileID = newID;
    this->borders->setID(newID);

};

void Tile::setFriction(double newFriction){ this->friction = newFriction; };

// Private methods
void Tile::addTileShapes(vector<vec3> verts, vec4 color, float depth){

    // Ensure depth >= 0
    depth = abs(depth);

    // Separate out new vectors with verts for each face in correct order for shape generation
    // Top and bottom
    vector<vec3> top = verts;

    // Check if need to calculate additional verts
    if(depth > 0){
        vector<vec3> bottom;
        int topVertIndex = 0;
        for(int i = 0; i < verts.size(); i++){
            bottom.push_back(glm::vec3(top[topVertIndex].x, top[topVertIndex].y - depth, top[topVertIndex].z));

            topVertIndex--;
            if(topVertIndex < 0){
                topVertIndex = top.size() - 1;
            }
        }

        // Sides
        // Initial vert index setup
        int firstVertIndex = 0, // index of top vert
            secondVertIndex = 0, // index of bottom vert
            thirdVertIndex = (bottom.size() - 1), // index of bottom vert
            fourthVertIndex = 1; // index of top vert

        vector<glm::vec3> sides; // Store all side verts 

        for(int i = 0; i < verts.size(); i++){ // For each side
            sides.push_back(top[firstVertIndex]);
            sides.push_back(bottom[secondVertIndex]);
            sides.push_back(bottom[thirdVertIndex]);
            sides.push_back(top[fourthVertIndex]);

            // Update vert indices
            firstVertIndex--;
            secondVertIndex++;
            thirdVertIndex++;
            fourthVertIndex--;

            // Correct for wrap around
            if(firstVertIndex < 0){
                firstVertIndex = (top.size() - 1);
            }
            if(secondVertIndex > (bottom.size() - 1)){
                secondVertIndex = 0;
            }
            if(thirdVertIndex > (bottom.size() - 1)){
                thirdVertIndex = 0;
            }
            if(fourthVertIndex < 0){
                fourthVertIndex = (top.size() - 1);
            }
        }
       
        vector<glm::vec3> tmpVec;
        vector<glm::vec3>::iterator it;

        this->shapes.push_back(new Shape(top, color));
        this->shapes.push_back(new Shape(bottom, color));
        for(it = sides.begin(); it != sides.end(); it+=4){
            tmpVec.clear();
            tmpVec.insert(tmpVec.begin(), it, (it +4));
            this->shapes.push_back(new Shape(tmpVec, color));
        }

    }
    else{
        this->shapes.push_back(new Shape(top, color));
    }

};

vec3 Tile::calculateCenter(vector<vec3> verts){

	float x = 0,y = 0,z = 0;
    for(int i = 0; i < verts.size(); i++){
        x += verts.at(i).x;
        y += verts.at(i).y;
        z += verts.at(i).z;
    }
    x = x / verts.size();
    y = y / verts.size();
    z = z / verts.size();

    return vec3(x,y,z);

};
