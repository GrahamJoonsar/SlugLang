#include "slugInterpreter.h"
#include <iostream>
#include <ctype.h>

/***** Standard functions *****/

bool Interpreter::inInts(std::string potentialInt){
    return integers.find(potentialInt) != integers.end();
}

bool Interpreter::inFloats(std::string potentialFloat){
    return floats.find(potentialFloat) != floats.end();
}

bool Interpreter::inStrings(std::string potentialStr){
    return strings.find(potentialStr) != strings.end();
}

std::string takeOffFrontChar(std::string str){
    return str.substr(1, str.length() - 1);
}

// Printing a string literal or var to the string
void print(std::string * printString, Interpreter * interp){ // ptinting a string
    if (interp->inInts(printString[0])){ // integer var
        std::cout << interp->integers.at(printString[0]);
    } else if (interp->inFloats(printString[0])){ // float var
        std::cout << interp->floats.at(printString[0]);
    } else if (interp->inStrings(printString[0])){ // string var
        std::cout << interp->strings.at(printString[0]);
    } else { // string literal
        std::cout << takeOffFrontChar(printString[0]);
    } // substr so we ignore the dollar sign at the front
}

// Printing a string literal or var to the string with a newline
void println(std::string * printString, Interpreter * interp){ // ptinting a string
    if (interp->inInts(printString[0])){ // integer var
        std::cout << interp->integers.at(printString[0]) << std::endl;
    } else if (interp->inFloats(printString[0])){ // float var
        std::cout << interp->floats.at(printString[0]) << std::endl;
    } else if (interp->inStrings(printString[0])){ // string var
        std::cout << interp->strings.at(printString[0]) << std::endl;
    } else { // string literal
        std::cout << takeOffFrontChar(printString[0]) << std::endl;
    } // substr so we ignore the dollar sign at the front
}

/* Declaring and printing data types */
// takes in two arguments, name of var and value to set it to
void declareInt(std::string * declaration, Interpreter * interp){
    interp->integers[declaration[0]] = std::stoi(declaration[1], nullptr, 10);
}

// takes in two arguments, name of var and value to set it to
void declareFloat(std::string * declaration, Interpreter * interp){
    interp->floats[declaration[0]] = std::stof(declaration[1], nullptr);
}

// takes in two arguments, name of var and value to set it to
void declareStr(std::string * declaration, Interpreter * interp){
    interp->strings[declaration[0]] = declaration[1];
}

// takes in five arguments
// 1: name of bool
// 2: type of vals that are compared
// 3: first val to be evaluated. Ex: 20
// 4: boolean operator Ex: >, <, ==
// 5: second val to be evaluated Ex: 10
void declareBool(std::string * args, Interpreter * interp){
    if (args[1] == "int"){ // comparing numbers
        int a;
        int b;
        if (!isdigit(args[2][0])){ // No variable should be declared with a number in front
            a = interp->integers[args[2]];
        } else {
            a = std::stoi(args[2], nullptr, 10);
        }
        if (!isdigit(args[4][0])){
            b = interp->integers[args[4]];
        } else {
            b = std::stoi(args[4], nullptr, 10);
        }
        if (args[3] == "=="){
            interp->booleans[args[0]] = (a == b);
        } else if (args[3] == ">"){
            interp->booleans[args[0]] = (a > b);
        } else if (args[3] == "<"){
            interp->booleans[args[0]] = (a < b);
        } else if (args[3] == ">="){
            interp->booleans[args[0]] = (a >= b);
        } else if (args[3] == "<="){
            interp->booleans[args[0]] = (a <= b);
        } else if (args[3] == "!="){
            interp->booleans[args[0]] = (a != b);
        }
    } else if (args[1] == "float"){ // comparing numbers
        float a;
        float b;
        if (!isdigit(args[2][0])){ // No variable should be declared with a number in front
            a = interp->floats[args[2]];
        } else {
            a = std::stof(args[2], nullptr);
        }
        if (!isdigit(args[4][0])){ // No variable should be declared with a number in front
            b = interp->floats[args[4]];
        } else {
            b = std::stof(args[4], nullptr);
        }
        if (args[3] == "=="){
            interp->booleans[args[0]] = (a == b);
        } else if (args[3] == ">"){
            interp->booleans[args[0]] = (a > b);
        } else if (args[3] == "<"){
            interp->booleans[args[0]] = (a < b);
        } else if (args[3] == ">="){
            interp->booleans[args[0]] = (a >= b);
        } else if (args[3] == "<="){
            interp->booleans[args[0]] = (a <= b);
        } else if (args[3] == "!="){
            interp->booleans[args[0]] = (a != b);
        }
    } else if (args[1] == "string"){
        if (args[2][0] == '$'){ // string literals should start with $
            args[2] = takeOffFrontChar(args[2]);
        } else {
            args[2] = interp->strings[args[2]];
        }
        if (args[4][0] == '$'){
            args[4] = takeOffFrontChar(args[4]);
        } else {
            args[4] = interp->strings[args[4]];
        }
        if (args[3] == "=="){
            interp->booleans[args[0]] = (args[2] == args[4]);
        } else if (args[3] == ">"){
            interp->booleans[args[0]] = (args[2] > args[4]);
        } else if (args[3] == "<"){
            interp->booleans[args[0]] = (args[2] < args[4]);
        } else if (args[3] == "!="){
            interp->booleans[args[0]] = (args[2] != args[4]);
        }
    }
}

