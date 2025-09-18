#include "TicTacToe.h"
#include <array>
// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
const int AI_PLAYER   = 1;      // index of the AI player (O)
const int HUMAN_PLAYER= 0;      // index of the human player (X)

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}

// -----------------------------------------------------------------------------
// make an X or an O
// -----------------------------------------------------------------------------
// DO NOT CHANGE: This returns a new Bit with the right texture and owner
Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 1 ? "o.png" : "x.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    _gameOptions.rowX = 3;
	_gameOptions.rowY = 3;
    _gameOptions.numberOfPlayers = 2; 
    setNumberOfPlayers(_gameOptions.numberOfPlayers); // ------Change for AI? Maybe not------

    for (int row = 0; row < _gameOptions.rowY; row++) {
        for (int col = 0; col < _gameOptions.rowX; col++) {
            int screenOffset = 50;
            _grid[row][col].initHolder(ImVec2(screenOffset + col*100, screenOffset + row*100), "square.png", col, row);
        }
    }
if (gameHasAI()){
    setAIPlayer(AI_PLAYER);
}

    startGame();
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    if(!holder){return false;}
    if(!holder->empty()){return false;}

    Bit *newBit = PieceForPlayer(getCurrentPlayer()->playerNumber());
    newBit->setPosition(holder->getPosition());
    holder->setBit(newBit);
    return true;   
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    for (int row = 0; row < _gameOptions.rowY; row++) {
        for (int col = 0; col < _gameOptions.rowX; col++) {
            _grid[row][col].destroyBit();
        }
    }
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index ) const
{
    // index is 0..8
    int yPos = index / 3;
    int xPos = index % 3;
    Bit *indexedBit = _grid[yPos][xPos].bit();
    return indexedBit ? indexedBit->getOwner() : nullptr;
}

Player* TicTacToe::checkForWinner()
{
    std::array<std::array<int,3>, 8> winningTriples = {{
    {{0,1,2}}, {{3,4,5}}, {{6,7,8}},
    {{0,3,6}}, {{1,4,7}}, {{2,5,8}},
    {{0,4,8}}, {{2,4,6}}
    }};

    for(std::array<int,3> arr : winningTriples){
        Player* slot1P {ownerAt(arr[0])};
        Player* slot2P {ownerAt(arr[1])};
        Player* slot3P {ownerAt(arr[2])};

        if(slot1P && slot2P && slot3P &&
            slot1P->playerNumber() == slot2P->playerNumber() && slot2P->playerNumber() == slot3P->playerNumber()){
            return slot1P;
        }
    }
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    if(checkForWinner() != nullptr){
        return false;
    }
    for (int row = 0; row < _gameOptions.rowY; row++) {
        for (int col = 0; col < _gameOptions.rowX; col++) {
            if(!_grid[row][col].bit()){
                return false;
            }
        }
    }
    return true;
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString() const
{
    std::string returnString {""};
    for(int i=0; i<9; i++){
        Player *bitOwner = ownerAt(i);
        if(bitOwner){
            returnString.append(bitOwner->playerNumber() == 0 ? "1" : "2");
        }else{
            returnString.append("0");
        }
    }
    return returnString;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    int stringIndex = 0;
    for (int row = 0; row < _gameOptions.rowY; row++) {
        for (int col = 0; col < _gameOptions.rowX; col++) {
            int playerNumber = s[stringIndex] - '0';
            stringIndex ++;
            if(playerNumber == 0){continue;}
            playerNumber--;

            BitHolder *holder = &_grid[row][col]; 
            Bit *newBit = PieceForPlayer(playerNumber);
            newBit->setPosition(holder->getPosition());
            holder->setBit(newBit);
        }
    }
}


//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() 
{
    //Shitty ai code
    std::string state = stateString();
    for(int i=0; i<9; i++){
        if(state[i] == '0'){
            actionForEmptyHolder(&_grid[i/3][i%3]);
            endTurn();
            return;
        }
    }

    /*IP Smart AI
    std::string state = stateString();
    int bestMove = -1000;
    int bestSquare =-1;
    for(int i=0; i<9; i++){
        if(state[i] == '0'){
            state[i] = '2';
            int aiMove = -negamax(state,0,HUMAN_PLAYER);
            state[i] ='0';
            if(aiMove > bestMove){
                bestMove = aiMove;
                bestSquare = i;
            }
        }
    }

    if(bestSquare != -1){
        actionForEmptyHolder(&_grid[bestSquare/3][bestSquare%3]);
        endTurn();
    }
    */
}

bool isAIBoardFull(const std::string& state){
    return (state.find('0') == std::string::npos);
}

int checkForAIWinner(const std::string& state){
    static const int kWinningTriples[8][3] = {
    {0,1,2}, {3,4,5}, {6,7,8},
    {0,3,6}, {1,4,7}, {2,5,8},
    {0,4,8}, {2,4,6}
    };

    for (int i = 0; i < 8; i++)
    {
        const int *triple = kWinningTriples[i];
        char player = state[triple[0]];
        if(player != '0' && player == state[triple[1]] && player == state[triple[2]]){
            return 10;
        }
    }
    
    return 0;
}

int TicTacToe::negamax(std::string& state, int depth, int playerColor){
    int score = checkForAIWinner(state);

    if(score){
        return -score;
    }

    if (isAIBoardFull(state)) {
        return 0;
    }

    int bestVal = -1000;
    for(int i=0; i<9; i++){
        if(state[i] == '0'){
            state[i] = playerColor == HUMAN_PLAYER ? '1' : '2';
            bestVal = std::max(bestVal, -negamax(state, depth+1, -playerColor));
            state[i] = '0';
        }
    }
    return bestVal;
}

