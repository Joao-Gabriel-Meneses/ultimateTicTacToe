
#include <stdio.h>

#define LINHA 3
#define COLUNA 3
#define PROFUNDIDADE 9
#define NOME_ARQUIVO "ultimate_save.dat"

enum class BoardMark : char {
    EMPTY = '-',
    XMARK = 'X',
    OMARK = 'O',
    TIE
};

// Construção dos Arquivos
struct UltimateTicTacToe {
    BoardMark tabuleirosMenores[PROFUNDIDADE * LINHA * COLUNA]; 
    BoardMark tabuleiroPrincipal[PROFUNDIDADE]; 
    BoardMark proximoJogador;
    int proximoTabuleiro;
};

//Funções da Lógica:
bool detectVictory(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int index_played);
BoardMark detectGlobalVictory(BoardMark winnerMatrix[LINHA][COLUNA]);
void init(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], BoardMark winnerMatrix[LINHA][COLUNA]);
bool isBoardFull (BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int index_played);
void robotPlay(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], BoardMark winnerMatrix[LINHA][COLUNA], int& mustPlayBoardIndex, BoardMark& currentPlayer);
void cleanmatrix(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int index_played);

//Interface Gráfica:
void drawUltimateBoard();
int drawWelcomeScreen();
void drawEndScreen();
void drawMarks();
void handleMouseInput();

//Arquivos:
void matrixToStruct(const BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], 
                      const BoardMark winnerMatrix[LINHA][COLUNA], 
                      int mustPlay, BoardMark currentPlayer,
                      UltimateTicTacToe *jogo_out);

void structToMatrix(const UltimateTicTacToe *jogo_in,
                      BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], 
                      BoardMark winnerMatrix[LINHA][COLUNA], 
                      int *mustPlay, BoardMark *currentP);

void saveGame(const UltimateTicTacToe *jogo, const char *nome_arquivo);
int loadGame(UltimateTicTacToe *jogo, const char *nome_arquivo);
void DeleteGame(const char *nome_arquivo);

