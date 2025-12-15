#include <stdio.h>
#include "estrutura.hpp"
#define LINHA 3
#define COLUNA 3
#define PROFUNDIDADE 9

 static const int winPossibilities[8][3][2] = {
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

        // Diagonais
        {{0, 0},
         {1, 1},
         {2, 2}},

        {{0, 2},
         {1, 1},
         {2, 0}}};

bool detectVictory(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int index_played)
{
    for (int i = 0; i < 8; i++)
    {
        //verifica se alguém ganhou, medium pode ser XMARK ou OMARK
        BoardMark medium = matrix[index_played][winPossibilities[i][1][0]][winPossibilities[i][1][1]];
        if (medium != BoardMark::EMPTY &&
            matrix[index_played][winPossibilities[i][0][0]][winPossibilities[i][0][1]] == medium &&
            matrix[index_played][winPossibilities[i][2][0]][winPossibilities[i][2][1]] == medium)
        {
            return true;
        }
    }
    return false;
}

BoardMark detectGlobalVictory(BoardMark winnerMatrix[LINHA][COLUNA]) {
    
    for (int i = 0; i < 8; i++) {
        BoardMark medium = winnerMatrix[winPossibilities[i][1][0]][winPossibilities[i][1][1]];
        
        if (medium != BoardMark::EMPTY &&
            winnerMatrix[winPossibilities[i][0][0]][winPossibilities[i][0][1]] == medium &&
            winnerMatrix[winPossibilities[i][2][0]][winPossibilities[i][2][1]] == medium)
        {
            return medium; // Retorna XMARK ou OMARK
        }
    }
    
    // Se não houver vencedor, verifica se houve empate global 
    bool isDraw = true;
    for (int i = 0; i < LINHA; i++) {
        for (int j = 0; j < COLUNA; j++) {
            if (winnerMatrix[i][j] == BoardMark::EMPTY) {
                isDraw = false;
                break;
            }
        }
    }

    if (isDraw == true) {
        return BoardMark::TIE; 
    }
    
    return BoardMark::EMPTY; // Nenhum vencedor ainda
}

