// C++ program to find the next optimal move for 
// a computer 
#include<bits/stdc++.h> 
using namespace std;
#include <random>
#include <iostream>
#include <chrono>

#include "Tic-Tac-Toe-Game-Rand.h"
#define SIZE 9

#define SHUFFLE 0
#define FAST_SHUFFLE 1
#define NO_SHUFFLE 2
#define ORDER_MODE FAST_SHUFFLE

#define TEST_SPEED 1
#define PLAY 0
#define MODE PLAY

#define TEST_TIMES 10000

uint32_t s[SIZE] = {0,1,2,3,4,5,6,7,8};

unsigned seed = chrono::high_resolution_clock::now().time_since_epoch().count();
struct Move 
{ 
    int row, col; 
}; 
  
char computer = 'x', player = 'o'; 
  
bool anyMovesLeft(char board[3][3]) 
{ 
    for (int i = 0; i<3; i++) 
        for (int j = 0; j<3; j++) 
            if (board[i][j]=='_') 
                return true; 
    return false; 
} 
  
int evaluate(char b[3][3]) 
{ 
    for (int row = 0; row<3; row++) 
    { 
        if (b[row][0]==b[row][1] && 
            b[row][1]==b[row][2]) 
        { 
            if (b[row][0]==computer) 
                return +10; 
            else if (b[row][0]==player) 
                return -10; 
        } 
    } 
  
    for (int col = 0; col<3; col++) 
    { 
        if (b[0][col]==b[1][col] && 
            b[1][col]==b[2][col]) 
        { 
            if (b[0][col]==computer) 
                return +10; 
  
            else if (b[0][col]==player) 
                return -10; 
        } 
    } 
  
    if (b[0][0]==b[1][1] && b[1][1]==b[2][2]) 
    { 
        if (b[0][0]==computer) 
            return +10; 
        else if (b[0][0]==player) 
            return -10; 
    } 
  
    if (b[0][2]==b[1][1] && b[1][1]==b[2][0]) 
    { 
        if (b[0][2]==computer) 
            return +10; 
        else if (b[0][2]==player) 
            return -10; 
    } 
      return 0; 
}

// map random value to [0,range), redraws to avoid bias if needed
static inline uint32_t pcg32_random_bounded_divisionless(uint32_t range) {
    uint64_t random32bit, multiresult;
    uint32_t leftover;
    uint32_t threshold;
    random32bit =  pcg32_random();
    multiresult = random32bit * range;
    leftover = (uint32_t) multiresult;
    if(leftover < range ) {
        threshold = -range % range ;
        while (leftover < threshold) {
            random32bit =  pcg32_random();
            multiresult = random32bit * range;
            leftover = (uint32_t) multiresult;
        }
    }
    return multiresult >> 32; // [0, range)
}

void shuffle_pcg_divisionless(uint32_t *storage, uint32_t size) {
    uint32_t i;
    for (i=size; i>1; i--) {
        uint32_t nextpos = pcg32_random_bounded_divisionless(i);
        uint32_t tmp = storage[i-1];// likely in cache
        uint32_t val = storage[nextpos]; // could be costly
        storage[i - 1] = val;
        storage[nextpos] = tmp; // you might have to read this store later
    }
}

int minimax(char board[3][3], int depth, bool isMax) 
{ 
    int score = evaluate(board); 
  
    if (score == 10) 
        return score; 
    if (score == -10) 
        return score; 
    if (anyMovesLeft(board)==false) 
        return 0; 

    if(ORDER_MODE == SHUFFLE){
        shuffle (&s[0], &s[8], std::ranlux48_base(seed));
    } else if(ORDER_MODE == FAST_SHUFFLE){
        shuffle_pcg_divisionless(s,SIZE);
    }   

    if (isMax) 
    { 
        int best = -1000; 
  
        for (int i = 0; i<3; i++) 
        { 
            for (int j = 0; j<3; j++) 
            { 
                uint32_t pos_i = s[i*3+j]/3;
                uint32_t pos_j = s[i*3+j]%3;
                if (board[pos_i][pos_j]=='_') 
                { 
                    board[pos_i][pos_j] = computer; 
                    best = max( best, 
                        minimax(board, depth+1, !isMax) ); 
                    board[pos_i][pos_j] = '_';

                    if (best == 10) {
                        return best;
                    }
                     
                } 
            } 
        } 
        return best; 
    }
    else
    { 
        int worst = 1000; 
        for (int i = 0; i<3; i++) 
        { 
            for (int j = 0; j<3; j++) 
            { 
                uint32_t pos_i = s[i*3+j]/3;
                uint32_t pos_j = s[i*3+j]%3;
                if (board[pos_i][pos_j]=='_') 
                { 
                    board[pos_i][pos_j] = player; 
                    worst = min(worst, 
                           minimax(board, depth+1, !isMax)); 
                    board[pos_i][pos_j] = '_';

                    if (worst == -10) {
                        return worst;
                    }
                    
                } 
            } 
        } 
        return worst; 
    } 
} 

