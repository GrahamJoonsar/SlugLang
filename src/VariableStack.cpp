#include "slugInterpreter.h"

ScopedVariables VariableStack::back(){
    return v.back();
}

void VariableStack::push(ScopedVariables sv){
    v.push_back(sv);
}

void VariableStack::pop_back(){
    v.pop_back();
}

unsigned int VariableStack::length(){
    return v.size();
}