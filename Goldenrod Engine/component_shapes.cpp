#include "component_shapes.h"

Shapes::Shapes(){

    this->name = "NONE";
    this->type = cSHAPES_T;

};

Shapes::Shapes(vector<glm::vec3> verts, glm::vec4 color, float depth){

    this->name = "NONE";
    this->type = cSHAPES_T;
    this->generateTileShapes(this->shapes, verts, color, depth);

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

vector<Shape*> Shapes::getShapes(){ return this->shapes; }; // REMOVE THIS AFTER CONVERT TO USING VBOs

void Shapes::generateTileShapes(vector<Shape*>& shapes, vector<glm::vec3> verts, glm::vec4 color, float depth){

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
