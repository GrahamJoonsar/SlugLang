#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "slugInterpreter.h"

Interpreter slugInterp;

void proccessLine(std::string line){
    int amountOfTokens = 0;
    std::string args[10]; // Hopefully not more that 10 arguments
    std::vector<std::string> currentLineTokens = slugInterp.tokenizer(line);
    amountOfTokens = currentLineTokens.size();
    if (slugInterp.curlyBraceLevel[slugInterp.curlyBraceNum][0]){ // if statement succeded
        for (int i = 0; i < amountOfTokens; i++){ // looping through tokens
            if (slugInterp.inFunctions(currentLineTokens[i])){ // function was called
                if (slugInterp.argcountForFunc >= 0){ // not a variable argument amount
                    if (slugInterp.argcountForFunc != amountOfTokens - 1){ // Wrong amount of arguments
                        slugInterp.callError("Error: " + std::to_string(slugInterp.argcountForFunc) + 
                        " arguments were expected, but " + std::to_string(amountOfTokens - 1) + 
                        " were recieved.");
                    }
                    for (int j = 0; j < slugInterp.argcountForFunc; j++){
                        i++;
                        args[j] = currentLineTokens[i];
                    }
                    slugInterp.functions[slugInterp.funcNum].actualFunc(args, &slugInterp); // calling the function the user wants
                } else { // variable arg amount
                    slugInterp.argsPassedIn = amountOfTokens - 1;
                    if (slugInterp.argsPassedIn < -slugInterp.argcountForFunc){ // Not enough arguments
                        slugInterp.callError("Error: At least " +  std::to_string(-slugInterp.argcountForFunc) +
                        " arguments were expected, but " + std::to_string(slugInterp.argsPassedIn) + 
                        " were recieved.");
                    }
                    for (int j = 0; j < amountOfTokens - 1; j++){
                        i++;
                        args[j] = currentLineTokens[i];
                        // std::cout << '"' << currentLineTokens[i] << '"' << std::endl;
                    }
                    slugInterp.functions[slugInterp.funcNum].actualFunc(args, &slugInterp); // calling the function the user wants
                }
            }
        }
    }
}

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

    while (getline(inputFile, currentLine)){ // Looping through the file
        proccessLine(currentLine);
        slugInterp.lineNum++;
    }
    // Closing the file
    inputFile.close();
}
