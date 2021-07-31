#pragma once

#include <string>
#include <vector>
#include <unordered_map>
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

struct UserDefinedFunction{
    std::string name;
    int argc;
    std::vector<std::string> linesOfFunction;
    std::vector<std::string> params;
    UserDefinedFunction(int _argc, std::string _name, std::vector<std::string> _params){
        argc = _argc;
        name = _name;
        params = _params;
    }
};

struct ScopedVariables{
    std::unordered_map<std::string, int> integers; // integers that are declared
    std::unordered_map<std::string, float> floats; // floats that are declared
    std::unordered_map<std::string, std::string> strings; // stringss that are declared
    std::unordered_map<std::string, bool> booleans; // booleans that are declared
};

// Storing the varibles for functions
class VariableStack{
    private:
    std::vector<ScopedVariables> v; 
    public:
    void push(ScopedVariables sv);
    ScopedVariables back();
    void pop_back();
    unsigned int length();
};

union VAL_RETURNED{
    int i;
    float f;
    std::string s;
    bool b;
    VAL_RETURNED(){}
    ~VAL_RETURNED(){}
};

enum RETURN_TYPE{
    INT,
    FLOAT,
    STRING,
    BOOL
};

class WhileLoop{
    public:
        std::vector<std::string> linesOfLoop;
        std::string* booleanExpression;
        int length;
        std::string tabs = "";
        WhileLoop(std::string* _bExpression, int _length){
            booleanExpression = _bExpression;
            length = _length;
        }
};

class WhileLoopStack{
    private:
    std::vector<WhileLoop> v; 
    public:
    void push(WhileLoop sv);
    WhileLoop back();
    void pop_back();
    unsigned int length();
    void addl(std::string l);
};

class Interpreter{
    public:
        int argcountForFunc = 1;
        unsigned short int argsPassedIn = 1;
        unsigned int lineNum = 1;
        std::vector<std::string> fullFile;

        unsigned int funcNum;
        bool curlyBraceLevel[10][2]; // Hopefully not more than 10 layer of curly braces
        int loopLevel[10];
        int curlyBraceNum = 0;
        bool isValid = true;
        std::vector<Function> functions; // standard functions for the interpreter

        bool isDefiningFunction = false;
        std::vector<UserDefinedFunction> UFunctions;

        std::vector<std::string> includedFiles; // For checking if a file has been included

        VAL_RETURNED returnedVal;
        RETURN_TYPE rt;
        bool isReturning = false;

        VariableStack vstack;
        WhileLoopStack wstack;
        bool definingLoop = false;

        /* User declared Variables */
        std::unordered_map<std::string, int> integers; // integers that are declared
        std::unordered_map<std::string, float> floats; // floats that are declared
        std::unordered_map<std::string, std::string> strings; // stringss that are declared
        std::unordered_map<std::string, bool> booleans; // booleans that are declared

        std::unordered_map<std::string, int> int_temp;
        std::unordered_map<std::string, float> float_temp;
        std::unordered_map<std::string, std::string> string_temp;
        std::unordered_map<std::string, bool> bool_temp;

        std::unordered_map<std::string, unsigned int> pointNums;

        Interpreter();

        bool inFunctions(std::string potentialFunc);
        bool inUFunctions(std::string potentialUFunc);
        bool inInts(std::string);
        bool inFloats(std::string);
        bool inStrings(std::string);
        bool inBools(std::string);

        std::vector<std::string> tokenizer(std::string passedInString);
        void callError(std::string errorMsg);
};

extern float evalNum(std::string num, Interpreter * interp);
extern std::string getStrValOf(std::string val, Interpreter * interp);
extern void proccessLine(std::string line);
extern bool getBooleanValOf(std::string * args, Interpreter * interp, int argc);