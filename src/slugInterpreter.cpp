#include "slugInterpreter.h"
#include <iostream>
#include <ctype.h>
#include <math.h>

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

// Getting the string value of whatever has been passed in
std::string getStrValOf(std::string val, Interpreter * interp){
    if (interp->inInts(val)){ // integer var
        return std::to_string(interp->integers[val]);
    } else if (interp->inFloats(val)){ // float var
        return std::to_string(interp->floats[val]);
    } else if (interp->inStrings(val)){ // string var
        return interp->strings[val];
    } else { // string literal
        return takeOffFrontChar(val);
    } // ignore the dollar sign at the front
}

// Getting the numerical value of whatever's been passed in
float evalNum(std::string num, Interpreter * interp){
    if (!isdigit(num[0]) && num[0] != '-'){ // not a number literal and not a negative number literal
        if (interp->inInts(num)){
            return interp->integers[num];
        } else if (interp->inFloats(num)){
            return interp->floats[num];
        }
    } else {// Is a number literal
        return std::stof(num, nullptr);
    }
}

// Printing a string literal or var to the string
void print(std::string * printString, Interpreter * interp){ // ptinting a string
    std::cout << getStrValOf(printString[0], interp);
}

// Printing a string literal or var to the string with a newline
void println(std::string * printString, Interpreter * interp){ // ptinting a string
    std::cout << getStrValOf(printString[0], interp) << std::endl;
}

// A print that takes in a varied amount of arguments.
void slugPrintf(std::string * args, Interpreter * interp){
    std::string strToBeOutputted;
    for (int i = 0; i < interp->argsPassedIn; i++){
        strToBeOutputted += getStrValOf(args[i], interp);
    }
    std::cout << strToBeOutputted << std::endl;
}

/* Declaring and printing data types */
// takes in two arguments, name of var and value to set it to
void declareInt(std::string * declaration, Interpreter * interp){
    interp->integers[declaration[0]] = evalNum(declaration[1], interp);
}

// takes in two arguments, name of var and value to set it to
void declareFloat(std::string * declaration, Interpreter * interp){
    interp->floats[declaration[0]] = evalNum(declaration[1], interp);
}

// takes in two arguments, name of var and value to set it to
void declareStr(std::string * declaration, Interpreter * interp){
    interp->strings[declaration[0]] = getStrValOf(declaration[1], interp);
}

