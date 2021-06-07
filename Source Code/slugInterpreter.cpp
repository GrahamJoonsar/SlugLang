#include "slugInterpreter.h"
#include <iostream>

// Standard functions
// Printing a string literal or var to the string
void print(std::string * printString, Interpreter * interp){ // ptinting a string
    if (interp->integers.find(printString[0]) != interp->integers.end()){ // integer var
        std::cout << interp->integers.at(printString[0]) << std::endl;
    } else if (interp->floats.find(printString[0]) != interp->floats.end()){ // float var
        std::cout << interp->floats.at(printString[0]) << std::endl;
    } else if (interp->strings.find(printString[0]) != interp->strings.end()){ // string var
        std::cout << interp->strings.at(printString[0]) << std::endl;
    } else {
        std::cout << printString[0] << std::endl;
    }
}

// Declaring and printing data types
void declareInt(std::string * declaration, Interpreter * interp){
    interp->integers[declaration[0]] = std::stoi(declaration[1], nullptr, 10);
}

void declareFloat(std::string * declaration, Interpreter * interp){
    interp->floats[declaration[0]] = std::stof(declaration[1], nullptr);
}

void declareStr(std::string * declaration, Interpreter * interp){
    interp->strings[declaration[0]] = declaration[1];
}

Interpreter::Interpreter(){
    functions.push_back({"print", 1, &print}); // adding the print function
    functions.push_back({"int", 2, &declareInt}); // the int declaration.
    functions.push_back({"float", 2, &declareFloat}); // the float declaration
    functions.push_back({"string", 2, &declareStr}); // the float declaration
}

bool Interpreter::inFunctions(std::string potentialFunc){
    int i = 0;
    for (Function fun : functions){
        if (potentialFunc == fun.name){
            argcountForFunc = fun.argc;
            funcNum = i;
            return true;
        }
        i++;
    }
    return false;
}

std::vector<std::string> Interpreter::tokenizer(std::string passedInString){
    std::vector<std::string> tokens;
    std::string token;
    bool isString = false;
    for (int i = 0; i < passedInString.length(); i++){ // looping through string
        if (passedInString[i] != ' ' && !isString){
            if (passedInString[i] == '"'){ // string started
                isString = true;
            } else {
                token += passedInString[i];
            }
        } else if (isString){
            if (passedInString[i] != '"'){
                token += passedInString[i];
            } else {
                isString = false;
            }
        } else {
            if (token != ""){
                tokens.push_back(token);
                token = "";
            }
        }
    }
    tokens.push_back(token); // adding last token
    return tokens;
}
