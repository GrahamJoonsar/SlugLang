#include "evaluations.h"
#include <cmath>

// Takes of the front character of a string
// Used for taking off the characters that signify what type of token they are
// like " ( and $
std::string takeOffFrontChar(std::string str){
    return str.substr(1, str.length() - 1);
}

// Operates on a float that is passed in, used for math evaluations
void operateOnAns(float * ans, char op, float num, Interpreter * interp){
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
        case '^':
            *ans = powf(*ans, num);
            break;
        default:
            break;
    }
}

// Checks if the character passed in is an operator
bool isOp(char op){
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '%' || op == '^';
}

// For tokenizing expressions
std::vector<std::string> tinyTokenizer(std::string s){
    bool alphanum = true; // Currently aplhanumeric
    bool parens = false; // Inside of parentheses
    bool brace = false;
    int braceNum = 0;
    int parensNum = 0;
    std::vector<std::string> tokens; // Tokens of the expression
    std::string current_token = ""; // The current token
    s = s.substr(1, s.size() - 2); // Removing outer parentheses
    std::string truStr = "";
    for (unsigned int i = 0; i < s.size(); i++){if(s[i] != ' '){truStr+=s[i];}} // Removing spaces
    for (unsigned int i = 0; i < truStr.size(); i++){
        if ((isalnum(truStr[i]) || truStr[i] == '.' || truStr[i] == '_') && !parens && !brace){// var or number literal
            if (!alphanum){ // Token changed
                alphanum = true;
                tokens.push_back(current_token);
                current_token = "";
            }
            current_token += truStr[i];
        } else if (isOp(truStr[i]) && !parens && !brace){ // operator or parentheses
            if (alphanum){
                alphanum = false;
                tokens.push_back(current_token);
                current_token = "";
            }
            current_token += truStr[i];
        } else if (truStr[i] == '(' && !brace){ // Start parentheses
            if (parensNum == 0){
                tokens.push_back(current_token);
                current_token = "";
            }
            parensNum++;
            parens = true;
            current_token += '(';
        } else if (truStr[i] == ')' && !brace){ // End parentheses
            parensNum--;
            current_token += ')';
            if (parensNum == 0){
                parens = false;
                if (current_token != ""){tokens.push_back(current_token);}
                current_token = "";
            }
        } else if (truStr[i] == '[' && !parens){ // Start parentheses
            if (braceNum == 0){
                tokens.push_back(current_token);
                current_token = "";
            }
            braceNum++;
            brace = true;
            current_token += '[';
        } else if (truStr[i] == ']' && !parens){ // End parentheses
            braceNum--;
            current_token += ']';
            if (braceNum == 0){
                brace = false;
                current_token.pop_back();
                if (current_token != ""){tokens.push_back(current_token);}
                current_token = "";
            }
        } else if (parens || brace){
            current_token += truStr[i];
        }
    }
    if (current_token != ""){
        tokens.push_back(current_token);
    }
    return tokens;
}

// Getting the numerical value of whatever's been passed in
extern float evalNum(std::string num, Interpreter * interp){
    if (!isdigit(num[0]) && num[0] != '-' && num[0] != '(' && num[0] != '['){ // not a number literal and not a negative number literal
        if (interp->inInts(num)){
            return interp->integers[num];
        } else if (interp->inFloats(num)){
            return interp->floats[num];
        }
    } else if (num[0] == '('){ // Expression
        // Shunting yard algorithm
        // https://en.wikipedia.org/wiki/Shunting-yard_algorithm
        NumberStack s;
        auto tokens = tinyTokenizer(num);
        std::vector<std::string> polishTokens;
        // Convert to reverse polish
        for (std::vector<std::string>::size_type i = 0; i < tokens.size(); i++){ // Looping through tokens
            if (isOp(tokens[i][0])){ // Operator
                auto temp = tokens[i]; // Store the operator
                i++;
                polishTokens.push_back(tokens[i]); // The value
                polishTokens.push_back(temp); // The operator
            } else if (tokens[i] != ""){ // Removing blanks that generate
                polishTokens.push_back(tokens[i]);
            }
        }

        for (auto t : polishTokens){ // Looping through
            if (isOp(t[0])){
                float a = s.pop();
                float b = s.pop();
                operateOnAns(&b, t[0], a, interp);
                s.push(b);
            } else {
                s.push(t, interp);
            }
        }
        return s.pop();
    } else if (num[0] == '['){// Function
        auto temp = interp->argcountForFunc;
        auto temp1 = interp->funcNum;
        auto temp2 = interp->argsPassedIn;
        auto temp3 = interp->curlyBraceNum;
        proccessLine(takeOffFrontChar(num)); // Run the function
        interp->argcountForFunc = temp;
        interp->funcNum = temp1;
        interp->argsPassedIn = temp2;
        interp->curlyBraceNum = temp3;
        switch (interp->rt){
            case RETURN_ENUM::RETURN_TYPE::INT:
                return interp->returnedVal.i;
                break;
            case RETURN_ENUM::RETURN_TYPE::FLOAT:
                return interp->returnedVal.i;
                break;
            case RETURN_ENUM::RETURN_TYPE::STRING:
            case RETURN_ENUM::RETURN_TYPE::BOOL:
                interp->callError("Cannot convert to number");
                break;
        }
    } else { // Is a number literal
        return std::stof(num, nullptr);
    }
    //std::cout << "EVAL " << num << std::endl;
    return 0; // In case all else fails
}

