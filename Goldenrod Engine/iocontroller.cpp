#include "iocontroller.h"

IOController::IOController(){

	this->gameIO = new GameIOController();
	this->fileIO = new FileIOController();

};

IOController::~IOController(){

	delete this->gameIO;
	delete this->fileIO;

};
