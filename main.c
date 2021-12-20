/*
 * File:        project_reversi_skeleton.c
 * Written by: Danial Khan
 * 
 * Date: Jan 2021
 */

#include "project_reversi_skeleton.h" // DO NOT modify this line
#include "lab8part2lib.h"
#include <stdio.h>
// =========   Function declaration  ==========

// ========= Function implementation ==========
/*
 * Function:  changeBoard 
 * --------------------
 * Based on given tile, performs a move
 */
void changeBoard(char board[26][26], int n, int row, int col, char colourCheck, int deltaRow, int deltaCol){
    int i, j;
    char oppCol;
    board[row][col] = colourCheck;
    
    if(colourCheck == 'W'){
        oppCol = 'B';
    }
    else{
        oppCol = 'W';
    }
    
    if (checkLegalInDirection(board, n, row, col, oppCol, deltaRow, deltaCol)){
        for (i = row - deltaRow, j = col + deltaCol; positionInBounds(n, i, j); i-= deltaRow, j+= deltaCol){
            if (board[i][j] == 'U'){
                break;
            }
            else if (board[i][j] == colourCheck){
                break;
            }
            board[i][j] = colourCheck;
        }   
    }
}

// ========= Function implementation ==========
/*
 * Function:  moveChecker 
 * --------------------
 * Based on the tile and colour,  all eight directions are checked if there is a possible move
 */
bool moveChecker(char board[][26], int n, char colour, int *i, int *j){
    //Note using pointet *i = row, *j = col --> if valid move, then later added to a list
    int directionTrue[8] = {}, k, counter;
    if (board[*i][*j] == 'U'){
        counter = 1;
        directionTrue[0] = checkLegalInDirection(board, n, *i, *j, colour, 1, 0);   //North
        directionTrue[1] = checkLegalInDirection(board, n, *i, *j, colour, 0, 1);   //East
        directionTrue[2] = checkLegalInDirection(board, n, *i, *j, colour, -1, 0);  //South
        directionTrue[3] = checkLegalInDirection(board, n, *i, *j, colour, 0, -1);  //West
        directionTrue[4] = checkLegalInDirection(board, n, *i, *j, colour, 1, 1);   //NorthEast
        directionTrue[5] = checkLegalInDirection(board, n, *i, *j, colour, -1, -1); //SouthWest
        directionTrue[6] = checkLegalInDirection(board, n, *i, *j, colour, 1, -1);  //NorthWest
        directionTrue[7] = checkLegalInDirection(board, n, *i, *j, colour, -1, 1);  //SouthEast
        for (k = 0; k < 8; k++){
            //If any direction has a move, than tile is valid
            if (directionTrue[k] && counter == 1){
                counter++;
                return true;
            }
        }
    }
    return false;
}

// ========= Function implementation ==========
/*
 * Function:  printBoard 
 * --------------------
 * Print the whole board to the terminal window.
 */