// takes in five arguments
// 1: name of bool
// 2: first val to be evaluated. Ex: 20
// 3: boolean operator Ex: >, <, ==
// 4: second val to be evaluated Ex: 10
void declareBool(std::string * args, Interpreter * interp){
    if (args[1][0] == '$' || interp->inStrings(args[1])){ // Comparison of string literal or string var
        std::string a = getStrValOf(args[1], interp);
        std::string b = getStrValOf(args[3], interp);
        if (args[2] == "=="){
            interp->booleans[args[0]] = a == b;
        } else if (args[2] == "<"){
            interp->booleans[args[0]] = a < b;
        } else if (args[2] == ">"){
            interp->booleans[args[0]] = a > b;
        }
    } else if (/*Number Literal*/(args[1][0] == '-' || isdigit(args[1][0])) || /*A number var*/(interp->inInts(args[1]) || interp->inFloats(args[1]))){
        float a = evalNum(args[1], interp);
        float b = evalNum(args[3], interp);
        if (args[2] == "=="){
            interp->booleans[args[0]] = a == b;
        } else if (args[2] == "<"){
            interp->booleans[args[0]] = a < b;
        } else if (args[2] == ">"){
            interp->booleans[args[0]] = a > b;
        } else if (args[2] == "<="){
            interp->booleans[args[0]] = a <= b;
        } else if (args[2] == ">="){
            interp->booleans[args[0]] = a >= b;
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
    std::string inStr;
    while(std::getline(std::cin, inStr)){ 
        if (inStr != ""){break;}
    } // getting a full line to account for spaces
    interp->strings[args[0]] = inStr;
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
    if (!interp->curlyBraceLevel[interp->curlyBraceNum + 1][1]){ // past if statement failed
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
    if (!interp->curlyBraceLevel[interp->curlyBraceNum + 1][1]){ // past if statement failed
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = true;
    } else {
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = false;
    }
}

void test(std::string * args, Interpreter * interp){
    std::cout << "Test" << std::endl;
}

void slugSystem(std::string * args, Interpreter * interp){
    system(getStrValOf(args[0], interp).c_str());
}

/* String manipulation functions */
// Standard sting concatenation
void slugConcat(std::string * args, Interpreter * interp){
    std::string fullStr = interp->strings[args[0]];
    for (int i = 1; i < interp->argsPassedIn; i++){
        fullStr += getStrValOf(args[i], interp);
    }
    interp->strings[args[0]] = fullStr;
}

// Reversing the string passed in
void reverseStr(std::string * args, Interpreter * interp){
    std::string reversedStr;
    std::string trueStr = getStrValOf(args[0], interp);
    for (int i = trueStr.length() - 1; i >= 0; i--){
        reversedStr += trueStr[i];
    }
    interp->strings[args[0]] = reversedStr;
}

// Getting the length of a string
// First val is the string
// Second val id the int to store the result in
void strLength(std::string * args, Interpreter * interp){
    interp->integers[args[1]] = getStrValOf(args[0], interp).length();
}

void slugSubstr(std::string * args, Interpreter * interp){
    std::string strVal = getStrValOf(args[0], interp);
    std::string result;
    result = strVal.substr((int)evalNum(args[2], interp), (int)evalNum(args[3], interp) - (int)evalNum(args[2], interp) + 1);
    interp->strings[args[1]] = result;
}

void operateOnAns(float * ans, char op, float num){
    switch (op){
        case '+': // Addition
            *ans += num;
            break;
        case '-': // Subtraction
            *ans -= num;
            break;
        case '*': // Multiplication
            *ans *= num;
            break;
        case '/': // Division
            *ans /= num;
            break;
        case '%': // Modulus
            *ans = (int)(*ans) % (int)num;
            break;
    }
}

// The function that performs mathematical operations
// does not do PEMDAS, but goes from left to right
// This includes the variable that it is going to in the assignment
void evalSet(std::string * args, Interpreter * interp){
    float ans = evalNum(args[0], interp); // Starting off with the var

    for (int i = 1; i < interp->argsPassedIn; i += 2){ // Looping through all of the 
        operateOnAns(&ans, args[i][0], evalNum(args[i + 1], interp));
    }

    // Storing the answer in the variable
    if (interp->inInts(args[0])){ // If the var passed in is an integer variable
        interp->integers[args[0]] = (int) ans;
    } else { // must be float
        interp->floats[args[0]] = ans;
    }
}

// Starting off with the first number/variable, not the variable it will be assigned into.
void evalAssign(std::string * args, Interpreter * interp){
    float ans = evalNum(args[1], interp); 

    for (int i = 2; i < interp->argsPassedIn; i += 2){ // Looping through all of the 
        operateOnAns(&ans, args[i][0], evalNum(args[i + 1], interp));
    }

    // Storing the answer in the variable
    if (interp->inInts(args[0])){ // If the var passed in is an integer variable
        interp->integers[args[0]] = (int) ans;
    } else { // must be float
        interp->floats[args[0]] = ans;
    }
}

// takes in one argument, a var to sqrt
void slugSQRT(std::string * args, Interpreter * interp){
    if (interp->inInts(args[0])){ // integer var passed in
        interp->integers[args[0]] = sqrt(interp->integers[args[0]]);
    } else { // float var passed in
        interp->floats[args[0]] = sqrt(interp->floats[args[0]]);
    }
}

// Easter egg, wont be included in the docs
void dispSlug(std::string * args, Interpreter * interp){
    std::cout << "            ______      (o)o)     " << std::endl;
    std::cout << "          _'      '_    ////      " << std::endl;
    std::cout << "        _'          '-.///        " << std::endl;
    std::cout << "     .-'               //         " << std::endl;
    std::cout << "     '---..____...---''           " << std::endl;
}

/* Interpreter Functions */
Interpreter::Interpreter(){ // whenever an interpreter is initiated
    // For negative argcounts, The number is the minimum amount of args that could be passed in
    curlyBraceLevel[0][0] = true; // When there are no curly braces
    /* standard functions */
    // print functions
    functions.push_back({"print", 1, &print}); // adding the print function
    functions.push_back({"println", 1, &println}); // adding the print function
    functions.push_back({"printf", -1, &slugPrintf}); // adding the printf function
    // Declaration functions
    functions.push_back({"int", 2, &declareInt}); // the int declaration.
    functions.push_back({"float", 2, &declareFloat}); // the float declaration
    functions.push_back({"string", 2, &declareStr}); // the float declaration
    functions.push_back({"bool", 4, &declareBool}); // the boolean declaration
    // Input functions
    functions.push_back({"readInt", 1, &readInt}); // reading an integer from the user
    functions.push_back({"readFloat", 1, &readFloat}); // reading a float from the user
    functions.push_back({"readStr", 1, &readStr}); // reading a string from the user
    /* Conditional functions */
    functions.push_back({"if", 1, &ifSlug}); // the if statement 
    functions.push_back({"elseif", 1, &elseifSlug}); // the else if statement
    functions.push_back({"else", 0, &elseSlug}); // the else statement
    // Mathematical functions
    functions.push_back({"evalSet", -3, &evalSet}); // This is for math that includes the variable it assigns to in the operations
    functions.push_back({"evalAssign", -4, &evalAssign}); // This is for math that does not include the number in the equation
    functions.push_back({"sqrt", 1, &slugSQRT}); // sqrts the variable passed in.
    // String operations
    functions.push_back({"concat", -2, &slugConcat});
    functions.push_back({"reverseStr", 1, &reverseStr});
    functions.push_back({"strLength", 2, &strLength});
    functions.push_back({"substr", 4, &slugSubstr});

    // Other stuff
    functions.push_back({"slug", 0, &dispSlug});
    functions.push_back({"system", 1, &slugSystem});
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

void Interpreter::callError(std::string errorMsg){
    std::cout << "Error on line: " << lineNum << std::endl;
    std::cout << currentLineOfFile << std::endl;
    std::cout << errorMsg << std::endl;
    exit(EXIT_FAILURE); // Stopping the program
}

std::vector<std::string> Interpreter::tokenizer(std::string passedInString){
    currentLineOfFile = passedInString;
    std::vector<std::string> tokens;
    std::string token;
    bool isString = false;
    bool charSeen;
    int tabLevel = 0;
    int spaceNum = 0;
    for (int i = 0; i < passedInString.length(); i++){ // looping through string
        if (passedInString[i] != ' ' && !isString){
            charSeen = true;
            if (passedInString[i] == '"'){ // string started
                isString = true;
            } else if (passedInString[i] == '#'){ // Comment
                return tokens; // Stop Tokenization
            } else {
                token += passedInString[i];
            }
        } else if (isString){
            if (passedInString[i] != '"'){
                token += passedInString[i];
            } else {
                isString = false;
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
    curlyBraceNum = tabLevel;
    return tokens;
}
