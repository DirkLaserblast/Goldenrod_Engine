#include "level.h"

int Level::levelCount = 0;

Level::Level(){

    // Default values
    this->name = "NONE";
    this->type = "LEVEL_T";
    this->ID = (Level::levelCount)++;

    this->currentLevelShapes = new vector<Shape> (); // REMOVE THIS AFTER CONVERT TO USING VBOs

};

Level::Level(vector<ProcessedInputLine*>* inLines){

    // Same as default
    this->name = "NONE";
    this->type = "LEVEL_T";
    this->ID = (Level::levelCount)++;

    this->currentLevelShapes = new vector<Shape> (); // REMOVE THIS AFTER CONVERT TO USING VBOs

    string keyword;

    for(int i = 0; i < inLines->size(); i++){
        // Create appropriate Entities and add to corresponding vectors
        keyword = (*inLines)[i]->getKeyword();
        if(keyword == "tile"){
            this->addTile(*((*inLines)[i]));
        }
        else if(keyword == "tee"){
            this->addTee(*((*inLines)[i]));
            this->addBall(*((*inLines)[i]));
        }
        else if(keyword == "cup"){
            this->addCup(*((*inLines)[i]));
        }
        else{
            cerr << endl << "Invalid keyword in processed file." << endl;
        }
    }

};

Level::~Level(){

    this->currentLevelShapes->clear(); // REMOVE THIS AFTER CONVERT TO USING VBOs
    delete this->currentLevelShapes; // REMOVE THIS AFTER CONVERT TO USING VBOs

};

void Level::update(){

};

void Level::printInfo(){

    // Print level name
    cerr << endl << "LEVEL NAME: " << this->name << endl;
    // Print level type
    cerr << "TYPE: " << this->type << endl;
    // Print ID
    cerr << "ID: " << this->ID << endl;


};

void Level::addTile(ProcessedInputLine& inLine){
    
    // New entity
    Entity* newTile = new Entity(eSTATIC_GEOMETRY_T);

    // Attach components
    newTile->attachComponent(new Tile(inLine.getID(), inLine.getNeighborIDs()));

    // Setup public components since real components not working yet
    newTile->publicShapes = new Shapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs
    newTile->publicShapes->addWedgeShapes(inLine.getVerts(), TILE_COLOR, TILE_DEPTH);
    newTile->publicPhysics = new Physc();
    newTile->publicPhysics->setPosition(calculateCenter(inLine.getVerts()));

    // Add corresponding border entity
    if(TILE_USE_BORDER){
        // New entity
        Entity* newBorder = new Entity(eSTATIC_GEOMETRY_T);

        // Attach components
        newBorder->attachComponent(new Border(inLine.getID()));

        // Setup public components since real components not working yet
        newBorder->publicShapes = new Shapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs
        newBorder->publicShapes->addBorderShapes(inLine.getVerts(), BORDER_COLOR, inLine.getNeighborIDs(), BORDER_HEIGHT, BORDER_THICKNESS);

        newBorder->publicPhysics = new Physc();

        /*newBorder->publicCollision = new Collision();
        newBorder->publicCollision->addBorderCollisionShapes(inLine.getVerts(), COLLISION_SHAPE_COLOR, inLine.getNeighborIDs(), BORDER_HEIGHT, BORDER_THICKNESS);*/

        this->borders.push_back(newBorder);
    }

    this->tiles.push_back(newTile);

};

void Level::deleteTile(){
    
};

void Level::addCup(ProcessedInputLine& inLine){
    
    // New entity
    Entity* newCup = new Entity(eSTATIC_GEOMETRY_T);

    // Attach components
    newCup->attachComponent(new Cup(inLine.getID()));

    // Setup public components since real components not working yet
    vector<glm::vec3> cupVerts = squareFromPoint(inLine.getVerts().at(0),CUP_WIDTH,CUP_HEIGHT,CUP_OFFSET);   
    newCup->publicShapes = new Shapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs
    newCup->publicShapes->addWedgeShapes(cupVerts, CUP_COLOR, CUP_DEPTH);

    newCup->publicPhysics = new Physc();
    newCup->publicPhysics->setPosition(inLine.getVerts().at(0));

    /*newCup->publicCollision = new Collision();
    newCup->publicCollision->addWedgeCollisionShapes(cupVerts, COLLISION_SHAPE_COLOR, CUP_DEPTH);*/

    this->cup = newCup;

};

void Level::deleteCup(){
    
};

void Level::addTee(ProcessedInputLine& inLine){
    
    // New entity
    Entity* newTee = new Entity(eSTATIC_GEOMETRY_T);

    // Attach components
    newTee->attachComponent(new Tee(inLine.getID()));

    // Setup public components since real components not working yet
    vector<glm::vec3> teeVerts = squareFromPoint(inLine.getVerts().at(0),TEE_WIDTH,TEE_HEIGHT,TEE_OFFSET);  
    newTee->publicShapes = new Shapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs
    newTee->publicShapes->addWedgeShapes(teeVerts, TEE_COLOR, TEE_DEPTH);

    newTee->publicPhysics = new Physc();
    newTee->publicPhysics->setPosition(inLine.getVerts().at(0));

    /*newTee->publicCollision = new Collision();
    newTee->publicCollision->addWedgeCollisionShapes(teeVerts, COLLISION_SHAPE_COLOR, TEE_DEPTH);*/

    this->tee = newTee;

};

