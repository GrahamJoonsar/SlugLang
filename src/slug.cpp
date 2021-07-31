#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "slugInterpreter.h"

Interpreter slugInterp;

bool inFunc = false;
int baseFuncNum;

extern float evalNum(std::string num, Interpreter * interp);
extern std::string getStrValOf(std::string val, Interpreter * interp);
extern bool getBooleanValOf(std::string * args, Interpreter * interp, int argc);

extern void proccessLine(std::string line){
    int amountOfTokens = 0;
    std::string args[32]; // Hopefully not more that 10 arguments
    std::vector<std::string> currentLineTokens = slugInterp.tokenizer(line);
    amountOfTokens = currentLineTokens.size();
    if (slugInterp.curlyBraceLevel[slugInterp.curlyBraceNum][0] && !slugInterp.isDefiningFunction && !slugInterp.definingLoop){ // if statement succeded
        for (int i = 0; i < amountOfTokens; i++){ // looping through tokens
            if (slugInterp.inFunctions(currentLineTokens[i])){ // function was called
                if (slugInterp.argcountForFunc >= 0){ // not a variable argument amount
                    if (slugInterp.argcountForFunc != amountOfTokens - 1 && currentLineTokens[i] != "into"){ // Wrong amount of arguments
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
                bool goingInto = false;
                // Storing the variables so the function does not affect the outside
                slugInterp.vstack.push({slugInterp.integers, slugInterp.floats, slugInterp.strings, slugInterp.booleans});

                // Storing the global variables
                if (slugInterp.vstack.length() == 1){ // first layer
                    slugInterp.int_temp = slugInterp.integers;
                    slugInterp.float_temp = slugInterp.floats;
                    slugInterp.string_temp = slugInterp.strings;
                    slugInterp.bool_temp = slugInterp.booleans;
                }
                // Including the global variables
                slugInterp.integers = slugInterp.int_temp;
                slugInterp.floats = slugInterp.float_temp;
                slugInterp.strings = slugInterp.string_temp;
                slugInterp.booleans = slugInterp.bool_temp;

                // Checking if the correct amount of args is passed in
                if (amountOfTokens - 1 > slugInterp.UFunctions[slugInterp.funcNum].argc){
                    if (currentLineTokens[slugInterp.UFunctions[slugInterp.funcNum].argc + 1] == "into"){
                        goingInto = true;
                    }
                }
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

                if (goingInto){
                    goingInto = false;
                    switch(slugInterp.rt){
                        case INT:
                            slugInterp.integers[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.i;
                            break;
                        case FLOAT:
                            slugInterp.floats[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.f;
                            break;
                        case STRING:
                            slugInterp.strings[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.s;
                            break;
                        case BOOL:
                            slugInterp.booleans[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.b;
                            break;
                    }
                }

                // So the mutate keyword works
                slugInterp.integers = slugInterp.int_temp;
                slugInterp.floats = slugInterp.float_temp;
                slugInterp.strings = slugInterp.string_temp;
                slugInterp.booleans = slugInterp.bool_temp;
            }
        }
    } else if (slugInterp.isDefiningFunction){
        if (line == "end"){ // End function
            slugInterp.isDefiningFunction = false;
            return;
        }
        // Adding to the function and taking off the first 4 spaces
        if (line.length() > 4){
            slugInterp.UFunctions.back().linesOfFunction.push_back(line.substr(4, line.length() - 3));
        }
    } else if (slugInterp.definingLoop){ // Collecting and executing lines for loops
        if (line.length() >= slugInterp.wstack.back().tabs.length()){
            if (line != slugInterp.wstack.back().tabs + "endw"){ // Getting rid of spaces relative to tab level
                line.erase(0, slugInterp.wstack.back().tabs.length()+4);
                slugInterp.wstack.addl(line);
            } else {
                slugInterp.definingLoop = false;
                while(getBooleanValOf(slugInterp.wstack.back().booleanExpression, &slugInterp, slugInterp.wstack.back().length)){
                    for (auto l : slugInterp.wstack.back().linesOfLoop){
                        proccessLine(l);
                    }
                }
                slugInterp.wstack.pop_back();
            }
        }
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
