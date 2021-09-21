#include "slugInterpreter.h"
#include <iostream>
#include <fstream>
//#include <ctype.h>
#include <cmath>
#include "evaluations.h"

extern void proccessLine(std::string line);

// Unused currently
int getPrecedence(char op){
    if (op == '+' || op == '-'){
        return 1;
    } else if (op == '*' || op == '/'){
        return 2;
    } else if (op == '%'){
        return 3;
    } else if (op == '^'){
        return 4;
    } else {
        return -1;
    }
}

// Checks if a variable name is valid
bool isValidVarName(std::string name){
    if (isalpha(name[0]) || name[0] == '_'){
        if (name.length() == 1){return true;}
        for (unsigned int i = 1; i < name.length(); i++){
            if (!isalnum(name[i]) && name[i] != '_'){return false;}
        }
        return true;
    } else {
        return false;
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
    if (isValidVarName(declaration[0])){
        interp->integers[declaration[0]] = evalNum(declaration[1], interp);
    } else {
        interp->callError(declaration[0] + " is not a valid variable name.");
    }

}

// takes in two arguments, name of var and value to set it to
void declareFloat(std::string * declaration, Interpreter * interp){
    if (isValidVarName(declaration[0])){
        interp->floats[declaration[0]] = evalNum(declaration[1], interp);
    } else {
        interp->callError(declaration[0] + " is not a valid variable name.");
    }
}

// takes in two arguments, name of var and value to set it to
void declareStr(std::string * declaration, Interpreter * interp){
    if (isValidVarName(declaration[0])){
        interp->strings[declaration[0]] = getStrValOf(declaration[1], interp);
    } else {
        interp->callError(declaration[0] + " is not a valid variable name.");
    }
}

// takes in a bollean expression
void declareBool(std::string * args, Interpreter * interp){
    std::string temp[32];
    for (int i = 1; i < interp->argsPassedIn; i++){ // Collecting everything but the variable name
        temp[i-1] = args[i];
    }
    if (isValidVarName(args[0])){
        interp->booleans[args[0]] = getBooleanValOf(temp, interp, interp->argsPassedIn - 1);
    } else {
        interp->callError(args[0] + " is not a valid variable name.");
    }
}

// Setting the value of a variable so you doin't have to specify the type
// (It must exist before though)
void setSlug(std::string * args, Interpreter * interp){
    if (interp->inInts(args[0])){ // Setting an integer variable
        interp->integers[args[0]] = evalNum(args[1], interp);
    } else if (interp->inFloats(args[0])){
        interp->floats[args[0]] = evalNum(args[1], interp);
    } else if (interp->inStrings(args[0])){
        interp->strings[args[0]] = getStrValOf(args[1], interp);
    } else if (interp->inBools(args[0])){
        std::string temp[32];
        for (int i = 1; i < interp->argsPassedIn; i++){ // Collecting everything but the variable name
            temp[i-1] = args[i];
        }
        interp->booleans[args[0]] = getBooleanValOf(temp, interp, interp->argsPassedIn - 1);
    } else {
        interp->callError(args[0] + " is not a variable in existence.");
    }
}

// Changing a global variable inside a function
void slugMutate(std::string * args, Interpreter * interp){
    for (int i = 0; i < interp->argsPassedIn; i++){
        if (interp->inInts(args[i])){
            interp->int_temp[args[i]] = interp->integers[args[i]];
        } else if (interp->inFloats(args[i])){
            interp->float_temp[args[i]] = interp->floats[args[i]];
        } else if (interp->inStrings(args[i])){
            interp->string_temp[args[i]] = interp->strings[args[i]];
        } else if (interp->inBools(args[i])){
            interp->bool_temp[args[i]] = interp->booleans[args[i]];
        }
    }
}

void setAndMutateSlug(std::string * args, Interpreter * interp){
    if (interp->inInts(args[0])){ // Setting an integer variable
        interp->integers[args[0]] = evalNum(args[1], interp);
    } else if (interp->inFloats(args[0])){
        interp->floats[args[0]] = evalNum(args[1], interp);
    } else if (interp->inStrings(args[0])){
        interp->strings[args[0]] = getStrValOf(args[1], interp);
    } else if (interp->inBools(args[0])){
        std::string temp[32];
        for (int i = 1; i < interp->argsPassedIn; i++){ // Collecting everything but the variable name
            temp[i-1] = args[i];
        }
        interp->booleans[args[0]] = getBooleanValOf(temp, interp, interp->argsPassedIn - 1);
    } else {
        interp->callError(args[0] + " is not a variable in existence.");
    }
    // Mutating
    int i = 0;
    if (interp->inInts(args[i])){
            interp->int_temp[args[i]] = interp->integers[args[i]];
        } else if (interp->inFloats(args[i])){
            interp->float_temp[args[i]] = interp->floats[args[i]];
        } else if (interp->inStrings(args[i])){
            interp->string_temp[args[i]] = interp->strings[args[i]];
        } else if (interp->inBools(args[i])){
            interp->bool_temp[args[i]] = interp->booleans[args[i]];
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
// Takes one argument, a boolean expression
void ifSlug(std::string * args, Interpreter * interp){
    interp->curlyBraceLevel[interp->curlyBraceNum + 1][1] = false;
    if (getBooleanValOf(args, interp, interp->argsPassedIn)){
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = true;
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][1] = true;
    } else {
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = false;
    }
}

// else if 
void elseifSlug(std::string * args, Interpreter * interp){
    if (!interp->curlyBraceLevel[interp->curlyBraceNum + 1][1]){ // past if statement failed
        if (getBooleanValOf(args, interp, interp->argsPassedIn)){
            interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = true;
            interp->curlyBraceLevel[interp->curlyBraceNum + 1][1] = true;
        } else {
            interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = false;
        }
    } else {
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = false;
    }
}

// Else
void elseSlug(std::string * args, Interpreter * interp){
    if (!interp->curlyBraceLevel[interp->curlyBraceNum + 1][1]){ // past if statement failed
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = true;
    } else {
        interp->curlyBraceLevel[interp->curlyBraceNum + 1][0] = false;
    }
}

// Deprecated
void test(std::string * args, Interpreter * interp){
    std::cout << "Test" << std::endl;
}

// Just like the system command in c++
// Takes in one string arg to execute in the console
void slugSystem(std::string * args, Interpreter * interp){
    system(getStrValOf(args[0], interp).c_str());
}

/* String manipulation functions */
// Standard sting concatenation
// Concats the second string val (arg2) onto the string variable (arg1)
void slugConcat(std::string * args, Interpreter * interp){
    std::string fullStr = interp->strings[args[0]];
    for (int i = 1; i < interp->argsPassedIn; i++){
        fullStr += getStrValOf(args[i], interp);
    }
    interp->strings[args[0]] = fullStr;
}

// Reversing the string passed in
// Must be a string variable
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

// Gets a substr of a string first num is starting index, second num is ending index
void slugSubstr(std::string * args, Interpreter * interp){
    std::string strVal = getStrValOf(args[0], interp);
    std::string result;
    result = strVal.substr((int)evalNum(args[2], interp), (int)evalNum(args[3], interp) - (int)evalNum(args[2], interp) + 1);
    interp->strings[args[1]] = result;
}

// Indexing a string at the specific index
void slugGetchar(std::string * args, Interpreter * interp){
    interp->strings[args[2]] = getStrValOf(args[0], interp)[(int)evalNum(args[1], interp)];
}

// Puts a newline in the console
void slugNewline(std::string * args, Interpreter * interp){
    std::cout << std::endl;
}

void slugSetChar(std::string * args, Interpreter * interp){
    interp->strings[args[0]][(int)evalNum(args[1], interp)] = getStrValOf(args[2], interp)[0];
}

// The function that performs mathematical operations
// does not do PEMDAS, but goes from left to right
// This includes the variable that it is going to in the assignment
void evalSet(std::string * args, Interpreter * interp){
    float ans = evalNum(args[1], interp); // Starting off with the var
    // Storing the answer in the variable
    if (interp->inInts(args[0])){ // If the var passed in is an integer variable
        interp->integers[args[0]] = (int) ans;
    } else { // must be float
        interp->floats[args[0]] = ans;
    }
}

// ++ operater
void slugIncrement(std::string * args, Interpreter * interp){
    if (interp->inInts(args[0])){
        (interp->integers[args[0]]) = (interp->integers[args[0]]) + 1;
    } else if (interp->inFloats(args[0])){
        (interp->floats[args[0]]) = (interp->floats[args[0]]) + 1;
    }
}

// -- operator
void slugDecrement(std::string * args, Interpreter * interp){
    if (interp->inInts(args[0])){
        (interp->integers[args[0]]) = (interp->integers[args[0]]) - 1;
    } else if (interp->inFloats(args[0])){
        (interp->floats[args[0]]) = (interp->floats[args[0]]) - 1;
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

// Deleting a variable
extern void slugDelete(std::string * args, Interpreter * interp){
    for (int i = 0; i < interp->argsPassedIn; i++){
        if (interp->inInts(args[i])){
            interp->integers.erase(args[i]);
        } else if (interp->inFloats(args[i])){
            interp->floats.erase(args[i]);
        } else if (interp->inStrings(args[i])){
            interp->strings.erase(args[i]);
        } else if (interp->inBools(args[i])){
            interp->booleans.erase(args[i]);
        }
    }
}

/* Goto Statements 
// Deprecated
void slugPoint(std::string * args, Interpreter * interp){ // Setting where the goto goes
    interp->pointNums[args[0]] = interp->lineNum;
}

void slugGoto(std::string * args, Interpreter * interp){ // Actually going to the point
    interp->lineNum = interp->pointNums[args[0]];
}
*/
/* System like commands */
// Force end the program
void slugQuit(std::string * args, Interpreter * interp){
    exit(EXIT_SUCCESS);
}

// Gets parameter's types and names
std::vector<std::string> funcTokenizer(std::string str){
    std::vector<std::string> tokens;
    std::string token;
    for (unsigned int i = 0; i < str.length(); i++){
        if (str[i] != ' ' && str[i] != ')'){
            token += str[i];
        } else {
            if (token != ""){
                tokens.push_back(token);
                token = "";
            }
        }
    }
    if (token != ""){
        tokens.push_back(token);
    }
    return tokens;
}

/* Functions */
void defineFunc(std::string * args, Interpreter * interp){
    auto name = args[0]; // function name
    interp->currentFunctionName = name;
    auto params = funcTokenizer(args[1]);
    int argc;
    if (args[1] == ")"){
        argc = 0;
    } else {
        argc = params.size()/2; // types and names / 2
    }
    auto func = UserDefinedFunction(argc, name, params);
    interp->UFunctions.insert({name, func});
    interp->isDefiningFunction = true; // so proccessLine doesn't execute stuff in a function
}

// Returning a value from a 
// Type must be specified in the return statement, not the function signature
void slugReturn(std::string * args, Interpreter * interp){
    switch(args[0][0]){
        case 'i': // int
            interp->returnedVal.i = evalNum(args[1], interp);
            interp->rt = RETURN_ENUM::INT;
            break;
        case 'f': // float
            interp->returnedVal.f = evalNum(args[1], interp);
            interp->rt = RETURN_ENUM::FLOAT;
            break;
        case 's': // String
            interp->returnedVal.s = getStrValOf(args[1], interp);
            interp->rt = RETURN_ENUM::STRING;
            break;
        case 'b': // Bool
            std::string temp[32];
            for (int i = 1; i < interp->argsPassedIn; i++){ // Collecting everything but the variable name
                temp[i-1] = args[i];
            }
            interp->returnedVal.b = getBooleanValOf(temp, interp, interp->argsPassedIn - 1);
            interp->rt = RETURN_ENUM::BOOL;
            break;
    }
    interp->isReturning = true;
    interp->breakingLoop = true;
}

// Getting a value returned from a function
// Can be on the same line as the function called
// abs -90 into var
void slugInto(std::string * args, Interpreter * interp){
    switch(interp->rt){
        case RETURN_ENUM::INT:
            interp->integers[args[0]] = interp->returnedVal.i;
            break;
        case RETURN_ENUM::FLOAT:
            interp->floats[args[0]] = interp->returnedVal.f;
            break;
        case RETURN_ENUM::STRING:
            interp->strings[args[0]] = interp->returnedVal.s;
            break;
        case RETURN_ENUM::BOOL:
            interp->booleans[args[0]] = interp->returnedVal.b;
            break;
    }
}

// File inclusion
// Will not include the same file twice
void slugInclude(std::string * args, Interpreter * interp){
    auto path = getStrValOf(args[0], interp);
    for (auto t : interp->includedFiles){ // So files aren't proccessed twice
        if (t == path){ // Don't want to reinclude
            return;
        }
    }
    interp->includedFiles.push_back(path);
    std::ifstream inputFile;
    std::string line;
    inputFile.open(path);
    if (inputFile.is_open()){
        while (getline(inputFile, line)){
            proccessLine(line);
        }
    } else {
        interp->callError("The file " + path + " cannot be found.");
    }
    inputFile.close();
}

// Loops
// Typical while loop, takes in boolean expression
void slugWhile(std::string * args, Interpreter * interp){
    auto pargs = new std::string[interp->argsPassedIn]; // for the pointer
    for (int i = 0; i < interp->argsPassedIn; i++){
        std::string temp = args[i];
        pargs[i] = temp;
    }
    std::string tabs = "";
    for (int i = 0; i < interp->curlyBraceNum; i++){
        tabs += "    ";
    }
    interp->wstack.push({pargs, interp->argsPassedIn, false, " ", tabs});
    interp->definingLoop = true;
}

// Typical for loop
// Variable name, boolean expression, action seperated by ;
// for int i 0 ; i < 100 ; incr i
//     println i
// endw
void slugFor(std::string * args, Interpreter * interp){
    std::string set; // the int i part
    std::string booleanExpression; // i < num
    std::string action; // i++
    int boolPartNum = 0;
    bool seenBool = false;
    int boolStartIndex;
    int sepNum = 0; // Number of seperators
    for (int i = 0; i < interp->argsPassedIn; i++){
        if (args[i] == ";"){ // Seperator
            sepNum++;
        } else {
            switch(sepNum){
                case 0:
                    set += args[i] + ' ';
                    break;
                case 1:
                    if (!seenBool){
                        seenBool = true;
                        boolStartIndex = i;
                    }
                    booleanExpression += args[i] + ' ';
                    boolPartNum++;
                    break;
                case 2:
                    action += args[i] + ' ';
                    break;
            }
        }
    }
    auto pfargs = new std::string[boolPartNum]; // for the pointer
    for (int i = boolStartIndex; i < boolStartIndex + boolPartNum; i++){
        std::string temp = args[i];
        pfargs[i - boolStartIndex] = temp;
    }
    std::string tabs = "";
    for (int i = 0; i < interp->curlyBraceNum; i++){
        tabs += "    ";
    }
    proccessLine(set);
    interp->definingLoop = true;
    interp->wstack.push({pfargs, boolPartNum, true, action, tabs});
}

void slugNamespace(std::string * args, Interpreter * interp){
    interp->namespaceNames.push_back(args[0]);
}

void slugEndNamespace(std::string * args, Interpreter * interp){
    interp->namespaceNames.pop_back();
}

// Breaking out of a loop
void slugBreak(std::string * args, Interpreter * interp){
    interp->breakingLoop = true;
}

/**void slugEndWhile(std::string * args, Interpreter * interp){
    interp->definingLoop = false;
    while(getBooleanValOf(interp->wstack.back().booleanExpression, interp, interp->wstack.back().length)){
        for (auto l : interp->wstack.back().linesOfLoop){
            proccessLine(l);
        }
    }
    interp->wstack.pop_back();
}*/

// Most powerful and useful function in sluglang, runs the string passed in
void slugExecute(std::string * args, Interpreter * interp){
    proccessLine(getStrValOf(args[0], interp));
}

void slugCheckType(std::string * args, Interpreter * interp){
    interp->rt = RETURN_ENUM::RETURN_TYPE::STRING;
    if (interp->inInts(getStrValOf(args[0], interp))){
        interp->returnedVal.s = "i";
    } else if (interp->inFloats(getStrValOf(args[0], interp))){
        interp->returnedVal.s = "f";
    } else if (interp->inStrings(getStrValOf(args[0], interp))){
        interp->returnedVal.s = "s";
    } else if (interp->inBools(getStrValOf(args[0], interp))){
        interp->returnedVal.s = "b";
    }
}

/* Interpreter Functions */
// Adding the functions to the interpreter
Interpreter::Interpreter(){ // whenever an interpreter is initiated
    // For negative argcounts, The number is the minimum amount of args that could be passed in
    curlyBraceLevel[0][0] = true; // When there are no curly braces
    // Boolean constants
    booleans["true"] = true;
    booleans["false"] = false;

    strings["newline"] = "\n";
    strings["tab"] = "\t";

    /* standard functions */
    // print functions
    functions.insert({"print", {"print", 1, &print}}); // adding the print function
    functions.insert({"println", {"println", 1, &println}}); // adding the print function
    functions.insert({"printf", {"printf", -1, &slugPrintf}}); // adding the printf function
    // Variable functions
    functions.insert({"int", {"int", 2, &declareInt}}); // the int declaration.
    functions.insert({"float", {"float", 2, &declareFloat}}); // the float declaration
    functions.insert({"string", {"string", 2, &declareStr}}); // the float declaration
    functions.insert({"bool", {"bool", -2, &declareBool}}); // the boolean declaration
    functions.insert({"set", {"set", -2, &setSlug}}); // setting a variable to a value (must exist beforehand)
    functions.insert({"setm", {"setm", 2, &setAndMutateSlug}}); // setting a variable to a value (must exist beforehand)
    // Input functions
    functions.insert({"readInt", {"readInt", 1, &readInt}}); // reading an integer from the user
    functions.insert({"readFloat", {"readFloat", 1, &readFloat}}); // reading a float from the user
    functions.insert({"readStr", {"readStr", 1, &readStr}}); // reading a string from the user
    /* Conditional functions */
    functions.insert({"if", {"if", -1, &ifSlug}}); // the if statement 
    functions.insert({"elseif", {"elseif", -1, &elseifSlug}}); // the else if statement
    functions.insert({"else", {"else", 0, &elseSlug}}); // the else statement
    // Mathematical functions
    functions.insert({"eval", {"eval", 2, &evalSet}}); // Assigning the var to the expression
    functions.insert({"sqrt", {"sqrt", 1, &slugSQRT}}); // sqrts the variable passed in.
    functions.insert({"incr", {"incr", 1, &slugIncrement}}); // sqrts the variable passed in.
    functions.insert({"decr", {"decr", 1, &slugDecrement}}); // sqrts the variable passed in.
    // String operations
    functions.insert({"concat", {"concat", -2, &slugConcat}}); // String concatenation
    functions.insert({"reverseStr", {"reverseStr", 1, &reverseStr}}); // Reversing the string passed in
    functions.insert({"strLength", {"strLength", 2, &strLength}}); // Getting the length of the string passed in
    functions.insert({"substr", {"substr", 4, &slugSubstr}}); // substr of a string passed in
    functions.insert({"getch", {"getch", 3, &slugGetchar}});
    functions.insert({"setch", {"setch", 3, &slugSetChar}});
    functions.insert({"newl", {"newl", 0, &slugNewline}});
    // Goto statements
    /*functions.push_back({"point", 1, &slugPoint});
    functions.push_back({"goto", 1, &slugGoto});*/
    // System commands
    functions.insert({"system", {"system", 1, &slugSystem}});
    functions.insert({"quit", {"quit", 0, &slugQuit}});
    // Function functions
    functions.insert({"func", {"func", 2, &defineFunc}});
    functions.insert({"return", {"return", -1, &slugReturn}});
    functions.insert({"into", {"into", 1, &slugInto}}); // Collects a returned val and puts it in a var
    functions.insert({"mutate", {"mutate", -1, &slugMutate}}); // Changes a variable globally
    // End is inserti"hey", cally a function, but a marker
    // Loops
    functions.insert({"while", {"while", -1, &slugWhile}});
    functions.insert({"for", {"for", -1, &slugFor}});
    functions.insert({"break", {"break", 0, &slugBreak}});
    //functions.push_back({"endw", 0, &slugEndWhile});
    // File functions
    functions.insert({"include", {"include", 1, &slugInclude}});
    // Other stuff
    functions.insert({"slug", {"slug", 0, &dispSlug}});
    functions.insert({"exec", {"exec", 1, &slugExecute}});
    functions.insert({"getType", {"getType", 1, &slugCheckType}});
    functions.insert({"delete", {"delete", -1, &slugDelete}});
}
