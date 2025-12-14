#include <stdio.h>
#include "estrutura.hpp"
#define LINHA 3
#define COLUNA 3
#define PROFUNDIDADE 9

//TODO:
//Fazer arquivos até a quarta interagração (fazer um save da partida)
//Validação de vitória geral para fazer a janela de vitória

bool detect_victory(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int index_played)
{
    int possibilities[8][3][2] = {
        {{0, 0},
         {0, 1},
         {0, 2}},
        
        {{1, 0},
         {1, 1},
         {1, 2}},
        
        {{2, 0},
         {2, 1},
         {2, 2}},
        
        {{0, 0},
         {1, 0},
         {2, 0}},
        
        {{0, 1},
         {1, 1},
         {2, 1}},

        {{0, 2},
         {1, 2},
         {2, 2}},
        
        //Diagonais
        {{0, 0},
         {1, 1},
         {2, 2}},
         
        {{0, 2},
         {1, 1},
         {2, 0}}
    };

    for (int i = 0; i < 8; i++)
    {
        BoardMark medium = matrix[index_played][possibilities[i][1][0]][possibilities[i][1][1]];
        if (medium != BoardMark::EMPTY &&
            matrix[index_played][possibilities[i][0][0]][possibilities[i][0][1]] == medium &&
            matrix[index_played][possibilities[i][2][0]][possibilities[i][2][1]] == medium)
        {
            return true;
        }
    }
    return false;
}

void init(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA])
{
    for (int z = 0; z < PROFUNDIDADE; z++)
        for (int i = 0; i < LINHA; i++)
            for (int j = 0; j < COLUNA; j++)
                matrix[z][i][j] = BoardMark::EMPTY;
}

bool isBoardFull(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int index_played)
{
    for(int i = 0; i < LINHA; i++){
        for(int j = 0; j < COLUNA; j++){
            if(matrix[index_played][i][j] == BoardMark:: EMPTY){
                return false;
            }            
        }
    }
    //se o loop não achar nenhum espaço vazio, retorna true para cheio;
    return true;   
}


void robotPlay(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int& mustPlayBoardIndex, BoardMark& currentPlayer)
{
    int bestBoardIndex = -1;
    int bestLin = -1;
    int bestCol = -1;
    int bestScore = -1000;

    BoardMark opponent = (currentPlayer == BoardMark::XMARK) ? BoardMark::OMARK : BoardMark::XMARK;


    for(int z = 0; z < PROFUNDIDADE; z++)
    {
        bool isAllowedBoard = (mustPlayBoardIndex == -1) || (z == mustPlayBoardIndex);

        if (!isAllowedBoard || detect_victory(matrix, z) || isBoardFull(matrix, z)) {
            continue; 
        }

        for(int i = 0; i < LINHA; i++)
        {
            for(int j = 0; j < COLUNA; j++)
            {
                if(matrix[z][i][j] == BoardMark::EMPTY) 
                {
                    int currentScore = 0; // Prioridade

                    // 1: Simula e checa a possibilidade de vitória

                    matrix[z][i][j] = currentPlayer; // Simula a jogada
                    if(detect_victory(matrix, z)) {
                        currentScore = 1000;
                    }
                    matrix[z][i][j] = BoardMark::EMPTY; // Desfaz a simulação 

                    
                    // 2: Simula e checa o bloqueio da jogada do oponente
 
                    if (currentScore < 1000) { // Se não for vitória, verifica o bloqueio
                        
                        // Oponente joga na célula [i][j] (para bloquear a vitória do robô no tabuleiro atual)
                        matrix[z][i][j] = opponent; 
                        if(detect_victory(matrix, z)) {
                            currentScore = 500; 
                        }
                        matrix[z][i][j] = BoardMark::EMPTY;
                    }
                    
                    // 3: Atualização da melhor jogada
                    if(currentScore > bestScore)
                    {
                        bestScore = currentScore;
                        bestBoardIndex = z;
                        bestLin = i;
                        bestCol = j; 
                    }
                }
            }
        }
    }


    // Execução da jogada
    if(bestBoardIndex != -1)
    {
        matrix[bestBoardIndex][bestLin][bestCol] = currentPlayer;

        int nextBoardIndex = bestLin * 3 + bestCol;
        mustPlayBoardIndex = nextBoardIndex;

        if(detect_victory(matrix, mustPlayBoardIndex) || isBoardFull(matrix, mustPlayBoardIndex)) {
            mustPlayBoardIndex = -1;
        }
        
        // Alterna o jogador
        currentPlayer = opponent; 
    }
}



// void matrix_to_struct(const BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], UltimateTicTacToe *jogo) {
    
//     // 1. Converte a matriz 3D para o array 1D
//     for (int z = 0; z < PROFUNDIDADE; z++) {
//         for (int i = 0; i < LINHA; i++) {
//             for (int j = 0; j < COLUNA; j++) {
//                 // Cálculo de índice 1D (Serialização)
//                 int index_1d = z * (LINHA * COLUNA) + i * COLUNA + j;
//                 BoardMark tabuleiros_menores[index_1d] = matrix[z][i][j];
//             }
//         }
//     }

// void struct_to_matrix(const UltimateTicTacToe *jogo, BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA]) {
//     // 1. Converte o array 1D para a matriz 3D
//     for (int z = 0; z < PROFUNDIDADE; z++) {
//         for (int i = 0; i < LINHA; i++) {
//             for (int j = 0; j < COLUNA; j++) {
//                 // Cálculo de índice 1D (Desserialização)
//                 int index_1d = z * (LINHA * COLUNA) + i * COLUNA + j;
//                 matrix[z][i][j] = jogo->tabuleiros_menores[index_1d];
//             }
//         }
//     }

