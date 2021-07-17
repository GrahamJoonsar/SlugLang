#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "slugInterpreter.h"

Interpreter slugInterp;

extern float evalNum(std::string num, Interpreter * interp);
extern std::string getStrValOf(std::string val, Interpreter * interp);
extern void slugDelete(std::string * args, Interpreter * interp);

void proccessLine(std::string line){
    int amountOfTokens = 0;
    std::string args[32]; // Hopefully not more that 10 arguments
    std::vector<std::string> currentLineTokens = slugInterp.tokenizer(line);
    amountOfTokens = currentLineTokens.size();
    if (slugInterp.curlyBraceLevel[slugInterp.curlyBraceNum][0] && !slugInterp.isDefiningFunction){ // if statement succeded
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
                    }
                    slugInterp.functions[slugInterp.funcNum].actualFunc(args, &slugInterp); // calling the function the user wants
                }
            } else if (slugInterp.inUFunctions(currentLineTokens[i])){
                // Passing in parameters
                int trueFuncNum = slugInterp.funcNum;
                for (std::vector<std::string>::size_type i = 0; i < slugInterp.UFunctions[slugInterp.funcNum].params.size(); i++){
                    switch (slugInterp.UFunctions[slugInterp.funcNum].params[i][0]){
                        case 'i': // int
                            i++;
                            slugInterp.integers[slugInterp.UFunctions[slugInterp.funcNum].params[i]] = evalNum(currentLineTokens[1+(i/2)], &slugInterp);
                            break;
                        case 'f': // float
                            i++;
                            slugInterp.floats[slugInterp.UFunctions[slugInterp.funcNum].params[i]] = evalNum(currentLineTokens[1+(i/2)], &slugInterp);
                            break;
                        case 's': // string
                            i++;
                            slugInterp.strings[slugInterp.UFunctions[slugInterp.funcNum].params[i]] = getStrValOf(currentLineTokens[1+(i/2)], &slugInterp);
                            break;
                        case 'b': // bool (Must be stored in a var)
                            i++;
                            slugInterp.booleans[slugInterp.UFunctions[slugInterp.funcNum].params[i]] = slugInterp.booleans[currentLineTokens[1+(i/2)]];
                            break;
                    }
                }
                // Running the function
                for (auto uf : slugInterp.UFunctions[trueFuncNum].linesOfFunction){
                    proccessLine(uf);
                    if (slugInterp.isReturning){ // Stopping the function when it is returned from
                        slugInterp.isReturning = false;
                        break;
                    }
                }
                // Deleting the variables passed in
                slugInterp.argsPassedIn = 1; // To correct slugDelete
                for (std::vector<std::string>::size_type k = 1; k < slugInterp.UFunctions[trueFuncNum].params.size(); k += 2){
                    slugDelete(&slugInterp.UFunctions[trueFuncNum].params[k], &slugInterp);
                }
            }
        }
    } else if (slugInterp.isDefiningFunction){
        if (line == "end"){ // End function
            slugInterp.isDefiningFunction = false;
            return;
        }
        // Adding to the function and taking off the first 4 spaces
        slugInterp.UFunctions.back().linesOfFunction.push_back(line.substr(4, line.length() - 3));
    }
}

int main(int argc, char * argv[]){
    // Reading the command line arguments
    if (argc != 2){
        std::cout << "Invalid command line arguments" << std::endl;
        return 8; // error
    }

    std::ifstream inputFile(argv[1]); // Opening the file provided

    if (!inputFile.is_open()){
        std::cout << "File not found" << std::endl;
        return 8; // error
    }
    
    system("cls"); // Clear screen

    // Reading through the file line by line
    std::string currentLine;

    while (getline(inputFile, currentLine)){ // Looping through the file
        slugInterp.fullFile.push_back(currentLine);
    }

    for (slugInterp.lineNum = 0; slugInterp.lineNum < slugInterp.fullFile.size(); slugInterp.lineNum++){
        proccessLine(slugInterp.fullFile[slugInterp.lineNum]);
    }

    // Closing the file
    inputFile.close();
}