// Pushing onto the stack
void NumberStack::push(std::string val, Interpreter * interp){
    stack.push_back(evalNum(val, interp));
}


// Getting the string value of whatever has been passed in
extern std::string getStrValOf(std::string val, Interpreter * interp){
    if (interp->inInts(val)){ // integer var
        return std::to_string(interp->integers[val]);
    } else if (interp->inFloats(val)){ // float var
        return std::to_string(interp->floats[val]);
    } else if (interp->inStrings(val)){ // string var
        return interp->strings[val];
    } else if (isdigit(val[0]) || val[0] == '-'){ // Number literal
        return val;
    } else if (val[0] == '('){ // Expression
        return std::to_string(evalNum(val, interp));
    } else if (val[0] == '"'){ // string literal
        return takeOffFrontChar(val);
    } else if (val[0] == '['){ // string returned from a function
        auto temp = interp->argcountForFunc;
        auto temp1 = interp->funcNum;
        auto temp2 = interp->argsPassedIn;
        auto temp3 = interp->curlyBraceNum;
        proccessLine(takeOffFrontChar(val)); // Run the function
        interp->argcountForFunc = temp;
        interp->funcNum = temp1;
        interp->argsPassedIn = temp2;
        interp->curlyBraceNum = temp3;
        std::string valReturned;
        switch (interp->rt){ // Get whatever was returned
            case RETURN_ENUM::INT:
                valReturned = std::to_string(interp->returnedVal.i);
                break;
            case RETURN_ENUM::FLOAT:
                valReturned = std::to_string(interp->returnedVal.f);
                break;
            case RETURN_ENUM::STRING:
                valReturned = interp->returnedVal.s;
                break;
            case RETURN_ENUM::BOOL: // Why
                valReturned = std::to_string(interp->returnedVal.b);
                break;
        }
        return valReturned;
    } else if (val[0] == '$'){ // string literal
        StringTokenization s;
        return s.EvalStringExpression(val, interp);
    } else {
        interp->callError("Error converting '" + val + "' to string");
        return "";
    }
    return "";
}

std::string StringTokenization::EvalStringExpression(std::string val, Interpreter * interp){
    std::string base = "";
    auto tokens = stringTokenizer(takeOffFrontChar(val));
    for (auto t : tokens){
        base += getStrValOf(t, interp);
    }
    return base;
}

