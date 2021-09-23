#include <unordered_map>
#include <string>
#include <vector>
#include "slugInterpreter.h"
#include "slugCompilation.h"

extern std::unordered_map<std::string, std::string (*)(std::vector<std::string>, Interpreter*)> compedLines = {};

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

std::string getCPPStrValOf(std::string s, Interpreter * interp){
    if (s[0] == '"'){ // Literal
        return s + '"';
    } else if (s[0] == '$'){ // Concatenation
        std::string base = "";
        auto tokens = stringTokenizer(s.substr(1, s.length() - 1), interp);
        for (auto t : tokens){
            base += getCPPStrValOf(t, interp);
        }
        return base;
    } else { // Variable name
        return s;
    }
}

std::string printComp(std::vector<std::string> tokens, Interpreter * interp){
    return "std::cout << " + getCPPStrValOf(tokens[1], interp) + ";";
}

std::string printlnComp(std::vector<std::string> tokens, Interpreter * interp){
    return "std::cout << " + getCPPStrValOf(tokens[1], interp) + " << std::endl;";
}

extern void initCompilation(){
    compedLines.insert({"print", printComp});
    compedLines.insert({"println", printlnComp});
}