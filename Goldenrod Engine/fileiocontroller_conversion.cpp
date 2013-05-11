#include "fileiocontroller_conversion.h"

vector<ProcessedInputLine> FileIOController::processFile(string fileName){

    ifstream inFile(fileName);
    string line;
    Tokenizer str;
    string keyword;
    vector<ProcessedInputLine> processedLines;

    // Check if file was opened correctly
    if(inFile.is_open()){
        cerr << endl << "Successfully opened " << fileName.c_str() << " for reading." << endl;

        // Loop to parse file contents
        while(inFile.good()){
            getline(inFile, line); // get next line of input file
            str.setString(line); // attach string to tokenizer

            // Determine what kind of object and add to level
            keyword = str.nextEcho(); // determine keyword
            if(keyword == "tile"){

                processedLines.push_back(ProcessedInputLine());
                processedLines.back().setTileData(str);

            }
            else if(keyword == "tee"){

                processedLines.push_back(ProcessedInputLine());
                processedLines.back().setTeeData(str);

            }
            else if(keyword == "cup"){

                processedLines.push_back(ProcessedInputLine());
                processedLines.back().setCupData(str);

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
	}

    return processedLines;

};