void init(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], BoardMark winnerMatrix[LINHA][COLUNA])
    {
        for (int z = 0; z < PROFUNDIDADE; z++)
        {
            for (int i = 0; i < LINHA; i++) 
            {
                for (int j = 0; j < COLUNA; j++)
                {
                    matrix[z][i][j] = BoardMark::EMPTY;
                } 
            }
        }

        for (int i = 0; i < LINHA; i++)
        {
            for (int j = 0; j < COLUNA; j++)
            {
            winnerMatrix[i][j] = BoardMark::EMPTY;
            }   
        }     
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


void robotPlay(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], BoardMark winnerMatrix[LINHA][COLUNA], int& mustPlayBoardIndex, BoardMark& currentPlayer)
{
    // Variáveis para guardar a melhor jogada
    int bestBoardIndex = -1;
    int bestLin = -1;
    int bestCol = -1;
    // Indica a pontuação da jogada (confiança) 
    int bestScore = -1000; 
    BoardMark opponent;

    //Alterna os jogadores
    if (currentPlayer == BoardMark::XMARK) {
        opponent = BoardMark::OMARK;
    } else {
        opponent = BoardMark::XMARK;
    }


    for(int z = 0; z < PROFUNDIDADE; z++)
    {
        // Verifica se, de acordo com a regra, o tabuleiro está disponível para jogar
        bool isAllowedBoard; 
        if (mustPlayBoardIndex == -1 || z == mustPlayBoardIndex) {
            isAllowedBoard = true;
        } else {
            isAllowedBoard = false;
        }
        // O robô ignora o tabuleiro se:
        if (!isAllowedBoard || detectVictory(matrix, z) || isBoardFull(matrix, z)) {
            continue; 
        }

        for(int i = 0; i < LINHA; i++)
        {
            for(int j = 0; j < COLUNA; j++)
            {
                if(matrix[z][i][j] == BoardMark::EMPTY) 
                {
                    int currentScore = 0; // Prioridade

                    // 1: Simula e verifica a possibilidade de vitória
                    matrix[z][i][j] = currentPlayer; // Simula a jogada
                    if(detectVictory(matrix, z)) {
                        currentScore = 1000; // Se ele for ganhar com a jogada
                    }

                    matrix[z][i][j] = BoardMark::EMPTY; // Desfaz a simulação 

                    
                    // 2: Simula e verifica o bloqueio da jogada do oponente
 
                    if (currentScore < 1000) 
                    {   
                        // Detecta a jogada do oponente e simula o bloqueio
                        matrix[z][i][j] = opponent; 
                        if(detectVictory(matrix, z)) {
                            currentScore = 500; 
                        }
                        matrix[z][i][j] = BoardMark::EMPTY;
                    }
                    
                    // 3: Atualização da melhor jogada
                    if(currentScore > bestScore) // Verifica se o robô conseguiu executar a jogada
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


    // Execução da jogada do robô e continua a partida
    if(bestBoardIndex != -1)
    {
        matrix[bestBoardIndex][bestLin][bestCol] = currentPlayer;

        int nextBoardIndex = bestLin * 3 + bestCol; 
        mustPlayBoardIndex = nextBoardIndex;

        if(detectVictory(matrix, mustPlayBoardIndex) || isBoardFull(matrix, mustPlayBoardIndex)) {
            mustPlayBoardIndex = -1;
        }
        
        // Alterna o jogador
        currentPlayer = opponent; 
    }
}



// Implementação da função para converter a lógica 3D/2D para a estrutura 1D para salvar
void matrixToStruct(const BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], 
                      const BoardMark winnerMatrix[LINHA][COLUNA], 
                      int mustPlay, BoardMark currentPlayer,
                      UltimateTicTacToe *gameOut) {
    
    // 1. Tabuleiros Menores (Matrix 3d)
    for (int z = 0; z < PROFUNDIDADE; z++) {
        for (int i = 0; i < LINHA; i++) {
            for (int j = 0; j < COLUNA; j++) {
                // Fórmula: Índice = z * 9 + i * 3 + j
                int index1d = z * (LINHA * COLUNA) + i * COLUNA + j;
                gameOut->tabuleirosMenores[index1d] = matrix[z][i][j];
            }
        }
    }

    // 2. Tabuleiro Principal (matrix 2d)
    for (int i = 0; i < LINHA; i++) {
        for (int j = 0; j < COLUNA; j++) {
            // Fórmula: índice = i * 3 + j
            int index1d = i * COLUNA + j;
            gameOut->tabuleiroPrincipal[index1d] = winnerMatrix[i][j];
        }
    }

    //3. Variáveis de Controle
    gameOut->proximoJogador = currentPlayer;
    gameOut->proximoTabuleiro = mustPlay;
}


// Implementação da função para converter a estrutura 1D para a lógica 3D/2D após carregar
void structToMatrix(const UltimateTicTacToe *gameIn,
                      BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], 
                      BoardMark winnerMatrix[LINHA][COLUNA], 
                      int *mustPlay, BoardMark *currentPlayer) {

 
    for (int z = 0; z < PROFUNDIDADE; z++) {
        for (int i = 0; i < LINHA; i++) {
            for (int j = 0; j < COLUNA; j++) {
                // Fórmula: Índice = z * 9 + i * 3 + j
                int index1d = z * (LINHA * COLUNA) + i * COLUNA + j;
                matrix[z][i][j] = gameIn->tabuleirosMenores[index1d];
            }
        }
    }

    for (int i = 0; i < LINHA; i++) {
        for (int j = 0; j < COLUNA; j++) {
            // Fórmula: indice = i * 3 + j
            int index1d = i * COLUNA + j;
            winnerMatrix[i][j] = gameIn->tabuleiroPrincipal[index1d];
        }
    }

    //3. Variáveis de Controle
    // Atualiza as variáveis usando ponteiros.
    *currentPlayer = gameIn->proximoJogador;
    *mustPlay = gameIn->proximoTabuleiro;
}

