#include "level_refactor.h"

// Initialize static levelCount
int Level::levelCount = 0;

// Constructors/Destructor
Level::Level(vector<ProcessedInputLine*> inLines, int ID){

    // Increment level count
    Level::levelCount++;

    // Set levelID
    this->levelID = ID;

    // Pull level information
    string keyword; // store current input line keyword
    for(int i = 0; i < inLines.size(); i++){
        // Create appropriate Entities and add to corresponding vectors
        keyword = inLines.at(i)->getKeyword();

        if(keyword == "tile"){ // Add tile
            this->addTile(inLines.at(i));
        }
        else if(keyword == "tee"){ // Add tee and ball
            this->addTee(inLines.at(i));

            this->addBall(inLines.at(i));
        }
        else if(keyword == "cup"){ // Add cup
            this->addCup(inLines.at(i));
        }
		else if(keyword == "course"){
			this->setCourseName(inLines.at(i)->getName());
		}
		else if(keyword == "name"){
			this->setLevelName(inLines.at(i)->getName());
		}
		else if(keyword == "par"){
			this->setPar(inLines.at(i)->getPar());
		}
        else{
            cerr << endl << "Invalid keyword in processed file." << endl;
        }
    }

	// Add all level shapes to levelShapes vector
	this->updateLevelShapes();

};

Level::~Level(){

    // Decrement Level Count
    Level::levelCount--;

    // Delete tiles
    for(int i = 0; i < this->tiles.size(); i++){
        delete this->tiles.at(i);
    }
    this->tiles.clear();

    // Delete cup, tee, and ball
    delete this->cup;
    delete this->tee;
    delete this->ball;

};

// Print info
void Level::printInfo(){

};

// Add/delete level elements
void Level::addTile(ProcessedInputLine* inLine){

    this->tiles.push_back(new Tile(inLine));

};

void Level::deleteTile(){

};

void Level::addCup(ProcessedInputLine* inLine){

    this->cup = new Cup(inLine);

};

void Level::deleteCup(){

};

void Level::addTee(ProcessedInputLine* inLine){

    this->tee = new Tee(inLine);

};

void Level::deleteTee(){

};

void Level::addBall(ProcessedInputLine* inLine){

    this->ball = new Ball(inLine);

};

void Level::deleteBall(){

};

void Level::setCourseName(string name){ this->courseName = name; };

void Level::setLevelName(string name){ this->levelName = name; };

void Level::setPar(int par){ this->par = par; };

// Access methods
int Level::getLevelCount(){ return Level::levelCount; };

int Level::getLevelID(){ return this->levelID; };

string Level::getCourseName(){ return this->courseName; };

string Level::getLevelName(){ return this->levelName; };

int Level::getPar(){ return this->par; };

vector<Tile*> Level::getTiles(){ return this->tiles; };

Tile* Level::getTile(int tileID){

    for(int i = 0; i < this->tiles.size(); i++){
        if(this->tiles.at(i)->getID() == tileID){
            return this->tiles.at(i);
        }
    }

    return NULL;

};

Cup* Level::getCup(){ return this->cup; };

Tee* Level::getTee(){ return this->tee; };

Ball* Level::getBall(){ return this->ball; };

vector<Shape*> Level::getLevelShapes(){ return this->levelShapes; };

// Update level shapes
void Level::updateLevelShapes(){

    // Reset shapes vector
    this->levelShapes.clear();

    // Fill shapes vector
    // Add tile and border shapes
    Tile* tmpTile;
    for(int i = 0; i < this->tiles.size(); i++){
        tmpTile = this->tiles.at(i);
        for(int ii = 0; ii < tmpTile->getShapes().size(); ii++){
            this->levelShapes.push_back(tmpTile->getShapes()[ii]);
        }
        if(TILE_USE_BORDER){
            for(int ii = 0; ii < tmpTile->getBorders()->getShapes().size(); ii++){
                this->levelShapes.push_back(tmpTile->getBorders()->getShapes()[ii]);
            }
        }
    }

    // Add cup shapes
    for(int i = 0; i < this->cup->getShapes().size(); i++){
        this->levelShapes.push_back(this->cup->getShapes().at(i));
    }

    // Add tee shapes
    for(int i = 0; i < this->cup->getShapes().size(); i++){
        this->levelShapes.push_back(this->tee->getShapes().at(i));
    }

    // Add ball shapes
    for(int i = 0; i < this->cup->getShapes().size(); i++){
        this->levelShapes.push_back(this->ball->getShapes().at(i));
    }

};
