#include "fileiocontroller_conversion.h"

FileIOController::FileIOController(){

    this->currentFile = NULL;

};

FileIOController::~FileIOController(){

    // Clear vectors
    this->currentFile->clear(); // Not sure if this is correct way to do this...

    // Set pointers to NULL
    this->currentFile = NULL;

};

void FileIOController::processFile(string fileName){

    ifstream inFile(fileName);
    string line;
    Tokenizer str;
    string keyword;

    

    // Check if file was opened correctly
    if(inFile.is_open()){
        cerr << endl << "Successfully opened " << fileName.c_str() << " for reading." << endl;

        // Create new file vector and set to currentFile
        this->currentFile = new vector<ProcessedInputLine*> ();

        // Loop to parse file contents
        while(inFile.good()){
            getline(inFile, line); // get next line of input file
            str.setString(line); // attach string to tokenizer

            // Determine what kind of object and add to level
            keyword = str.nextEcho(); // determine keyword
            if(keyword == "tile"){

                this->currentFile->push_back(new ProcessedInputLine());
                this->currentFile->back()->setTileData(str);

            }
            else if(keyword == "tee"){

                this->currentFile->push_back(new ProcessedInputLine());
                this->currentFile->back()->setTeeData(str);

            }
            else if(keyword == "cup"){

                this->currentFile->push_back(new ProcessedInputLine());
                this->currentFile->back()->setCupData(str);

            }
            else if(keyword == ""){

                cout << "End of file." << endl << endl;

            }
            else{

                cout << "Invalid keyword." << endl;

            }
        }      
    }
    else{
		cerr << endl << "Could not open " << fileName.c_str() << " for reading." << endl;
        // Bad filename, set currentFile to NULL
        this->currentFile = NULL;
	}

};

vector<ProcessedInputLine*>* FileIOController::getCurrentFile(){ return this->currentFile; };
