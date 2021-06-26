#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdlib.h>

class Interpreter;

struct Function{
    std::string name; // Name of the function
    int argc; // the amount of arguments it takes
    void (*actualFunc)(std::string*, Interpreter*);
    Function(std::string _name, int _argc, void (*func)(std::string*, Interpreter*)){
        name = _name;
        argc = _argc;
        actualFunc = func;
    }
};

class Interpreter{
    public:
        int argcountForFunc = 1;
        int argsPassedIn = 1;
        int lineNum = 1;
        std::vector<std::string> fullFile;

        int funcNum;
        bool curlyBraceLevel[10][2]; // Hopefully not more than 10 layer of curly braces
        int curlyBraceNum = 0;
        bool isValid = true;
        std::vector<Function> functions; // standard functions for the interpreter

        /* User declared Variables */
        std::map<std::string, int> integers; // integers that are declared
        std::map<std::string, float> floats; // floats that are declared
        std::map<std::string, std::string> strings; // stringss that are declared
        std::map<std::string, bool> booleans; // booleans that are declared

        std::map<std::string, int> pointNums;

        Interpreter();

        bool inFunctions(std::string potentialFunc);
        bool inInts(std::string);
        bool inFloats(std::string);
        bool inStrings(std::string);
        bool inBools(std::string);

        std::vector<std::string> tokenizer(std::string passedInString);
        void callError(std::string errorMsg);
};
