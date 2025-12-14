#ifndef ESTRUTURA_HPP
#define ESTRUTURA_HPP

#include <stdio.h>

#define LINHA 3
#define COLUNA 3
#define PROFUNDIDADE 9
#define TABTOTAL 9
#define CASATOTAL 9

enum class BoardMark : char {
    EMPTY = '-',
    XMARK = 'X',
    OMARK = 'O'
};

struct UltimateTicTacToe{
    BoardMark tabulirosMenores[TABTOTAL * CASATOTAL]; // 81 posições
    BoardMark tabuleiroPrincipal[TABTOTAL];
    BoardMark proximoJogador;
    int proximoTabuleiro;
};

//Funções:
bool detect_victory(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int index_played);
void init(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA]);
bool isBoardFull (BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int index_played);
void robotPlay(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int& mustPlayBoardIndex, BoardMark& currentPlayer);
void drawUltimateBoard();
void drawMarks();
void handleMouseInput();


#endif 