// Evaluating a boolean expression
bool evalBool(std::string * args, Interpreter * interp){
    if (interp->inBools(args[0])){
        return interp->booleans[args[0]];
    }
    if (args[0][0] == '"' || interp->inStrings(args[0]) || args[0][0] == '$'){ // Comparison of string literal or string var
        std::string a = getStrValOf(args[0], interp);
        std::string b = getStrValOf(args[2], interp);
        if (args[1] == "=="){
            return a == b;
        } else if (args[1] == "<"){
            return a < b;
        } else if (args[1] == ">"){
            return a > b;
        } else if (args[1] == "!="){
            return a != b;
        }
    } else if (/*Number Literal*/(args[0][0] == '-' || isdigit(args[0][0])) || /*A number var*/(interp->inInts(args[0]) || interp->inFloats(args[0])) || args[0][0] == '('){
        float a = evalNum(args[0], interp);
        float b = evalNum(args[2], interp);
        if (args[1] == "=="){
            return a == b;
        } else if (args[1] == "<"){
            return a < b;
        } else if (args[1] == ">"){
            return a > b;
        } else if (args[1] == "<="){
            return a <= b;
        } else if (args[1] == ">="){
            return a >= b;
        } else if (args[1] == "!="){
            return a != b;
        }
    } else { // Probably function
        if (args[0][0] == '[' || args[2][0] == '['){
            RETURN_ENUM::RETURN_TYPE type;
            if (args[0][0] == '['){ // Getting the type to be compared
                auto temp = interp->argcountForFunc;
                auto temp1 = interp->funcNum;
                auto temp2 = interp->argsPassedIn;
                auto temp3 = interp->curlyBraceNum;
                proccessLine(takeOffFrontChar(args[0])); // Run the function
                interp->argcountForFunc = temp;
                interp->funcNum = temp1;
                interp->argsPassedIn = temp2;
                interp->curlyBraceNum = temp3;
                type = interp->rt;
            } else {
                auto temp = interp->argcountForFunc;
                auto temp1 = interp->funcNum;
                auto temp2 = interp->argsPassedIn;
                auto temp3 = interp->curlyBraceNum;
                proccessLine(takeOffFrontChar(args[2])); // Run the function
                interp->argcountForFunc = temp;
                interp->funcNum = temp1;
                interp->argsPassedIn = temp2;
                interp->curlyBraceNum = temp3;
                type = interp->rt;
            }
            if (type == RETURN_ENUM::RETURN_TYPE::STRING){
                std::string a = getStrValOf(args[0], interp);
                std::string b = getStrValOf(args[2], interp);
                if (args[1] == "=="){
                    return a == b;
                } else if (args[1] == "<"){
                    return a < b;
                } else if (args[1] == ">"){
                    return a > b;
                } else if (args[1] == "!="){
                    return a != b;
                }
            } else if (type == RETURN_ENUM::RETURN_TYPE::INT || type == RETURN_ENUM::RETURN_TYPE::FLOAT){
                float a = evalNum(args[0], interp);
                float b = evalNum(args[2], interp);
                if (args[1] == "=="){
                    return a == b;
                } else if (args[1] == "<"){
                    return a < b;
                } else if (args[1] == ">"){
                    return a > b;
                } else if (args[1] == "<="){
                    return a <= b;
                } else if (args[1] == ">="){
                    return a >= b;
                } else if (args[1] == "!="){
                    return a != b;
                }
            } else { // bool
                return interp->returnedVal.b;
            }
        }
    }
    return false;
}

// Evaluating a complex string expression (and, or)
extern bool getBooleanValOf(std::string * args, Interpreter * interp, int argc){
    bool temp = false;
    std::vector<bool> parts;
    std::string andOr;
    for (int i = 0; i < argc; i++){
        if (args[i] == "and"){ // and operation
            andOr += "a";
        } else if (args[i] == "or"){ // or operation
            andOr += "o";
        } else if (interp->inBools(args[i])){
            parts.push_back(interp->booleans[args[i]]);
        } else if (args[i][0] == '['){ // Function that returns bool
            auto temp = interp->argcountForFunc;
            auto temp1 = interp->funcNum;
            auto temp2 = interp->argsPassedIn;
            auto temp3 = interp->curlyBraceNum;
            proccessLine(takeOffFrontChar(args[i])); // Run the function
            interp->argcountForFunc = temp;
            interp->funcNum = temp1;
            interp->argsPassedIn = temp2;
            interp->curlyBraceNum = temp3;
            parts.push_back(interp->returnedVal.b);
        } else { // Evaluating the boolean expression
            std::string sTemp[3] = {args[i]};
            i++;
            sTemp[1] = args[i];
            i++;
            sTemp[2] = args[i];
            parts.push_back(evalBool(sTemp, interp));
        }
    }
    if (parts.size() == 1){
        return parts[0];
    }
    temp = parts[0];
    for (unsigned int i = 1; i < parts.size(); i++){
        if (andOr[i - 1] == 'a'){ // and
            temp = temp && parts[i];
        } else if (andOr[i - 1] == 'o'){ // or
            temp = temp || parts[i];
        }
    }
    return temp;
}