#pragma once
// For number, string. and boolean evaluation

#include "slugInterpreter.h"

// Takes of the front character of a string
// Used for taking off the characters that signify what type of token they are
// like " ( and $
extern std::string takeOffFrontChar(std::string str);

// Operates on a float that is passed in, used for math evaluations
extern void operateOnAns(float * ans, char op, float num, Interpreter * interp);

// Checks if the character passed in is an operator
extern bool isOp(char op);

// The stack used for the shunting yard algorithm
class NumberStack{
    private:
        std::vector<float> stack; // The actual stack
    public:
        void push(std::string val, Interpreter * interp); // Pushing onto the stack
        inline void push(float val){ // Pushing onto the stack
            stack.push_back(val);
        }
        float pop(){ // Popping off of the stack
            float temp = stack.back();
            stack.pop_back();
            return temp;
        }
};

// For tokenizing expressions
extern std::vector<std::string> tinyTokenizer(std::string s);

// Getting the numerical value of whatever's been passed in
extern float evalNum(std::string num, Interpreter * interp);

// For the $ "String1" "String2" $ concatenation
class StringTokenization{
    public:
        std::vector<std::string> stringTokenizer(std::string val){
            std::vector<std::string> tokens;
            std::string token;
            bool quote = false;
            for (unsigned int i = 0; i < val.length(); i++){
                if (val[i] != ' ' && !quote){
                    if (val[i] == '"'){
                        token += '"';
                        quote = true;
                    } else {
                        token += val[i];
                    }
                } else if (quote){
                    if (val[i] != '"'){
                        token += val[i];
                    } else {
                        quote = false;
                    }
                } else {
                    if (token != ""){
                        tokens.push_back(token);
                        token = "";
                    }
                }
            }
            if (token != ""){
                tokens.push_back(token);
                token = "";
            }
            return tokens;
        }

        std::string EvalStringExpression(std::string val, Interpreter * interp);
};

// Getting the string value of whatever has been passed in
extern std::string getStrValOf(std::string val, Interpreter * interp);

// Evaluating a boolean expression
extern bool evalBool(std::string * args, Interpreter * interp);

// Evaluating a complex string expression (and, or)
extern bool getBooleanValOf(std::string * args, Interpreter * interp, int argc);