void deleteTee(){
    
};

void Level::addBall(ProcessedInputLine& inLine){

    // New entity
    Entity* newBall = new Entity(eDYNAMIC_GEOMETRY_T);

    // Attach components
    newBall->attachComponent(new Ball(inLine.getID()));

    // Setup public components since real components not working yet
    vector<glm::vec3> ballVerts = circleFromPoint(inLine.getVerts().at(0),BALL_RADIUS, BALL_DEGREE, BALL_OFFSET);  
    newBall->publicShapes = new Shapes(); // REMOVE THIS AFTER CONVERT TO USING VBOs
    newBall->publicShapes->addWedgeShapes(ballVerts, BALL_COLOR, BALL_DEPTH);

    newBall->publicPhysics = new Physc();
    newBall->publicPhysics->setPosition(inLine.getVerts().at(0));

    /*newBall->publicCollision = new Collision();
    newBall->publicCollision->addBallCollisionShapes(ballVerts, COLLISION_SHAPE_COLOR);*/

    this->ball = newBall;

};

void Level::deleteBall(){

};

int Level::getLevelCount(){ return this->levelCount; };

vector<Shape>* Level::getCurrentLevelShapes(){ // REMOVE THIS AFTER CONVERT TO USING VBOs

    return this->currentLevelShapes;

};

Entity* Level::getCup(){ return this->cup; };

Entity* Level::getTee(){ return this->tee; };

Entity* Level::getBall(){ return this->ball; };

void Level::updateCurrentLevelShapes(){ // REMOVE THIS AFTER CONVERT TO USING VBOs

    if(this->currentLevelShapes->size() != 0){
        this->currentLevelShapes->clear();
    }

    vector<Shape*> tmp;

    // Get tile shapes
    for(int i = 0; i < this->tiles.size(); i++){
        tmp = this->tiles.at(i)->publicShapes->getShapes();
        for(int ii = 0; ii < tmp.size(); ii++){ // Add (copy of?) each shape
            this->currentLevelShapes->push_back(*(tmp[ii]));
        }
    }

    // Get border shapes
    for(int i = 0; i < this->borders.size(); i++){
        tmp = this->borders.at(i)->publicShapes->getShapes();
        for(int ii = 0; ii < tmp.size(); ii++){ // Add (copy of?) each shape
            this->currentLevelShapes->push_back(*(tmp[ii]));
        }
    }

    // Get tee shapes
    tmp = this->tee->publicShapes->getShapes();
    for(int i = 0; i < tmp.size(); i++){
        this->currentLevelShapes->push_back(*(tmp[i]));
    }

    // Get cup shapes
    tmp = this->cup->publicShapes->getShapes();
    for(int i = 0; i < tmp.size(); i++){
        this->currentLevelShapes->push_back(*(tmp[i]));
    }

    // Get ball shapes
    tmp = this->ball->publicShapes->getShapes();
    for(int i = 0; i < tmp.size(); i++){
        this->currentLevelShapes->push_back(*(tmp[i]));
    }

};

vector<glm::vec3> Level::squareFromPoint(glm::vec3 point, float width, float height, float offset){
    
    vector<glm::vec3> squareVerts;

    squareVerts.push_back(glm::vec3(point.x - width, point.y + offset, point.z + height));
    squareVerts.push_back(glm::vec3(point.x + width, point.y + offset, point.z + height));
    squareVerts.push_back(glm::vec3(point.x + width, point.y + offset, point.z - height));
    squareVerts.push_back(glm::vec3(point.x - width, point.y + offset, point.z - height));

    return squareVerts;

};

vector<glm::vec3> Level::circleFromPoint(glm::vec3 point, float radius, float degree, float offset){

    vector<glm::vec3> circleVerts;
    float newX,
          newY,
          newZ;

    // Add verts for circle
    //for(float i = 0; i <= 360; i += degree){
    //    newX = (radius*sin(i)) + point.x;
    //    newY = point.y + offset;
    //    newZ = (radius*cos(i)) + point.z;

    //    circleVerts.push_back(glm::vec3(newX,newY,newZ));
    //}
    circleVerts.push_back(glm::vec3(point.x, point.y + offset, point.z + radius));
    circleVerts.push_back(glm::vec3(point.x + 0.7*radius, point.y + offset, point.z + 0.7*radius));
    circleVerts.push_back(glm::vec3(point.x + radius, point.y + offset, point.z));
    circleVerts.push_back(glm::vec3(point.x + 0.7*radius, point.y + offset, point.z - 0.7*radius));
    circleVerts.push_back(glm::vec3(point.x, point.y + offset, point.z - radius));
    circleVerts.push_back(glm::vec3(point.x - 0.7*radius, point.y + offset, point.z - 0.7*radius));
    circleVerts.push_back(glm::vec3(point.x - radius, point.y + offset, point.z));
    circleVerts.push_back(glm::vec3(point.x - 0.7*radius, point.y + offset, point.z + 0.7*radius));

    return circleVerts;

};

glm::vec3 Level::calculateCenter(vector<glm::vec3> plane){

    glm::vec3 center = glm::vec3();

    for(int i = 0; i < plane.size(); i++){
        center += plane[i];
    }

    center = glm::vec3(center.x/plane.size(), center.y/plane.size(), center.z/plane.size());

    return center;

};
