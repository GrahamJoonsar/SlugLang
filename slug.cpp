#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Function{
    std::string name; // Name of the function
    const int argc; // the amount of arguments it takes
    void (*actualFunc)(std::string*); // the actual function
};

std::vector<Function> functions;

int argcountForFunc = 1;
int funcNum;

bool inFunctions(std::string potentialFunc){
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

std::vector<std::string> tokenizer(std::string passedInString){
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
            tokens.push_back(token);
            token = "";
        }
    }
    tokens.push_back(token); // adding last token
    return tokens;
}

void print(std::string * printString){
    std::cout << printString[0] << std::endl;
}

int main(int argc, char * argv[]){
    // Functions definition
    
    functions.push_back({"print", 1, print}); // print function


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
    bool functionCalled = false;

    while (getline(inputFile, currentLine)){
        currentLineTokens = tokenizer(currentLine);
        for (int i = 0; i < currentLineTokens.size(); i++){ // looping through tokens
            if (inFunctions(currentLineTokens[i])){ // function was called, and getting the number of arguments for the funvtion
                for (int j = 0; j < argcountForFunc; j++){
                    i++;
                    args[j] = currentLineTokens[i];
                }
                functions[funcNum].actualFunc(args);
            }
        }
    }

    // Closing the file
    inputFile.close();
}