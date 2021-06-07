#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "slugInterpreter.h"

Interpreter slugInterp;

int main(int argc, char * argv[]){
    // Reading the command line arguments
    if (argc == 2){
        std::cout << "Starting interpretation" << std::endl;
    } else {
        std::cout << "Invalid command line arguments" << std::endl;
        return 8; // error
    }

    std::ifstream inputFile(argv[1]); // Opening the file provided

    if (inputFile.is_open()){
        std::cout << "File opened" << std::endl;
    } else {
        std::cout << "File not found" << std::endl;
        return 8; // error
    }

    std::cout << "Ready to start" << std::endl;
    system("pause"); // waiting for the user to press enter

    system("cls"); // Clear screen

    // Reading through the file line by line
    std::string currentLine;
    std::vector<std::string> currentLineTokens;
    std::string args[10]; // Hopefully not more that 10 arguments

    while (getline(inputFile, currentLine)){
        currentLineTokens = slugInterp.tokenizer(currentLine);
        for (int i = 0; i < currentLineTokens.size(); i++){ // looping through tokens
            if (slugInterp.inFunctions(currentLineTokens[i])){ // function was called
                for (int j = 0; j < slugInterp.argcountForFunc; j++){
                    i++;
                    args[j] = currentLineTokens[i];
                }
                slugInterp.functions[slugInterp.funcNum].actualFunc(args, &slugInterp);
            }
        }
    }

    // Closing the file
    inputFile.close();
}
