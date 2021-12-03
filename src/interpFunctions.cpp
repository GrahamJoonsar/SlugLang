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

std::string getTrueIndex(std::string variable, Interpreter * interp){
    auto trueStr = variable.substr(1, variable.length() - 2);
    bool brace = false;
    int bnum = 0;
    std::string result = "";
    std::string temp = "";
    for (int i = 0; i < trueStr.length(); i++){
        if (trueStr[i] != '[' && !brace){
            result += trueStr[i];
        } else if (trueStr[i] == '['){
            temp += '[';
            brace = true;
            bnum++;
        } else if (trueStr[i] == ']'){
            temp += ']';
            bnum--;
            if (bnum == 0){
                brace = false;
                result += std::to_string((int)evalNum(getTrueIndex(temp, interp), interp));
                temp = "";
            }
        }
    }
    return std::to_string((int)evalNum(result, interp));
}

// Very complicated tokenization
std::vector<std::string> Interpreter::tokenizer(std::string passedInString, bool compiling){
    if (passedInString == ""){return {};}
    if (remembering && rememberedLines.find(passedInString) != rememberedLines.end()){
        curlyBraceNum = rememberedLines[passedInString].tablevel;
        return rememberedLines[passedInString].tokens;
    }
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
        if (passedInString[i] == '[' && !isString && !compiling){ 
            if (!literalizing){
                variable = '[';
            } else {
                variable += '[';
            }
            literalizing = true;
            braceNum++;
        } else if (literalizing){
            if (passedInString[i] == ']'){
                braceNum--;
                variable += ']';
                if (braceNum == 0){
                    literalizing = false;
                    token += getTrueIndex(variable, this);
                }
            } else if (passedInString[i] == '['){
                braceNum++;
                variable += '[';
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
                if (remembering && passedInString.find('[') == std::string::npos){
                    rememberedLines[passedInString] = LineInfo{tokens, tabLevel};
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

    if (remembering && passedInString.find('[') == std::string::npos){
        rememberedLines[passedInString] = LineInfo{tokens, tabLevel};
    }

    curlyBraceNum = tabLevel;
    return tokens;
}

std::vector<Token> Interpreter::new_tokenizer(std::string line){
    std::vector<Token> tokens; // The list of tokens that we will return
    std::vector<std::string> unclassifiedTokens = tokenizer(line, false);

    for (auto t: unclassifiedTokens){
        if (t[0] == '"' || t[0] == '$'){ // String literal or concatenation grouping
            tokens.push_back({t, Token::STRING});
        } else if (/*Number literal*/(t[0] == '-' || isdigit(t[0])) || /*Mathematical expression*/ (t[0] == '(' && t[t.length()-1] == ')')){
            tokens.push_back({t, Token::NUMBER});
        } else { // op, variable name, or function call
            tokens.push_back({t, Token::OTHER});
        }
    }
    return tokens;
}
