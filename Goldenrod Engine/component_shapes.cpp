#include "component_shapes.h"

Shapes::Shapes(){

    this->name = "NONE";
    this->type = cSHAPES_T;

};

Shapes::~Shapes(){

    this->shapes.clear();

};

void Shapes::update(){

};

void Shapes::printInfo(){

    this->Component::printInfo();

    // Print info specific to Shapes

};

void Shapes::addWedgeShapes(vector<glm::vec3> verts, glm::vec4 color, float depth){
    
    generateWedgeShapes(this->shapes, verts, color, depth);

};

void Shapes::addBorderShapes(vector<glm::vec3> verts, glm::vec4 color, vector<int> neighborIDs, float height, float thickness){
    
    generateBorderShapes(this->shapes, verts, color, neighborIDs, height, thickness);

};

vector<Shape*> Shapes::getShapes(){ // REMOVE THIS AFTER CONVERT TO USING VBOs
    return this->shapes;
};

void Shapes::generateWedgeShapes(vector<Shape*>& shapes, vector<glm::vec3> verts, glm::vec4 color, float depth){

    // Ensure depth >= 0
    depth = abs(depth);

    // Separate out new vectors with verts for each face in correct order for shape generation
    // Top and bottom
    vector<glm::vec3> top = verts;

    // Check if need to calculate additional verts
    if(depth > 0){
        vector<glm::vec3> bottom;
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

        shapes.push_back(new Shape(top, color));
        shapes.push_back(new Shape(bottom, color));
        for(it = sides.begin(); it != sides.end(); it+=4){
            tmpVec.clear();
            tmpVec.insert(tmpVec.begin(), it, (it +4));
            shapes.push_back(new Shape(tmpVec, color));
        }

    }
    else{
        shapes.push_back(new Shape(top, color));
    }

};

void Shapes::generateBorderShapes(vector<Shape*>& shapes, vector<glm::vec3> verts, glm::vec4 color, vector<int> neighborIDs, float height, float thickness){
   
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
                shapes.push_back(new Shape(side, color));

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
                shapes.push_back(new Shape(side, color));
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
                shapes.push_back(new Shape(side, color));

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
                shapes.push_back(new Shape(side, color));
            }
        }
    }

};
