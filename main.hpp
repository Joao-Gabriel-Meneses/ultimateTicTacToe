#ifndef MAIN_HPP
#define MAIN_HPP

#include <stdio.h>

#define LINHA 3
#define COLUNA 3
#define PROFUNDIDADE 9
#define RUNNING false

enum class BoardMark : char {
    EMPTY = '-',
    XMARK = 'X',
    OMARK = 'O'
};

//Funções:
bool detect_victory(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], BoardMark winnerMatrix[LINHA][COLUNA], int index_played);
void init(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], BoardMark winnerMatrix[LINHA][COLUNA]);
//void play(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int max_plays);
//void print(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA]);
void drawUltimateBoard();
void drawMarks();
void handleMouseInput();


#endif 
