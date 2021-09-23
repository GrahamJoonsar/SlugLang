#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include "slugInterpreter.h"
#include "slugCompilation.h"

Interpreter slugInterp;

extern float evalNum(std::string num, Interpreter * interp);
extern std::string getStrValOf(std::string val, Interpreter * interp);
extern bool getBooleanValOf(std::string * args, Interpreter * interp, int argc);

bool notEndOfLoopLine(int lengthOfTabs, std::string line){
    int firstCharSeen = -1;
    for (int i = 0; i < line.length(); i++){
        if (line[i] == ' ') continue;
        firstCharSeen = i;
        break;
    }
    return firstCharSeen >= lengthOfTabs || firstCharSeen == -1;
}

extern void proccessLine(std::string line){
    slugInterp.currentLineBeingProcessed = line;
    auto argsPITemp = slugInterp.argsPassedIn;
    auto cbnTemp = slugInterp.curlyBraceNum;
    int amountOfTokens = 0;
    std::string args[32]; // Hopefully not more that 10 arguments
    std::vector<std::string> currentLineTokens = slugInterp.tokenizer(line);
    amountOfTokens = currentLineTokens.size();
    if (slugInterp.curlyBraceLevel[slugInterp.curlyBraceNum][0] && !slugInterp.isDefiningFunction && !slugInterp.definingLoop){ // if statement succeded
        for (int i = 0; i < amountOfTokens; i++){ // looping through tokens
            if (slugInterp.inFunctions(currentLineTokens[i])){ // function was called
                auto currentFunction = slugInterp.functions[currentLineTokens[i]];
                if (currentFunction.argc >= 0){ // not a variable argument amount
                    bool goingInto = false;
                    if (currentFunction.argc != amountOfTokens - 1){ // Wrong amount of arguments
                        if (amountOfTokens == currentFunction.argc+3){
                            if (currentLineTokens[currentFunction.argc + 1] == "into"){
                                goingInto = true;
                            } else {
                                slugInterp.callError("Error: " + std::to_string(currentFunction.argc) + 
                                " arguments were expected, but " + std::to_string(amountOfTokens - 1) + 
                                " were recieved.");
                            }
                        } else {
                            slugInterp.callError("Error: " + std::to_string(currentFunction.argc) + 
                            " arguments were expected, but " + std::to_string(amountOfTokens - 1) + 
                            " were recieved.");
                        }
                    }
                    for (int j = 0; j < currentFunction.argc; j++){
                        i++;
                        args[j] = currentLineTokens[i];
                    }
                    currentFunction.actualFunc(args, &slugInterp); // calling the function the user wants
                    if (goingInto){
                        goingInto = false;
                        switch(slugInterp.rt){
                            case RETURN_ENUM::INT:
                                slugInterp.integers[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.i;
                                break;
                            case RETURN_ENUM::FLOAT:
                                slugInterp.floats[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.f;
                                break;
                            case RETURN_ENUM::STRING:
                                slugInterp.strings[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.s;
                                break;
                            case RETURN_ENUM::BOOL:
                                slugInterp.booleans[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.b;
                                break;
                        }
                    }
                } else { // variable arg amount
                    slugInterp.argsPassedIn = amountOfTokens - 1; // Shouldn't use into for var args
                    if (slugInterp.argsPassedIn < -currentFunction.argc){ // Not enough arguments
                        slugInterp.callError("Error: At least " +  std::to_string(-currentFunction.argc) +
                        " arguments were expected, but " + std::to_string(slugInterp.argsPassedIn) + 
                        " were recieved.");
                    }
                    for (int j = 0; j < amountOfTokens - 1; j++){
                        i++;
                        args[j] = currentLineTokens[i];
                    }
                    currentFunction.actualFunc(args, &slugInterp); // calling the function the user wants
                }
            } else if (slugInterp.inUFunctions(currentLineTokens[i])){
                bool goingInto = false;
                auto currentUFunction = slugInterp.UFunctions[currentLineTokens[i]];
                // Storing the variables so the function does not affect the outside
                slugInterp.vstack.push({slugInterp.integers, slugInterp.floats, slugInterp.strings, slugInterp.booleans});

                // Storing the global variables
                if (slugInterp.vstack.length() == 1){ // first layer
                    slugInterp.int_temp = slugInterp.integers;
                    slugInterp.float_temp = slugInterp.floats;
                    slugInterp.string_temp = slugInterp.strings;
                    slugInterp.bool_temp = slugInterp.booleans;
                }

                // Checking if the correct amount of args is passed in
                if (amountOfTokens - 1 > currentUFunction.argc){
                    if (currentLineTokens[currentUFunction.argc + 1] == "into"){
                        goingInto = true;
                    }
                }
                // Passing in parameters
                for (std::vector<std::string>::size_type i = 0; i < currentUFunction.params.size(); i++){
                    switch (currentUFunction.params[i][0]){
                        case 'i': // int
                            i++;
                            slugInterp.integers[currentUFunction.params[i]] = evalNum(currentLineTokens[1+(i/2)], &slugInterp);
                            break;
                        case 'f': // float
                            i++;
                            slugInterp.floats[currentUFunction.params[i]] = evalNum(currentLineTokens[1+(i/2)], &slugInterp);
                            break;
                        case 's': // string
                            i++;
                            slugInterp.strings[currentUFunction.params[i]] = getStrValOf(currentLineTokens[1+(i/2)], &slugInterp);
                            break;
                        case 'b': // bool (Must be stored in a var)
                            i++;
                            slugInterp.booleans[currentUFunction.params[i]] = slugInterp.booleans[currentLineTokens[1+(i/2)]];
                            break;
                    }
                }
                // Running the function
                for (auto uf : currentUFunction.linesOfFunction){
                    proccessLine(uf);
                    if (slugInterp.isReturning){ // Stopping the function when it is returned from
                        slugInterp.isReturning = false;
                        slugInterp.breakingLoop = false;
                        break;
                    }
                }

                if (goingInto){
                    goingInto = false;
                    switch(slugInterp.rt){
                        case RETURN_ENUM::INT:
                            slugInterp.int_temp[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.i;
                            break;
                        case RETURN_ENUM::FLOAT:
                            slugInterp.float_temp[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.f;
                            break;
                        case RETURN_ENUM::STRING:
                            slugInterp.string_temp[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.s;
                            break;
                        case RETURN_ENUM::BOOL:
                            slugInterp.bool_temp[currentLineTokens[amountOfTokens-1]] = slugInterp.returnedVal.b;
                            break;
                    }
                }

                // So the mutate keyword works
                if (slugInterp.vstack.length() == 1){ // Heading into global scope
                    slugInterp.integers = slugInterp.int_temp;
                    slugInterp.floats = slugInterp.float_temp;
                    slugInterp.strings = slugInterp.string_temp;
                    slugInterp.booleans = slugInterp.bool_temp;
                    slugInterp.vstack.pop_back();
                } else {
                    slugInterp.vstack.pop_back();
                    auto itemp = slugInterp.int_temp;
                    auto ftemp = slugInterp.float_temp;
                    auto stemp = slugInterp.string_temp;
                    auto btemp = slugInterp.bool_temp;
                    itemp.insert(slugInterp.integers.begin(), slugInterp.integers.end());
                    slugInterp.integers = itemp;
                    ftemp.insert(slugInterp.floats.begin(), slugInterp.floats.end());
                    slugInterp.floats = ftemp;
                    stemp.insert(slugInterp.strings.begin(), slugInterp.strings.end());
                    slugInterp.strings = stemp;
                    btemp.insert(slugInterp.booleans.begin(), slugInterp.booleans.end());
                    slugInterp.booleans = btemp;
                }


                

                for (int i = 1; i < 10; i++){
                    slugInterp.curlyBraceLevel[i][0] = true;
                    slugInterp.curlyBraceLevel[i][1] = true;
                }
                break;
            }
        }
    } else if (slugInterp.isDefiningFunction){
        if (line.length() >= 3){
            if (line.substr(0, 3) == "end"){ // End function
                slugInterp.isDefiningFunction = false;
                return;
            }
        }
        // Adding to the function and taking off the first 4 spaces
        if (line.length() > 4){
            slugInterp.UFunctions[slugInterp.currentFunctionName].linesOfFunction.push_back(line.substr(4, line.length() - 3));
        }
    } else if (slugInterp.definingLoop){ // Collecting and executing lines for loops
        if (line.length() >= slugInterp.wstack.back().tabs.length()){
            if (line != slugInterp.wstack.back().tabs + "endw"){ // Getting rid of spaces relative to tab level
                line.erase(0, slugInterp.wstack.back().tabs.length()+4);
                slugInterp.wstack.addl(line);
            } else {
                slugInterp.definingLoop = false;
                while(getBooleanValOf(slugInterp.wstack.back().booleanExpression, &slugInterp, slugInterp.wstack.back().length) && !slugInterp.breakingLoop){
                    for (auto l : slugInterp.wstack.back().linesOfLoop){
                        proccessLine(l);
                        if (slugInterp.breakingLoop){
                            break;
                        }
                    }
                    if (slugInterp.wstack.back().isForLoop){
                        proccessLine(slugInterp.wstack.back().action);
                    }
                }
                slugInterp.wstack.pop_back();
                slugInterp.breakingLoop = false;
            }
        }
    }
    slugInterp.argsPassedIn = argsPITemp;
    slugInterp.curlyBraceNum = cbnTemp;
}

// Turning slug programs to c++
std::string slugToCpp(std::vector<std::string> tokens){
    if (tokens.size() == 0) return "";
    if (compedLines.find(tokens[0]) != compedLines.end()){
        return compedLines[tokens[0]](tokens, &slugInterp);
    } else {
        std::cout << "Unrecognized command for compilation." << std::endl;
        std::cout << "'" << tokens[0] << "' is not recognized." << std::endl;
        exit(EXIT_SUCCESS);
        return "";
    }
}

int main(int argc, char * argv[]){
    // Reading the command line arguments
    if (argc != 3){
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

    slugInterp.fullFile.push_back("###EOFSLUG###");

    // Closing the file
    inputFile.close();

    if (argv[2][0] == 'i'){ // Interpreted
        for (slugInterp.lineNum = 0; slugInterp.lineNum < slugInterp.fullFile.size(); slugInterp.lineNum++){
            proccessLine(slugInterp.fullFile[slugInterp.lineNum]);
        }
    } else if (argv[2][0] == 'c'){ // Compiled
        initCompilation(); // Initializing all of the slug to cpp commands
        argv[1][strlen(argv[1])-5] = '\0'; // taking off the .slug postfix
        const char * outPutFileName = argv[1]; // name of file
        std::ofstream cppSourceFile; // The output file
        cppSourceFile.open(strcat(argv[1], ".cpp"), std::ios_base::trunc); // Opening the file and erasing everything in it
        // Adding libraries
        cppSourceFile << "#include <iostream>\n"; 
        cppSourceFile << "#include <string>\n";
        cppSourceFile << "#include <cmath>\n";
        // Main function
        cppSourceFile << "int main(void){\n";
        for (slugInterp.lineNum = 0; slugInterp.lineNum < slugInterp.fullFile.size(); slugInterp.lineNum++){
            cppSourceFile << slugToCpp(slugInterp.tokenizer(slugInterp.fullFile[slugInterp.lineNum])) << '\n';
        }
        // End of main function
        cppSourceFile << "}\n";
        // Closing the file
        cppSourceFile.close();
        // Making the compiler command
        char command[256] = "g++ ";
        strcat(command, argv[1]);
        // Taking off .cpp postfix
        argv[1][strlen(argv[1])-4] = '\0';
        strcat(command, " -o ");
        strcat(command, argv[1]);
        strcat(command, ".exe");
        // Compiling
        system(command);
        // Removing cpp src file
        strcat(argv[1], ".cpp");
        remove(argv[1]);
    }
}
