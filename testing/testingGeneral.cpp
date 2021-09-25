#include <iostream>
#include <string>
#include <cmath>
int main(void){
    
    
    int width = 100;
    int height = 200;
    
    
    
    std::string start = " ";
    for(int i = 0;i < width-1 ;i++){
    start = start+" ";
}    
    
    std::string board[200];
    
    
    
    board[0] = start+"X ";
    
    for(int i = 1;i < height ;i++){
    board[i] = start+"  ";
}    
    
    
    
    std::cout << "NEW BOARD" << std::endl;
    for(int i = 0;i < height-1 ;i++){
    for(int j = 1;j <= width ;j++){
    std::string left = "";
    std::string right = "";
    std::string current = "";
    left = board[i][(j-1)];
    right = board[i][(j+1)];
    current = board[i][j];
    if(current=="X"){
    int aliveNeighbors = 0;
    if(left=="X"){
    aliveNeighbors++;
}    if(right=="X"){
    aliveNeighbors++;
}    if(aliveNeighbors<=1){
    board[(i+1)][j] = "X"[0];
}    else{
    board[(i+1)][j] = " "[0];
}}    else{
    int aliveNeighbors = 0;
    if(left=="X"){
    aliveNeighbors++;
}    if(right=="X"){
    aliveNeighbors++;
}    if(aliveNeighbors>0){
    board[(i+1)][j] = "X"[0];
}    else{
    board[(i+1)][j] = " "[0];
}}}    
    std::cout << "#"+board[i]+"#" << std::endl;
}    
    
    
    
}
