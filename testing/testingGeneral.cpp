#include <iostream>
#include <string>
#include <cmath>
int main(void){
    
    
    int width = 150;
    int height = 20000;
    
    
    
    std::string last = "";
    
    std::string start = " ";
    for(int i = 0;i < (width-1) ;i++){
    start = start+" ";
}    
    
    std::string blank = start+"  ";
    
    
    
    last = start+"X ";
    std::string first = blank;
    
    
    std::cout << "NEW BOARD" << std::endl;
    for(int i = 0;i < (height-1) ;i++){
    for(int j = 1;j <= width ;j++){
    std::string left = "";
    std::string right = "";
    std::string current = "";
    left = last[(j-1)];
    right = last[(j+1)];
    current = last[j];
    if(current=="X"){
    int aliveNeighbors = 0;
    if(left=="X"){
    aliveNeighbors++;
}    if(right=="X"){
    aliveNeighbors++;
}    if(aliveNeighbors<=1){
    first[j] = "X"[0];
}    else{
    first[j] = " "[0];
}}    else{
    int aliveNeighbors = 0;
    if(left=="X"){
    aliveNeighbors++;
}    if(right=="X"){
    aliveNeighbors++;
}    if(aliveNeighbors>0){
    first[j] = "X"[0];
}    else{
    first[j] = " "[0];
}}}    
    std::cout << "#"+first+"#" << std::endl;
    last = first;
    first = blank;
}    
    
}
