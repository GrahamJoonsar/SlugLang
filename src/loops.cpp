#include "slugInterpreter.h"

WhileLoop WhileLoopStack::back(){
    return v.back();
}

void WhileLoopStack::push(WhileLoop sv){
    v.push_back(sv);
}

void WhileLoopStack::pop_back(){
    v.pop_back();
}

unsigned int WhileLoopStack::length(){
    return v.size();
}

void WhileLoopStack::addl(std::string l){
    v.back().linesOfLoop.push_back(l);
}