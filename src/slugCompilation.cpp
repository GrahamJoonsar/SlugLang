#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include "slugInterpreter.h"
#include "slugCompilation.h"

std::unordered_map<std::string, std::string (*)(std::vector<std::string>, Interpreter*)> compedLines = {};

std::vector<std::string> stringTokenizer(std::string val, Interpreter * interp){
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

bool isValidNumeric(char test){
    return isalnum(test) || test == '_' || test == '.' || test == '-';
}

std::string getCPPValOf(std::string s, Interpreter * interp){
    if (s[0] == '"'){ // Literal
        return s + '"';
    } else if (s[0] == '$'){ // Concatenation
        auto tokens = stringTokenizer(s.substr(1, s.length() - 1), interp);
        std::string base = getCPPValOf(tokens[0], interp);
        for (int i = 1; i < tokens.size(); i++){
            base += "+" + getCPPValOf(tokens[i], interp);
        }
        return base;
    } else if (s[0] == '('){
        s = s.substr(1, s.length() - 2);
        std::string left = "";
        std::string right = "";
        std::string trueStr = "";
        int parens = 0;
        bool parensSeen = false;
        for (int i = 0; i < s.length(); i++){
            if (s[i] != '^'){ // Power operator
                for (int j = i-1; true; j--){
                    if (s[j] == ')'){
                        parens++;
                        parensSeen = true;
                        left += ')';
                    } else if (s[j] == '('){
                        parens--;
                        left += '(';
                    } else {
                        if (parensSeen && parens == 0){
                            break;
                        }
                        if ((isValidNumeric(s[j]) || parens != 0) && j != 0){
                            left += s[j];
                        } else {
                            break;
                        }
                    }
                }
                parensSeen = false;
                parens = 0;
                for (int j = i+1; true; j++){
                    i = j;
                    if (s[j] == ')'){
                        parens--;
                        right += ')';
                    } else if (s[j] == '('){
                        parens++;
                        parensSeen = true;
                        right += '(';
                    } else {
                        if (parensSeen && parens == 0){
                            break;
                        }
                        if ((isValidNumeric(s[j]) || parens != 0) && j != s.length()-1){
                            right += s[j];
                        } else {
                            break;
                        }
                    }
                }
                std::cout << "L: " << left << std::endl;
                std::cout << "R: " << right << std::endl;
                trueStr = trueStr.substr(0, trueStr.length() - (1+left.length()));
                trueStr += "powf(" + left + "," + right + ")";
            } else {
                trueStr += s[i];
            }
        }
        return trueStr;
    } else if (s == "and"){
        return "&&";
    } else if (s == "or"){
        return "||";
    } else { // Variable name
        return s;
    }
}

// Outputting to the console
std::string printComp(std::vector<std::string> tokens, Interpreter * interp){
    return "std::cout << " + getCPPValOf(tokens[1], interp) + ";";
}

std::string printlnComp(std::vector<std::string> tokens, Interpreter * interp){
    return "std::cout << " + getCPPValOf(tokens[1], interp) + " << std::endl;";
}

std::string printfComp(std::vector<std::string> tokens, Interpreter * interp){
    std::string temp = "std::cout << ";
    for (int i = 1; i < tokens.size(); i++){
        temp += getCPPValOf(tokens[i], interp) + " << ";
    }
    return temp + "std::endl;";
}

// Declaring variables
std::string intComp(std::vector<std::string> tokens, Interpreter * interp){
    return "int " + tokens[1] + " = " + tokens[2] + ";";
}

std::string floatComp(std::vector<std::string> tokens, Interpreter * interp){
    return "float " + tokens[1] + " = (float)" + getCPPValOf(tokens[2], interp) + ";";
}

std::string stringComp(std::vector<std::string> tokens, Interpreter * interp){
    return "std::string " + tokens[1] + " = " + getCPPValOf(tokens[2], interp) + ";";
}

std::string boolComp(std::vector<std::string> tokens, Interpreter * interp){
    std::string temp = "bool " + tokens[1] + " = ";
    for (int i = 2; i < tokens.size(); i++){
        temp += getCPPValOf(tokens[i], interp);
    }
    return temp + ';';
}

// Setting (assignment)
std::string setAndSetmComp(std::vector<std::string> tokens, Interpreter * interp){
    std::string temp = tokens[1] + " = ";
    for (int i = 2; i < tokens.size(); i++){
        temp += getCPPValOf(tokens[i], interp);
    }
    return temp + ';';
}

std::string incrAndDecrComp(std::vector<std::string> tokens, Interpreter * interp){
    if (tokens[0] == "incr"){
        return tokens[1] + "++;";
    } else { // decr
        return tokens[1] + "--;";
    }
}

std::string readComp(std::vector<std::string> tokens, Interpreter * interp){
    if (tokens[0] == "readInt" || tokens[0] == "readFloat"){
        return "std::cin >> " + tokens[1] + ';';
    } else { // readStr
        std::string temp = "while(std::getline(std::cin, " + tokens[1] + ")){if (";
        return temp + tokens[1] + " != \"\"){break;}}";
    }
}

std::string whileComp(std::vector<std::string> tokens, Interpreter * interp){
    std::string temp = "while(";
    for (int i = 1; i < tokens.size(); i++){
        temp += getCPPValOf(tokens[i], interp);
    }
    return temp + "){";
}

std::string forComp(std::vector<std::string> tokens, Interpreter * interp){
    std::string temp = "for(";
    std::vector<std::string> set;
    std::string check;
    std::vector<std::string> action;
    int part = 0;
    for (int i = 1; i < tokens.size(); i++){
        if (tokens[i] == ";"){
            part++;
            continue;
        }
        if (part == 0){
            set.push_back(tokens[i]);
        } else if (part == 1){
            check += getCPPValOf(tokens[i], interp) + ' ';
        } else {
            action.push_back(tokens[i]);
        }
    }
    auto sAction = slugToCpp(action);
    sAction.erase(sAction.size() - 1);
    return temp + slugToCpp(set) + check + ';' + sAction + "){";
}

std::string endAndEndwComp(std::vector<std::string> tokens, Interpreter * interp){
    if (interp->skippedBraces > 0){
        interp->skippedBraces--;
        return "";
    }
    return "}";
}

// String ops
std::string concatComp(std::vector<std::string> tokens, Interpreter * interp){
    std::string temp = tokens[1] + " = " + tokens[1];
    for (int i = 2; i < tokens.size(); i++){
        temp += '+' + getCPPValOf(tokens[i], interp);
    }
    return temp + ';';
}

std::string reverseStrComp(std::vector<std::string> tokens, Interpreter * interp){
    return "std::reverse(" + tokens[1] + ".begin(), " + tokens[1] + ".end());";
}

std::string getchComp(std::vector<std::string> tokens, Interpreter * interp){
    return tokens[3] + " = " + tokens[1] + '[' + tokens[2] + "];";
}

std::string setchComp(std::vector<std::string> tokens, Interpreter * interp){
    return tokens[1] + '[' + tokens[2] + "] = " + getCPPValOf(tokens[3], interp) + "[0];";
}

// Conditionals
std::string ifComp(std::vector<std::string> tokens, Interpreter * interp){
    std::string temp = "if(";
    for (int i = 1; i < tokens.size(); i++){
        temp += getCPPValOf(tokens[i], interp);
    }
    return temp + "){";
}

std::string elseifComp(std::vector<std::string> tokens, Interpreter * interp){
    std::string temp = "else if(";
    for (int i = 1; i < tokens.size(); i++){
        temp += getCPPValOf(tokens[i], interp);
    }
    return temp + "){";
}

std::string elseComp(std::vector<std::string> tokens, Interpreter * interp){
    return "else{";
}

std::string makeListComp(std::vector<std::string> tokens, Interpreter * interp){
    std::string temp = "";
    switch(tokens[2][1]){
        case 'i':
            temp += "int";
            break;
        case 'f':
            temp += "float";
            break;
        case 's':
            temp += "std::string";
            break;
        case 'b':
            temp += "bool";
            break;
    }
    return temp + ' ' + tokens[1] + '[' + tokens[3] + "];";
}

// misc
std::string systemComp(std::vector<std::string> tokens, Interpreter * interp){
    return "system(" + getCPPValOf(tokens[1], interp) + ");";
}

std::string quitComp(std::vector<std::string> tokens, Interpreter * interp){
    return "exit(0);";
}

std::string breakComp(std::vector<std::string> tokens, Interpreter * interp){
    return "break;";
}

// For functions that can't be properly implemented in c++
std::string emptyComp(std::vector<std::string> tokens, Interpreter * interp){
    return "";
}

extern void initCompilation(){
    // Output
    compedLines.insert({"print", printComp});
    compedLines.insert({"println", printlnComp});
    compedLines.insert({"printf", printfComp});
    // Variable declaration and assignment
    compedLines.insert({"int", intComp});
    compedLines.insert({"float", floatComp});
    compedLines.insert({"string", stringComp});
    compedLines.insert({"bool", boolComp});
    compedLines.insert({"set", setAndSetmComp});
    compedLines.insert({"setm", setAndSetmComp});
    // Getting input
    compedLines.insert({"readInt", readComp});
    compedLines.insert({"readFloat", readComp});
    compedLines.insert({"readStr", readComp});
    // Math
    compedLines.insert({"incr", incrAndDecrComp});
    compedLines.insert({"decr", incrAndDecrComp});
    // Loops
    compedLines.insert({"while", whileComp});
    compedLines.insert({"for", forComp});
    compedLines.insert({"end", endAndEndwComp});
    compedLines.insert({"endw", endAndEndwComp});
    compedLines.insert({"break", breakComp});
    // Conditionals
    compedLines.insert({"if", ifComp});
    compedLines.insert({"elseif", elseifComp});
    compedLines.insert({"else", elseComp});
    // String ops
    compedLines.insert({"concat", concatComp});
    compedLines.insert({"reverseStr", reverseStrComp});
    compedLines.insert({"getch", getchComp});
    compedLines.insert({"setch", setchComp});
    // List creation
    compedLines.insert({"makeList", makeListComp});
    // System Commands
    compedLines.insert({"system", systemComp});
    compedLines.insert({"quit", quitComp});
    // Uncompilable
    compedLines.insert({"slug", emptyComp});
    compedLines.insert({"exec", emptyComp});
    compedLines.insert({"getType", emptyComp});
    compedLines.insert({"delete", emptyComp});
    compedLines.insert({"remember", emptyComp});
    compedLines.insert({"forget", emptyComp});
}