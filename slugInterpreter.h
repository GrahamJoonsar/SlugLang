#pragma once

#include <string>
#include <vector>
#include <map>

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
        int funcNum;
        std::vector<Function> functions; // standard functions for the interpreter
        std::map<std::string, int> integers; // integers that are declared
        std::map<std::string, float> floats; // floats that are declared
        std::map<std::string, std::string> strings; // stringss that are declared

        Interpreter();

        bool inFunctions(std::string potentialFunc);

        bool inInts(std::string potentialInt);

        std::vector<std::string> tokenizer(std::string passedInString);
};