void printBoard(char board[][26], int n) {
    int i, j;
    //print rows
    printf("  ");
    for (i = 0; i < n; i++){
        printf("%c", 'a' + i);
    }
    printf("\n");
    
    //print each elements along with column specifier
    for (i = 0; i < n; i++){
        printf("%c ", 'a' + i);
        for(j=0; j < n; j++){
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    
}

/*
 * Function:  positionInBounds 
 * --------------------
 * Checks whether the specified (row, col) lies within the board dimensions.
 */
bool positionInBounds(int n, int row, int col) {
    //Checking board parameters
    if (row < n && col < n && row >= 0 && col >= 0){
        return true;
    }
    return false;  
}

/*
 * Function:  checkLegalInDirection 
 * --------------------
 * Checks whether (row, col) is a legal position for a tile of colour by “looking” in the direction 
 * specified by deltaRow and deltaCol.
 */
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) {    
    int i, j;
    char colourCheck;
    // opposite colour is what move you are checking
    if (colour == 'B'){
        colourCheck = 'W';
    }
    else{
        colourCheck = 'B';
    }
    
    //Now we can check for the tiles
    if ((board[row - deltaRow][col + deltaCol] == colour) && positionInBounds(n, row - deltaRow, col + deltaCol)){
        for (i = row - deltaRow, j = col + deltaCol; positionInBounds(n, i, j); i-= deltaRow, j+= deltaCol){
            if (board[i][j] == 'U'){
                return false;
            }
            else if (board[i][j] == colourCheck){
                return true;
            }
        } 
    }
    return false;             
}

/*
 * Function:  addToMoveList
 * --------------------
 * Adds available moves to a move list based on colour
 */
void addToMoveList(int nDim, char board[26][26], char moveList[676][2], char colour){
    bool moveDecision;
    int i, j, k = 0;
    
    //empties the list initially
    for(i = 0; i < nDim *nDim ; i++){
        for(j = 0; j < 2 ; j++){
            moveList[i][j] = ' ';
        }
    }
    
    for(i = 0; i < nDim; i++){
        for(j = 0; j < nDim; j++){
            //checking for all available tiles if there is a move
            moveDecision = moveChecker(board, nDim, colour, &i, &j);
            if (moveDecision){
                // if possible move, add it to list
                moveList[k][0] = 'a' + i;
                moveList[k][1] = 'a' + j;
                k++;
            }
        }
    }
}

/*
 * Function:  invalidMoveCheck
 * --------------------
 * Checks if the user inputs an invalid move
 */
bool invalidMoveCheck(int nDim, char playerMove[3], char moveList[676][2], char colour){
    int i;
    
    if (positionInBounds(nDim, playerMove[0] - 97, playerMove[1] - 97)){
        //check if it is an invalid move based on given list
        for (i = 0; i < nDim * nDim; i++){
            if (colour == 'W' && moveList[i][0] == playerMove[0] && moveList[i][1] == playerMove[1]){
                return true;
            }
            if (colour == 'B' && moveList[i][0] == playerMove[0] && moveList[i][1] == playerMove[1]){
                return true;
            }
        }
    }
    return false;
}

/*
 * Function:  makePlayerMove
 * --------------------
 * Takes in user input, and completes the players move if valid
 */
bool makePlayerMove(char board[26][26], int nDim, char moveList[676][2], char colour){
    bool moveDecision;
    char playerMove[3];
    int row = 0, col = 0;
    
    findSmarterMove(board, nDim, colour, &row, &col);
    printf("Testing AI move (row, col): %c%c\n", row + 'a', col + 'a');
    
    changeBoard(board, nDim, row, col, colour, 1, 0);   //North
    changeBoard(board, nDim, row, col, colour, 0, 1);   //East
    changeBoard(board, nDim, row, col, colour, -1, 0);  //South
    changeBoard(board, nDim, row, col, colour, 0, -1);  //West
    changeBoard(board, nDim, row, col, colour, 1, 1);   //NorthEast
    changeBoard(board, nDim, row, col, colour, -1, -1); //SouthWest
    changeBoard(board, nDim, row, col, colour, 1, -1);  //NorthWest
    changeBoard(board, nDim, row, col, colour, -1, 1);  //SouthEast
    return true;
}

/*
 * Function:  checkScore
 * --------------------
 * Will check the score that a specified move will provide
 */
int checkScore(char colour, char board[26][26], int deltaRow, int deltaCol, int n, int row, int col){
    int i, j, score = 0;
    char colourCheck;

    if (colour == 'B'){
        colourCheck = 'W';
    }
    else{
        colourCheck = 'B';
    }
    
    //Now we can check for the tiles
    for (i = row - deltaRow, j = col + deltaCol; positionInBounds(n, i, j); i-= deltaRow, j+= deltaCol){
        if (board[i][j] == 'U' || positionInBounds(n, i, j) == false){
            return 0;
        }
        else if (board[i][j] == colour){
            return score;
        }
        else if (board[i][j] == colourCheck) score++;
    } 
    return 0;
}


/*
 * Function:  makeComputerMove
 * --------------------
 * Creates the computer move (placing on board)
 */
void makeComputerMove(char board[26][26], int nDim, char compColour){
    int compRow, compCol;
    
    makeMove(board, nDim, compColour, &compRow, &compCol);
            
    printf("Computer places %c at %c%c.\n", compColour, compRow, compCol);
      
    //Implement Move
    changeBoard(board, nDim, compRow - 97, compCol - 97, compColour, 1, 0);   //North
    changeBoard(board, nDim, compRow - 97, compCol - 97, compColour, 0, 1);   //East
    changeBoard(board, nDim, compRow - 97, compCol - 97, compColour, -1, 0);  //South
    changeBoard(board, nDim, compRow - 97, compCol - 97, compColour, 0, -1);  //West
    changeBoard(board, nDim, compRow - 97, compCol - 97, compColour, 1, 1);   //NorthEast
    changeBoard(board, nDim, compRow - 97, compCol - 97, compColour, -1, -1); //SouthWest
    changeBoard(board, nDim, compRow - 97, compCol - 97, compColour, 1, -1);  //NorthWest
    changeBoard(board, nDim, compRow - 97, compCol - 97, compColour, -1, 1);  //SouthEast
}

/*
 * Function:  makeMovecopy
 * --------------------
 * Creates move
 */
void makeMoveCopy(char boardCopy[26][26], int nDim, char compColour, char compRow, char compCol){
    //Implement Move
    changeBoard(boardCopy, nDim, compRow - 97, compCol - 97, compColour, 1, 0);   //North
    changeBoard(boardCopy, nDim, compRow - 97, compCol - 97, compColour, 0, 1);   //East
    changeBoard(boardCopy, nDim, compRow - 97, compCol - 97, compColour, -1, 0);  //South
    changeBoard(boardCopy, nDim, compRow - 97, compCol - 97, compColour, 0, -1);  //West
    changeBoard(boardCopy, nDim, compRow - 97, compCol - 97, compColour, 1, 1);   //NorthEast
    changeBoard(boardCopy, nDim, compRow - 97, compCol - 97, compColour, -1, -1); //SouthWest
    changeBoard(boardCopy, nDim, compRow - 97, compCol - 97, compColour, 1, -1);  //NorthWest
    changeBoard(boardCopy, nDim, compRow - 97, compCol - 97, compColour, -1, 1);  //SouthEast
}

/*
 * Function:  makeMove 
 * --------------------
 * The player "turn" makes a move at (row, col).
 * Note: This function MUST NOT change board, only row and col can be changed to reflect the move.
 */
int makeMove(const char board[26][26], int n, char turn, int *row, int *col) {
    char computerMoveList[676][2] = {}, opponentMoveList[676][2] = {}, boardCopy[26][26] = {}, colourCheck;
    int tempScore = 0, i, j, k, f, currentScore2 = 0, counter = 0, possibleMoveList[676][3] = {};
    
    //create copy of board to avoid errors if sending const type parameter
    for (i = 0;i < n;i++){
        for(j = 0; j<n; j++){
            boardCopy[i][j] = board[i][j];
        }
    }
    
    // opposite colour is what move you are checking
    if (turn == 'B'){
        colourCheck = 'W';
    }
    else{
        colourCheck = 'B';
    }
    
    //based on colour, creates a list of moves for computer
    addToMoveList(n, boardCopy, computerMoveList, colourCheck);
    
    //Incrementing, finding the highest score move
    for(i = 0; i <= n*n; i++){
        if (computerMoveList[i][0] != ' ' && computerMoveList[i][0] != 0){
            makeMoveCopy(boardCopy, n, turn, computerMoveList[i][0], computerMoveList[i][1]);
            addToMoveList(n, boardCopy, opponentMoveList, turn);
            possibleMoveList[i][0] = computerMoveList[i][0];
            possibleMoveList[i][1] = computerMoveList[i][1];
            for(j = n*n; j >= 0; j--){
                currentScore2 = 0;
                if (opponentMoveList[j][0] != ' ' && opponentMoveList[j][0] != 0){
                    currentScore2 += checkScore(colourCheck, boardCopy, 1, 0, n,  opponentMoveList[j][0] - 97, opponentMoveList[j][1] - 97);
                    currentScore2 += checkScore(colourCheck, boardCopy, 0, 1, n, opponentMoveList[j][0] - 97, opponentMoveList[j][1] - 97);
                    currentScore2 += checkScore(colourCheck, boardCopy, -1, 0, n, opponentMoveList[j][0] - 97, opponentMoveList[j][1] - 97);
                    currentScore2 += checkScore(colourCheck, boardCopy, 0, -1, n, opponentMoveList[j][0] - 97, opponentMoveList[j][1] - 97);
                    currentScore2 += checkScore(colourCheck, boardCopy, 1, 1, n, opponentMoveList[j][0] - 97, opponentMoveList[j][1] - 97);
                    currentScore2 += checkScore(colourCheck, boardCopy, -1, -1, n, opponentMoveList[j][0] - 97, opponentMoveList[j][1] - 97);
                    currentScore2 += checkScore(colourCheck, boardCopy, 1, -1, n, opponentMoveList[j][0] - 97, opponentMoveList[j][1] - 97);
                    currentScore2 += checkScore(colourCheck, boardCopy, -1, 1, n, opponentMoveList[j][0] - 97, opponentMoveList[j][1] - 97);
    
                    if (counter == 0){
                        tempScore = currentScore2;
                        possibleMoveList[i][2] = tempScore;
                        counter ++;
                    }
                    if(currentScore2 >= tempScore){
                        possibleMoveList[i][2] = tempScore;
                        tempScore = currentScore2;
                    }
                }
            }
        }
        for (k = 0;k < n;k++){
            for(f = 0; f<n; f++){
                boardCopy[k][f] = board[k][f];
            }
        }
    }
    
    // Initialize maximum element
    int smallest = possibleMoveList[0][2];
    int smallestIndex = 0;
 
    // Traverse array elements from second and
    // compare every element with current max 
    for (i = 1; i < n*n; i++){
        if (possibleMoveList[i][2] < smallest && possibleMoveList[j][0] != ' ' && possibleMoveList[j][0] != 0){
            smallest = possibleMoveList[0][2];
            smallestIndex = i;
        }
    }
    
    *row = possibleMoveList[smallestIndex][0];
    *col = possibleMoveList[smallestIndex][1];
    
    return 0;
}

/*
 * Function:  checkWinnner
 * --------------------
 * Checking who the winner is when game is over
 */
char checkWinnner(const char board[26][26], int n){
    int i, j, wScore = 0, bScore = 0;
    
    //counting scores of W, B across board
    for (i = 0;i < n;i++){
        for(j = 0; j<n; j++){
            if (board[i][j] == 'W') wScore++;
            else if (board[i][j] == 'B') bScore++;
        }
    }
    
    //returning winner or tie
    if (wScore > bScore) return 'W';
    else if (bScore > wScore) return 'B';
    else return 'T';
}

/*
 * Function:  checkGameOver
 * --------------------
 * Checking if no avialable moves for any player
 */
bool checkGameOver(char whiteMoves[676][2], char blackMoves[676][2]){
    if (whiteMoves[0][0] == ' ' && blackMoves[0][0] == ' '){
        return true;
    }
    else return false;
}

/*
 * Function:  checkTurn
 * --------------------
 * Used to check if next player has no available move, otherwise normal
 * player goes.
 */
char checkTurn(char whiteMoves[676][2], char blackMoves[676][2]){
    if (whiteMoves[0][0] == ' '){
        return 'B';
    }
    if (blackMoves[0][0] == ' '){
        return 'W';
    }
    return 'T';
}



#ifndef COMPETE_MODE // DO NOT DELETE THIS LINE
int main(void) {
    // Complete your main function here
    int i, j, nDim; //board dimension
    char board[26][26], whiteMoves[676][2] = {},  blackMoves[676][2] = {}, compColour, turn = 'B', winner;
    bool moveDecision = false, invalidMove = false;
    
    //enter board dimension
    printf("Enter the board dimension: ");
    scanf("%d", &nDim);
    
    //Ask user what colour player should be
    printf("Computer plays (B/W) : ");
    scanf(" %c", &compColour);
    
    //initialize the board
    for (i = 0; i < nDim; i++){
        for(j=0; j < nDim; j++){
            if ((i + 1 == nDim/2) && (j + 1 == nDim/2)){
                board[i][j] = 'W';
                board[i][j + 1] = 'B';
                j ++;
            }
            else if ((i == nDim/2) && (j + 1 == nDim/2)){
                board[i][j] = 'B';
                board[i][j + 1] = 'W';
                j ++;
            }
            else{
                board[i][j] = 'U';
            }
        }
    }
    //print the board
    printBoard(board, nDim);
    
    //White Moves:
    addToMoveList(nDim, board, whiteMoves, 'B');
    //Black Moves:
    addToMoveList(nDim, board, blackMoves, 'W');
    
    while(true){
        
        //Computer is black:
        if (turn == compColour){
            //makes sure there is an available move, otherwise make move
            if(checkTurn(whiteMoves, blackMoves) == 'T' ||checkTurn(whiteMoves, blackMoves) == 'B' ){
                makeComputerMove(board, nDim, compColour);
            }
            printBoard(board, nDim);
            
            //update list, check if game over, otherwise if there is available move for player
            addToMoveList(nDim, board, whiteMoves, 'B');
            addToMoveList(nDim, board, blackMoves, 'W');
            if (checkGameOver(whiteMoves,blackMoves)) break;
            if (checkTurn(whiteMoves, blackMoves) == 'B') printf("W player has no valid move.\n");
            
            //player turn
            if (checkTurn(whiteMoves, blackMoves) == 'T' ||checkTurn(whiteMoves, blackMoves) == 'W'){
                moveDecision = makePlayerMove(board, nDim, whiteMoves, 'W');
            
                //Checks for invalid move
                if (moveDecision == false){
                    printf("Invalid move.\n");
                    invalidMove = true;
                    break;
                }
                printBoard(board, nDim);
                
                //Updates list, checks if computer has a move, and if game is over
                addToMoveList(nDim, board, whiteMoves, 'B');
                addToMoveList(nDim, board, blackMoves, 'W');
                if (checkGameOver(whiteMoves,blackMoves)) break;
                if (checkTurn(whiteMoves, blackMoves) == 'W') printf("B player has no valid move.\n");
            }
        }
        
        //Player starts first
        else if (turn != compColour){
            if(checkTurn(whiteMoves, blackMoves) == 'T' ||checkTurn(whiteMoves, blackMoves) == 'B'){
                moveDecision = makePlayerMove(board, nDim, blackMoves, 'B');
                //invalid move
                if (moveDecision == false){
                    printf("Invalid move.\n");
                    invalidMove = true;
                    break;
                }
            }
            //Updates list, checks if computer has a move, and if game is over
            printBoard(board, nDim);
            addToMoveList(nDim, board, whiteMoves, 'B');
            addToMoveList(nDim, board, blackMoves, 'W');
            if (checkGameOver(whiteMoves,blackMoves)) break;
            if (checkTurn(whiteMoves, blackMoves) == 'B') printf("W player has no valid move.\n");
            
            //computer move (white)
            if (checkTurn(whiteMoves, blackMoves) == 'T' ||checkTurn(whiteMoves, blackMoves) == 'W'){
                makeComputerMove(board, nDim, compColour);
                printBoard(board, nDim);
            }
            addToMoveList(nDim, board, whiteMoves, 'B');
            addToMoveList(nDim, board, blackMoves, 'W');
            if (checkGameOver(whiteMoves,blackMoves)) break;
            if (checkTurn(whiteMoves, blackMoves) == 'W') printf("B player has no valid move.\n");
            
        }
        
        //Update moves for next set of turns
        addToMoveList(nDim, board, whiteMoves, 'B');
        addToMoveList(nDim, board, blackMoves, 'W');        
    }
    //Checks for the winner
    winner = checkWinnner(board, nDim);
    
    //Final outcome shown
    if (invalidMove) printf("%c player wins.\n", compColour);
    else if (winner == 'T'){
        printf("Draw!\n");
    }
    else printf("%c player wins.\n", winner);
    
    return 0;
}
#endif // DO NOT DELETE THIS LINE
