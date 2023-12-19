#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {

    ifstream inFS;
    inFS.open(fileName);
    if(!inFS.is_open()){ //can't be opened
        inFS.close();
        // cout<<"hi73"<<endl;
        return nullptr;
    }
    inFS>>maxRow>>maxCol;
    if(inFS.fail()){
        // cout<<"hi22"<<endl;
        return nullptr;
    }
    if(maxRow>=999999||maxCol>=999999||maxRow<=0||maxCol<=0){ //columns and rows less than or equal to 0, rows and columns greater than 999999
        inFS.close();
        // cout<<"hi45"<<endl;
        return nullptr;
    }
    char symbol;
    inFS>>player.row>>player.col;
    if(inFS.fail()){
        // cout<<"hi22"<<endl;
        return nullptr;
    }
    if(player.row>maxRow||player.col>maxCol||player.row<0||player.col<0){ //player row/column is negative, not on grid
        // cout<<"hi2"<<endl;
        return nullptr;
    }
    if(maxRow>=INT32_MAX/maxCol){
        return nullptr;
    }
    char** map = createMap(maxRow,maxCol);
    if (map==nullptr){
        // cout<<"hi1"<<endl;
        deleteMap(map,maxRow);
        return nullptr;
    }
    bool poss = false; //doesnt have tile or door
    for(int row=0;row<maxRow;row++){
        for(int col=0;col<maxCol;col++){
            inFS >> symbol;
            if(inFS.fail()){
                // cout<<"hi22"<<endl;
                return nullptr;
            }
            if(symbol==TILE_DOOR||symbol==TILE_EXIT){
                map[row][col]=symbol;
                poss=true;
            }
            else if(symbol==TILE_OPEN||symbol==TILE_TREASURE||symbol==TILE_AMULET||
            symbol==TILE_MONSTER||symbol==TILE_PILLAR){ //not valid character
                map[row][col] = symbol;
            }
            else{
                inFS.close();
                deleteMap(map,maxRow);
                // cout<<"hi3"<<endl;
                return nullptr;
            }
        }
    }
    if(!poss){ //bad if there is no door or exit
        inFS.close();
        deleteMap(map,maxRow);
        return nullptr;
    }
    if(map[player.row][player.col]==TILE_OPEN){ //if player's position is at tile open
        map[player.row][player.col] = TILE_PLAYER;
    }
    else{
        inFS.close();
        deleteMap(map,maxRow);
        return nullptr;
    }
    char extra;
    inFS>>extra;
    if(!inFS.fail()){ //good if there is no extra symbols or values
        inFS.close();
        deleteMap(map,maxRow);
        return nullptr;
    }
    inFS.close();
    return map;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if(input=='w'){
        nextRow -= 1;
    }
    else if(input=='a'){
        nextCol -= 1;
    }
    else if(input=='s'){
        nextRow += 1;
    }
    else if(input=='d'){
        nextCol += 1;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    char** map = new char*[maxRow];
    for(int row =0;row<maxRow;row++){
        map[row] = new char[maxCol];
    }
    for(int row =0; row<maxRow;row++){
        for(int col =0;col<maxCol;col++){
            map[row][col] = TILE_OPEN;
        }
    }
    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if(map!=nullptr){
        for(int i =0;i<maxRow;i++){
            delete[] map[i]; //deletes each row array
        }
    }
    delete[] map; //deletes whole map array
    map = nullptr;
    maxRow = 0;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    int new_row = maxRow*2;
    int new_col = maxCol*2;
    if(map==nullptr||new_row >= INT32_MAX||new_col>=INT32_MAX||new_row<=0||new_col<=0){
        return nullptr;
    }
    char** new_map = createMap(new_row,new_col);
    //A
    for(int i =0;i<maxRow;i++){
        for(int j=0;j<maxCol;j++){
            new_map[i][j] = map[i][j];
        }
    }
    //B
    int b_col_start = (maxCol);
    for(int i =0; i<maxRow;i++){
        for(int j=b_col_start;j<new_col;j++){
            if(map[i][j-b_col_start]!='o'){
                new_map[i][j] = map[i][j-b_col_start];
            }

            else{
                new_map[i][j] = TILE_OPEN;
            }
        }
    }
    //C
    int c_row_start = (maxRow);
    for(int i=c_row_start;i<new_row;i++){
        for(int j=0;j<maxCol;j++){
            if(map[i-c_row_start][j]!='o'){
                new_map[i][j] = map[i-c_row_start][j];
            }
            else{
                new_map[i][j] = TILE_OPEN;
            }
        }
    }
    //D
    int d_row_start = (maxRow);
    int d_col_start = (maxCol);
    for(int i=d_row_start;i<new_row;i++){
        for(int j=d_col_start;j<new_col;j++){
            if(map[i-d_row_start][j-d_col_start]!='o'){
                new_map[i][j] = map[i-d_row_start][j-d_col_start];
            }
            else{
                new_map[i][j] = TILE_OPEN;
            }
        }
    }
    deleteMap(map,maxRow);
    maxRow = new_row;
    maxCol = new_col;
    return new_map;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    // cout<<"hi0";
    if(nextRow>=maxRow||nextCol>=maxCol||nextRow<0||nextCol<0){//if at border, monster, or pillar; or invalid numbers
    // cout<<"hi1";
        return STATUS_STAY;
    }
    else if(map[nextRow][nextCol]==TILE_PILLAR||map[nextRow][nextCol]==TILE_MONSTER){
        // cout<<"hi2";
        return STATUS_STAY;
    }
    else if(map[nextRow][nextCol]==TILE_OPEN){
        // cout<<"hi3";
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol]= TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_MOVE;
    }
    else if(map[nextRow][nextCol]==TILE_TREASURE){ //at treasure
    // cout<<"hi4";
        player.treasure += 1;
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol]= TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_TREASURE;
    }
    else if(map[nextRow][nextCol]==TILE_AMULET){ //at amulet
    // cout<<"hi5";
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol]= TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_AMULET;
    }
    else if(map[nextRow][nextCol]==TILE_DOOR){ //at door
    // cout<<"hi6";
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol]= TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_LEAVE;
    }
    else if(map[nextRow][nextCol]==TILE_EXIT){ //at exit
    // cout<<"hi7";
        if(player.treasure>=1){
            // cout<<"hi8";
            map[player.row][player.col] = TILE_OPEN;
            map[nextRow][nextCol]= TILE_PLAYER;
            player.row = nextRow;
            player.col = nextCol;
            return STATUS_ESCAPE;
        }
        else{
            // cout<<"hi9";
            // nextRow = player.row;
            // nextCol = player.col;
            return STATUS_STAY;
        }
    }
    // cout<<"hi10";
    return STATUS_STAY;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {

    //up
    for(int i = player.row-1;i!=-1;i--){
        char current_location = map[i][player.col];
        if(current_location==TILE_MONSTER){
            map[i][player.col] = TILE_OPEN;
            map[i+1][player.col] = TILE_MONSTER;
        }
        else if(current_location==TILE_PILLAR){
            break;
        }
    }
    //down
    for(int i = player.row+1;i!=maxRow;i++){
        char current_location = map[i][player.col];
        if(current_location==TILE_MONSTER){
            map[i][player.col] = TILE_OPEN;
            map[i-1][player.col] = TILE_MONSTER;
        }
        else if(current_location==TILE_PILLAR){
            break;
        }
    }
    //left
    for(int i = player.col-1;i!=-1;i--){
        char current_location = map[player.row][i];
        if(current_location==TILE_MONSTER){
            map[player.row][i] = TILE_OPEN;
            map[player.row][i+1] = TILE_MONSTER;
        }
        else if(current_location==TILE_PILLAR){
            break;
        }
    }
    //right
    for(int i = player.col+1;i!=maxCol;i++){
        char current_location = map[player.row][i];
        if(current_location==TILE_MONSTER){
            map[player.row][i] = TILE_OPEN;
            map[player.row][i-1] = TILE_MONSTER;
        }
        else if(current_location==TILE_PILLAR){
            break;
        }
    }
    if(map[player.row][player.col]==TILE_MONSTER){
        return true;
    }
    return false;
}