/* Getting user input */
// Getting an int from the user
void readInt(std::string * args, Interpreter * interp){
    std::cin >> interp->integers[args[0]];
}

// Getting a float from the user
void readFloat(std::string * args, Interpreter * interp){
    std::cin >> interp->floats[args[0]];
}

// Getting a string from the user
void readStr(std::string * args, Interpreter * interp){ // The only way I could get it top consistently work
    while(std::getline(std::cin, interp->strings[args[0]])){ 
        if (interp->strings[args[0]] != ""){break;}
    } // getting a full line to account for spaces
}

/* conditionals */
// Takes one argument, a boolean variable
void ifSlug(std::string * args, Interpreter * interp){
    if (interp->booleans[args[0]]){
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = true;
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][1] = true;
    } else {
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = false;
    }
}

void elseifSlug(std::string * args, Interpreter * interp){
    bool t = !interp->curlyBraceLevel[interp->curlyBraceNum + 1][1];
    if (t){ // past if statement failed
        if (interp->booleans[args[0]]){
            interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = true;
            interp->curlyBraceLevel[interp->curlyBraceNum + 1][1] = true;
        } else {
            interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = false;
        }
    } else {
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = false;
    }
}

void elseSlug(std::string * args, Interpreter * interp){
    bool t = !interp->curlyBraceLevel[interp->curlyBraceNum + 1][1];
    if (t){ // past if statement failed
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = true;
    } else {
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = false;
    }
}

void test(std::string * args, Interpreter * interp){
    std::cout << "Test" << std::endl;
}

Interpreter::Interpreter(){ // whenever an interpreter is initiated
    curlyBraceLevel[0][0] = true; // When there are no curly braces
    booleans["boolTest"] = true;
    /* standard functions */
    functions.push_back({"print", 1, &print}); // adding the print function
    functions.push_back({"println", 1, &println}); // adding the print function
    functions.push_back({"int", 2, &declareInt}); // the int declaration.
    functions.push_back({"float", 2, &declareFloat}); // the float declaration
    functions.push_back({"string", 2, &declareStr}); // the float declaration
    functions.push_back({"bool", 5, &declareBool}); // the boolean declaration
    functions.push_back({"readInt", 1, &readInt}); // reading an integer from the user
    functions.push_back({"readFloat", 1, &readFloat}); // reading a float from the user
    functions.push_back({"readStr", 1, &readStr}); // reading a string from the user
    /* Conditional functions */
    functions.push_back({"if", 1, &ifSlug}); // the if statement 
    functions.push_back({"elseif", 1, &elseifSlug}); // the else if statement
    functions.push_back({"else", 0, &elseSlug});
    // Other stuff
    functions.push_back({"test", 0, &test});
}

bool Interpreter::inFunctions(std::string potentialFunc){
    int i = 0;
    for (Function fun : functions){
        if (potentialFunc == fun.name){
            //std::cout << fun.name << std::endl;
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
    int tabLevel = 0;
    for (int i = 0; i < passedInString.length(); i++){ // looping through string
        if (passedInString[i] != ' ' && !isString){
            if (passedInString[i] == '"'){ // string started
                isString = true;
            } else if (passedInString[i] == '#'){ // Comment
                break;
            } else if (passedInString[i] == '~'){ // tab
                tabLevel++;
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
    curlyBraceNum = tabLevel;
    return tokens;
}
