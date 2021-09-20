#include "slugInterpreter.h"
#include <iostream>

/***** Standard functions *****/
// Checks if the variable name passed in is an integer variable
bool Interpreter::inInts(std::string potentialInt){
    return integers.find(potentialInt) != integers.end();
}

// Checks if the variable name passed in is a float variable
bool Interpreter::inFloats(std::string potentialFloat){
    return floats.find(potentialFloat) != floats.end();
}

// Checks if the variable name passed in is a string variable
bool Interpreter::inStrings(std::string potentialStr){
    return strings.find(potentialStr) != strings.end();
}

// Checks if the variable name passed in is a bool variable
bool Interpreter::inBools(std::string potentialBool){
    return booleans.find(potentialBool) != booleans.end();
}


// Checking if in standard c++ slugLang audience
bool Interpreter::inFunctions(std::string potentialFunc){
    return functions.find(potentialFunc) != functions.end();
}

// For user defined functions
bool Interpreter::inUFunctions(std::string potentialUFunc){
    return UFunctions.find(potentialUFunc) != UFunctions.end();
}

// For calling an error
void Interpreter::callError(std::string errorMsg){
    std::cout << "Error (around) line: " << lineNum + 1 << std::endl;
    std::cout << currentLineBeingProcessed << std::endl;
    std::cout << errorMsg << std::endl;
    exit(EXIT_FAILURE); // Stopping the program
}

// Very complicated tokenization
std::vector<std::string> Interpreter::tokenizer(std::string passedInString){
    std::vector<std::string> tokens;
    std::string token;
    bool isString = false;
    bool charSeen = false;
    int tabLevel = 0;
    int spaceNum = 0;
    int pareNum = 0;
    int braceNum = 0;
    bool seenParen = false;
    bool seenBrace = false;
    bool definingFunc = false;
    bool stringExpression = false;
    bool stringString = false;
    bool literalizing = false;
    std::string variable = "";
    for (unsigned int i = 0; i < passedInString.length(); i++){ // looping through string
        if (passedInString[i] == '{' && !isString){ 
            literalizing = true;
        } else if (literalizing){
            if (passedInString[i] == '}'){
                literalizing = false;
                token += std::to_string((int)evalNum(variable, this));
            } else {
                variable += passedInString[i];
            }
        } else if (passedInString[i] != ' ' && !isString && !seenParen && !stringExpression && !seenBrace){
            charSeen = true;
            if (passedInString[i] == '"'){ // string started
                if (token != ""){
                    tokens.push_back(token);
                    token = "";
                }
                token += '"';
                isString = true;
            } else if (passedInString[i] == '#'){ // Comments and ignored characters
                if (token != ""){
                    tokens.push_back(token);
                }
                curlyBraceNum = tabLevel;
                return tokens; // Stop Tokenization
            } else if (passedInString[i] == '('){ // Expression or defining function parameters
                if (tokens[0] == "func"){
                    if (token != ""){
                        tokens.push_back(token);
                    }
                    token = "";
                    definingFunc = true;
                } else {
                    seenParen = true;
                    token += '(';
                    pareNum++;
                }
            } else if (passedInString[i] == '['){ // Getting val returned from a function
                seenBrace = true;
                if (token != ""){
                    tokens.push_back(token);
                    token = "";
                }
                token += '[';
                braceNum++;
            } else if (passedInString[i] == '$'){ // string expression
                stringExpression = true;
                token += '$';
            } else {
                token += passedInString[i];
            } 
        } else if (isString){
            if (passedInString[i] != '"'){
                token += passedInString[i];
            } else {
                isString = false;
                if (token != ""){
                    tokens.push_back(token);
                    token = "";
                }
            }
        } else if (seenBrace){
            token += passedInString[i];
            if (passedInString[i] == '['){
                braceNum++;
            } else if (passedInString[i] == ']'){
                braceNum--;
                if (braceNum == 0){
                    seenBrace = false;
                    token.pop_back(); // Taking off the ]
                    tokens.push_back(token);
                    token = "";
                }
            }
        } else if (stringExpression){
            if (passedInString[i] != '$'){
                if (passedInString[i] == '"'){
                    stringString = !stringString;
                }
                token += passedInString[i];
            } else {
                if (!stringString){
                    stringExpression = false;
                } else {
                    token += '$';
                }
            }
        } else if (seenParen){
            token += passedInString[i];
            if (passedInString[i] == '('){
                pareNum++;
            } else if (passedInString[i] == ')'){
                pareNum--;
                if (pareNum == 0){
                    seenParen = false; 
                    tokens.push_back(token);
                    token = "";
                }
            }
        } else if (definingFunc){
            if (passedInString[i] != ')'){
                token += passedInString[i];
            } else {
                tokens.push_back(token.substr(1, token.length() - 1));
                token = "";
                definingFunc = false;
                return tokens; // Shouldnt be anything else after this
            }
        } else { // space
            spaceNum++;
            if (spaceNum == 4 && !charSeen){
                tabLevel++;
                spaceNum = 0;
            }
            if (token != "" && charSeen){
                tokens.push_back(token);
                token = "";
            }
        }
    }
    if (token != ""){
        tokens.push_back(token); // adding last token
    }

    for (auto t : tokens){

    }

    curlyBraceNum = tabLevel;
    return tokens;
}