Move findBestMove(char board[3][3]) 
{ 
    int bestVal = -1000; 
    Move bestMove; 
    bestMove.row = -1; 
    bestMove.col = -1; 

    if(ORDER_MODE == SHUFFLE){
        shuffle (&s[0], &s[8], std::ranlux48_base(seed));
    } else if(ORDER_MODE == FAST_SHUFFLE){
        shuffle_pcg_divisionless(s,SIZE);
    }   

    for (int i = 0; i<3; i++) 
    { 
        for (int j = 0; j<3; j++) 
        { 
            uint32_t pos_i = s[i*3+j]/3;
            uint32_t pos_j = s[i*3+j]%3;
            if (board[pos_i][pos_j]=='_') 
            { 
                board[pos_i][pos_j] = computer; 
                int moveVal = minimax(board, 0, false); 
                board[pos_i][pos_j] = '_'; 
                if (moveVal > bestVal) 
                { 
                    bestMove.row = pos_i; 
                    bestMove.col = pos_j; 
                    bestVal = moveVal; 
                } 
                if (moveVal == 10) {
                    return bestMove;
                }
            } 
        } 
    } 
  
    return bestMove; 
} 

void showInstructions() 
{ 
    printf("\t\t\t  Tic-Tac-Toe\n\n"); 
    printf("Choose a cell numbered from 1 to 9 as below"
            " and play\n\n"); 
      
    printf("\t\t\t  1 | 2  | 3  \n"); 
    printf("\t\t\t--------------\n"); 
    printf("\t\t\t  4 | 5  | 6  \n"); 
    printf("\t\t\t--------------\n"); 
    printf("\t\t\t  7 | 8  | 9  \n\n"); 
      
    printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n\n"); 
  
    return; 
}

void showBoard(char board[3][3]) 
{ 
    printf("\n\n"); 
      
    printf("\t\t\t  %c | %c  | %c  \n", board[0][0], 
                             board[0][1], board[0][2]); 
    printf("\t\t\t--------------\n"); 
    printf("\t\t\t  %c | %c  | %c  \n", board[1][0], 
                             board[1][1], board[1][2]); 
    printf("\t\t\t--------------\n"); 
    printf("\t\t\t  %c | %c  | %c  \n\n", board[2][0], 
                             board[2][1], board[2][2]); 
   
    return; 
}

bool weHaveAWinner(char board[3][3]){
    if (evaluate(board) == 10) {
        printf("Computer wins\n");
        return true;
    }

    if (evaluate(board) == -10) {
        printf("Player wins\n");
        return true;
    }
    return false;
}
  
int main() 
{ 
    char board[3][3] = 
    { 
        { '_', '_', '_' }, 
        { '_', '_', '_' }, 
        { '_', '_', '_' } 
    };
    
    if(MODE == TEST_SPEED){
        long long testSum = 0;
        for (int t=0;t<TEST_TIMES; t++){             
            long long runSum = 0;
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    board[i][j] = player;
                    auto start = std::chrono::high_resolution_clock::now();
                    Move bestMove = findBestMove(board);
                    auto finish = std::chrono::high_resolution_clock::now();
                    runSum += std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

                    board[i][j] = '_';
                }
            }
            long long runAverage = runSum/SIZE;
            testSum += runAverage;
            if (t % 50 == 0){
                cout << "Zavrsena iteracija " << t << "\n";
            }
        }
        long long testAverage = testSum/TEST_TIMES;
        cout << "Average: " << testAverage << "\n";
    } else {
        while(anyMovesLeft(board)){
            showInstructions();
            showBoard(board);
            int position;
            int i,j;
            do{
                scanf("%d", &position);

                i = (position - 1)/3;
                j = (position - 1)%3;
            }
            while(board[i][j] != '_');
            
            board[i][j]=player;
            if (!anyMovesLeft(board) || weHaveAWinner(board)){
                showBoard(board);
                break;
            }            
            auto start = std::chrono::high_resolution_clock::now();
            Move bestMove = findBestMove(board);
            auto finish = std::chrono::high_resolution_clock::now();
            std::cout << "Time required: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";

            board[bestMove.row][bestMove.col] = computer;
            if (!anyMovesLeft(board) || weHaveAWinner(board)){
                showBoard(board);
                break;
            }
        }
        if (evaluate(board) == 0) {
            printf("It is a drawn\n");
        }
    }
    return 0; 
} 