#include "border.h"

// Constructors/Destructor
Border::Border(ProcessedInputLine* inLine){

    this->tileID = inLine->getID();
    this->addBorderShapes(inLine->getVerts(), inLine->getNeighborIDs());

};

Border::~Border(){

    for(int i = 0; i < this->shapes.size(); i++){
        delete this->shapes.at(i);
    }
    this->shapes.clear();

};

// Print info
void Border::printInfo(){

};

// Access Methods
int Border::getID(){ return this->tileID; };

vector<Shape*> Border::getShapes(){ return this->shapes; };

void Border::setID(int newID){ this->tileID = newID; };

// Private Methods
void Border::addBorderShapes(vector<vec3> verts, vector<int> neighborIDs, vec4 color, float height, float thickness){

    glm::vec3 vert1,
              vert2,
              vert3,
              vert4,
              normal; // store normal to extrude along

    vector<glm::vec3> side;

    // Check each side and generate border if no neighbor
    for(int i = 0; i < neighborIDs.size(); i++){
        // Skip all tile that have neighbors
        if(neighborIDs[i] == 0){
            // Clear variables
            side.clear();

            // Special case for edge joining first and last vert
            if(i == (neighborIDs.size() - 1)){
                // Grab correct verts
                vert1 = verts[i];
                vert2 = verts[0];
                vert3 = glm::vec3(vert2.x, vert2.y + height, vert2.z);
                vert4 = glm::vec3(vert1.x, vert1.y + height, vert1.z);              

                // Push inside shape
                side.push_back(vert1);
                side.push_back(vert2);
                side.push_back(vert3);
                side.push_back(vert4);
                this->shapes.push_back(new Shape(side, color));

                side.clear();

                // Modify vert positions for next shape
                normal = shapes.back()->normals().at(0); // All normals are the same
                vert1 = glm::vec3(vert1.x - (thickness*normal.x), vert1.y - (thickness*normal.y), vert1.z - (thickness*normal.z));
                vert2 = glm::vec3(vert2.x - (thickness*normal.x), vert2.y - (thickness*normal.y), vert2.z - (thickness*normal.z));
                vert3 = glm::vec3(vert3.x - (thickness*normal.x), vert3.y - (thickness*normal.y), vert3.z - (thickness*normal.z));
                vert4 = glm::vec3(vert4.x - (thickness*normal.x), vert4.y - (thickness*normal.y), vert4.z - (thickness*normal.z));

                // Push outside shape
                side.push_back(vert4);
                side.push_back(vert3);
                side.push_back(vert2);
                side.push_back(vert1);
                this->shapes.push_back(new Shape(side, color));
            }
            // All other cases
            else{
                // Grab correct verts
                vert1 = verts[i];
                vert2 = verts[i+1];
                vert3 = glm::vec3(vert2.x, vert2.y + height, vert2.z);
                vert4 = glm::vec3(vert1.x, vert1.y + height, vert1.z);              

                // Push inside shape
                side.push_back(vert1);
                side.push_back(vert2);
                side.push_back(vert3);
                side.push_back(vert4);
                this->shapes.push_back(new Shape(side, color));

                side.clear();

                // Modify vert positions for next shape
                normal = shapes.back()->normals().at(0); // All normals are the same
                vert1 = glm::vec3(vert1.x - (thickness*normal.x), vert1.y - (thickness*normal.y), vert1.z - (thickness*normal.z));
                vert2 = glm::vec3(vert2.x - (thickness*normal.x), vert2.y - (thickness*normal.y), vert2.z - (thickness*normal.z));
                vert3 = glm::vec3(vert3.x - (thickness*normal.x), vert3.y - (thickness*normal.y), vert3.z - (thickness*normal.z));
                vert4 = glm::vec3(vert4.x - (thickness*normal.x), vert4.y - (thickness*normal.y), vert4.z - (thickness*normal.z));

                // Push outside shape
                side.push_back(vert4);
                side.push_back(vert3);
                side.push_back(vert2);
                side.push_back(vert1);
                this->shapes.push_back(new Shape(side, color));
            }
        }
    }

};
