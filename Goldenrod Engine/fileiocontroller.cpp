#include "fileiocontroller.h"

FileIOController::FileIOController(){

	this->currentHoleIndex = -1;
    this->currentHole.clear();
	this->parsedHoles.clear();

};

FileIOController::~FileIOController(){

    // Clear parsed Holes vector
    ProcessedInputLine* courseInfoPtr = this->currentHole.at(0); // Save pointer to single course info object
	for(int i = 0; i < this->parsedHoles.size(); i++){ // for each vector of parsed input lines
		for(int ii = 1; ii < this->parsedHoles.at(i).size(); ii++){ // for each input line
            if(this->parsedHoles.at(i).at(ii) != NULL){ // Make sure object exists
			    delete this->parsedHoles.at(i).at(ii); // delete the input line object
                this->parsedHoles.at(i).at(ii) = NULL;
            }
		}
		this->parsedHoles.at(i).clear(); // clear vector
    }
    this->parsedHoles.clear(); // clear vector

    // Delete course info object and pointer
    delete courseInfoPtr;

	// Clear current Hole vector
    this->currentHole.clear(); // delete pointers
};

int FileIOController::getNumHoles(){ return this->parsedHoles.size(); };

vector<ProcessedInputLine*> FileIOController::getCurrentHole(){ return this->currentHole; };

vector<ProcessedInputLine*> FileIOController::getHole(int index){ 
	
	if(index < this->parsedHoles.size() && index >= 0){
		return this->parsedHoles.at(index);
	}
	else{
		return vector<ProcessedInputLine*>();
	}

};

void FileIOController::processFile(string fileName){

    ifstream inFile(fileName);
    string line;
    Tokenizer str;
    string keyword;
    ProcessedInputLine* courseInfo;  

    // Check if file was opened correctly
    if(inFile.is_open()){
        cerr << endl << "Successfully opened " << fileName.c_str() << " for reading." << endl;

        // Loop to parse file contents
        while(inFile.good()){
            getline(inFile, line); // get next line of input file
            str.setString(line); // attach string to tokenizer

            // Determine keyword
            keyword = str.nextEcho();

            // If this is a new course
            if(keyword == "course"){
                // Create new course data object
                courseInfo = new ProcessedInputLine();
                courseInfo->setCourse(str);
            }
            else if(keyword == "begin_hole"){
                // Clear current hole
                this->currentHole.clear();
                // Increment index               
                this->currentHoleIndex++;
                // Push course info onto current hole
                this->currentHole.push_back(courseInfo);
            }
            else if(keyword == "end_hole"){
                // Push current hole to back of parsed holes
                this->parsedHoles.push_back(this->currentHole);                
            }
            else{ // Default reading of single Hole as in earlier version
                // Create input line and add to current Hole vector
			    createInputLine(keyword, str);
            }	
        }      
    }
    else{
		cerr << endl << "Could not open " << fileName.c_str() << " for reading." << endl;
	}

};

//void FileIOController::processFile(string fileName){
//
//    ifstream inFile(fileName);
//    string line;
//    Tokenizer str;
//    string keyword;
//
//    
//
//    // Check if file was opened correctly
//    if(inFile.is_open()){
//        cerr << endl << "Successfully opened " << fileName.c_str() << " for reading." << endl;
//
//        // Loop to parse file contents
//        while(inFile.good()){
//            getline(inFile, line); // get next line of input file
//            str.setString(line); // attach string to tokenizer
//
//            // Determine what kind of object and add to Hole
//            keyword = str.nextEcho(); // determine keyword
//            if(keyword == "tile"){
//
//                this->currentFile.push_back(new ProcessedInputLine());
//                this->currentFile.back()->setTileData(str);
//
//            }
//            else if(keyword == "tee"){
//
//                this->currentFile.push_back(new ProcessedInputLine());
//                this->currentFile.back()->setTeeData(str);
//
//            }
//            else if(keyword == "cup"){
//
//                this->currentFile.push_back(new ProcessedInputLine());
//                this->currentFile.back()->setCupData(str);
//
//            }
//            else if(keyword == ""){
//
//                cout << "End of file." << endl << endl;
//
//            }
//            else{
//
//                cout << "Invalid keyword." << endl;
//
//            }
//        }      
//    }
//    else{
//		cerr << endl << "Could not open " << fileName.c_str() << " for reading." << endl;
//        // Bad filename, clear currentFile
//        this->currentFile.clear();
//	}
//
//};

bool FileIOController::nextHole(){

	if(this->currentHoleIndex < (this->parsedHoles.size() - 1)){
		this->currentHoleIndex++;
        this->currentHole = this->parsedHoles.at(this->currentHoleIndex);
        return true;
	}
	else{
		return false;
	}

};

bool FileIOController::prevHole(){

    if(this->currentHoleIndex <= 0){
        return false;
    }
    else{
        this->currentHoleIndex--;
        this->currentHole = this->parsedHoles.at(this->currentHoleIndex);
        return true;
    }

}

bool FileIOController::goToHole(int index){

    if(index >= 0 && index < this->parsedHoles.size()){
        this->currentHoleIndex = index;
        this->currentHole = this->parsedHoles.at(this->currentHoleIndex);
        return true;
    }
    else{
        return false;
    }

}

int FileIOController::getCurrentHoleID(){ return this->currentHoleIndex; };

// Priavte methods
void FileIOController::createInputLine(string keyword, Tokenizer& str){

	if(keyword == "name"){

		this->currentHole.push_back(new ProcessedInputLine());
		this->currentHole.back()->setHoleName(str);

	}
	else if(keyword == "par"){

		this->currentHole.push_back(new ProcessedInputLine());
		this->currentHole.back()->setHolePar(str);

	}
    else if(keyword == "tile"){

        this->currentHole.push_back(new ProcessedInputLine());
        this->currentHole.back()->setTileData(str);

    }
    else if(keyword == "tee"){

        this->currentHole.push_back(new ProcessedInputLine());
        this->currentHole.back()->setTeeData(str);

    }
    else if(keyword == "cup"){

        this->currentHole.push_back(new ProcessedInputLine());
        this->currentHole.back()->setCupData(str);

    }
    else if(keyword == ""){

        cout << "End of file." << endl << endl;

    }
    else{

        cout << "Invalid keyword." << endl;

    }

};