#include "raylib.h"
#include <stdio.h>
#include "estrutura.hpp"
#define NOME_ARQUIVO "ultimate_save.dat"

BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA];
BoardMark currentPlayer;
BoardMark winnerMatrix[LINHA][COLUNA];
int mustPlayBoardIndex = -1;
bool isRobotGame;
UltimateTicTacToe estado_io;

enum class GameScreen {
    WELCOME,
    GAME,
    END
};

BoardMark globalWinner = BoardMark::EMPTY;
GameScreen currentScreen = GameScreen::WELCOME;


const int screenSize = 600;
const int bigCellSize = screenSize / 3;
const int smallCellSize = bigCellSize / 3;

struct Button
{
    Rectangle rect;
    const char *label;
    Color color;
};

Button buttons[3];

void initWelcomeButtons()
{
    buttons[0].rect = (Rectangle) {100, 200, 300, 40};
    buttons[0].label = "Começar com X";
    buttons[0].color = LIGHTGRAY;

    buttons[1].rect = (Rectangle){100, 250, 300, 40};
    buttons[1].label = "Começar com O";
    buttons[1].color = LIGHTGRAY;

    buttons[2].rect = (Rectangle){ 100, 300, 300, 40 };
    buttons[2].label = "Jogar contra o robô";
    buttons[2].color = LIGHTGRAY;
}

int drawWelcomeScreen()
{
    ClearBackground(RAYWHITE);

    DrawText("Bem-vindo ao Ultimate Tic Tac Toe", 50, 100, 20, BLACK);
    Vector2 mousePos = GetMousePosition();
    int clickedButton = -1;

    for (int i = 0; i < 3; i++) {
        // Muda cor se mouse estiver sobre o botão
        Color c = buttons[i].color;
        if (CheckCollisionPointRec(mousePos, buttons[i].rect)) c = GRAY;

        DrawRectangleRec(buttons[i].rect, c);
        DrawText(buttons[i].label, buttons[i].rect.x + 10, buttons[i].rect.y + 10, 20, BLACK);

        // Verifica clique
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, buttons[i].rect)) 
        {
            if(i == 0 || i == 2) // 2 refere-se a jogar contra o robô
                currentPlayer = BoardMark :: XMARK;
            if(i == 1)
                currentPlayer = BoardMark :: OMARK;

            clickedButton = i;
        }
    }

    return clickedButton; // retorna 0, 1 ou 2 se clicado; -1 se nenhum
}


void drawEndScreen() 
{
    ClearBackground(RAYWHITE);
    const char* winnerText = NULL;
    Color color = BLACK;

    if (globalWinner == BoardMark::XMARK) 
    {
        winnerText = "X VENCEU O JOGO!";
        color = RED;
    } 
    else if (globalWinner == BoardMark::OMARK) 
    {
        winnerText = "O VENCEU O JOGO!";
        color = BLUE;
    } 
    else if (globalWinner == BoardMark::TIE) 
    {
        winnerText = "JOGO EMPATADO!";
        color = GRAY;
    }

    DrawText(winnerText, screenSize / 2 - MeasureText(winnerText, 40) / 2, 150, 40, color);

    // Botão de jogar novamente
    Rectangle restartButton = {screenSize / 2 - 100, 300, 200, 50};
    DrawRectangleRec(restartButton, LIGHTGRAY);
    DrawText("JOGAR NOVAMENTE", restartButton.x + 10, restartButton.y + 15, 20, BLACK);

    // Lógica do clique
    if (CheckCollisionPointRec(GetMousePosition(), restartButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Reinicia o estado do jogo para a tela de boas-vindas
        init(matrix, winnerMatrix); 
        globalWinner = BoardMark::EMPTY;
        currentScreen = GameScreen::WELCOME;
    }
}

void drawUltimateBoard() 
{

    if(mustPlayBoardIndex != -1)
    {
        int targetBigLin = mustPlayBoardIndex / 3;
        int targetBigCol = mustPlayBoardIndex % 3;

        int startCol = targetBigCol * bigCellSize;
        int startLin = targetBigLin * bigCellSize;

        DrawRectangle(startCol, startLin, bigCellSize, bigCellSize, YELLOW);
    }

    // Tabuleiro grande
    for (int i = 1; i < 3; i++) 
    {
        DrawLine(i * bigCellSize, 0, i * bigCellSize, screenSize, BLACK);
        DrawLine(0, i * bigCellSize, screenSize, i * bigCellSize, BLACK);
    }

    // Mini-tabuleiros
    for (int bigLin = 0; bigLin < LINHA; bigLin++) 
    {
        for (int bigCol = 0; bigCol < COLUNA; bigCol++) 
        {
            for (int y = 1; y < 3; y++) 
            {
                DrawLine(bigCol * bigCellSize, bigLin * bigCellSize + y * smallCellSize,
                         (bigCol + 1) * bigCellSize, bigLin * bigCellSize + y * smallCellSize, GRAY);
                DrawLine(bigCol * bigCellSize + y * smallCellSize, bigLin * bigCellSize,
                         bigCol * bigCellSize + y * smallCellSize, (bigLin + 1) * bigCellSize, GRAY);
            }
        }
    }
}

void drawMarks() 
{
    // Desenhos das linhas do tabuleiro grande
    for (int bigLin = 0; bigLin < LINHA; bigLin++) 
    {
        // Desenhos das colunas do tabuleiro grande
        for (int bigCol = 0; bigCol < COLUNA; bigCol++) 
        {
            // Cálculo do próximo tabuleiro jogável de acordo com a regra do jogo 
            int boardIndex = bigLin * 3 + bigCol; 
            
            // Dsenho das Linhas dentro do mini-tabuleiro
            for (int smallLin = 0; smallLin < LINHA; smallLin++) 
            {
                // Desenho das coluna dentro do mini-tabuleiro
                for (int smallCol = 0; smallCol < COLUNA; smallCol++) 
                {
                    int centerHorizontal = bigCol * bigCellSize + smallCol * smallCellSize + smallCellSize / 2;
                    int centerVertical = bigLin * bigCellSize + smallLin * smallCellSize + smallCellSize / 2;
                    // Desenho do X e O dentro da matriz
                    if (matrix[boardIndex][smallLin][smallCol] == BoardMark::XMARK) 
                    {
                        DrawLine(centerHorizontal - 20, centerVertical - 20, centerHorizontal + 20, centerVertical + 20, RED);
                        DrawLine(centerHorizontal - 20, centerVertical + 20, centerHorizontal + 20, centerVertical - 20, RED);
                    } 
                    else if (matrix[boardIndex][smallLin][smallCol] == BoardMark::OMARK) 
                    {
                        DrawCircleLines(centerHorizontal, centerVertical, 20, BLUE);
                    }
                }
            }
        }
    }
}

void cleanmatrix(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int index_played)
{
    Rectangle detectdelete = {screenSize / 2 - 100, 300, 200, 50};
       char cclear;
     if (CheckCollisionPointRec(GetMousePosition(), detectdelete) && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) 
     {
         if(1) 
         {
             printf("\nDeseja apagar o tabuleiro atual? (S/N): ");
             fflush(stdout); 
             scanf("%c", &cclear);
     
              if(cclear == 'S' || cclear == 's') 
              {
                for(int i = 0 ; i < LINHA; i++)
                {
                    for(int j = 0; j < COLUNA; j++)
                    {
                        matrix[index_played][i][j] = BoardMark :: EMPTY;
                    }
                }
              }
         }
    
    }
}

void askForSave(bool ignoreSave) 
{
    
    // Verifica se o jogo deve ser ignorado (estava em WELCOME ou END)
    if (ignoreSave == true) 
        printf("Programa encerrado fora do jogo ativo, sem salvar.\n");
    
    char ans;
    
    while (1) 
     {
        printf("\nDeseja salvar o jogo atual? (S/N): ");
        fflush(stdout); 
        scanf("%c", &ans);

        if (ans == 'S' || ans == 'N' || ans == 's' || ans == 'n')  
            break;
        else 
            printf("Resposta inválida. Por favor, digite 'S' (Sim) ou 'N' (Não).\n");
     }
    
    if (ans == 'S' || ans == 's') 
     {
        printf("Salvando o estado do jogo...\n");
        
        matrixToStruct(matrix, winnerMatrix, mustPlayBoardIndex, currentPlayer, &estado_io);
        saveGame(&estado_io, NOME_ARQUIVO);
        
        printf("Jogo salvo com sucesso!\n");
     }
    else 
     {
        DeleteGame(NOME_ARQUIVO);
        printf("Jogo não salvo. Encerrando programa.\n");
     }
}


void handleMouseInput()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        int bigCol = mouse.x / bigCellSize;
        int bigLin = mouse.y / bigCellSize;
        int smallCol = (int(mouse.x) % bigCellSize) / smallCellSize;
        int smallLin = (int(mouse.y) % bigCellSize) / smallCellSize;

        // Índice do mini-tabuleiro onde o clique ocorreu 
        int boardIndex = bigLin * 3 + bigCol; 
        // Cálculo do próximo tabuleiro de acordo com as regras do jogo
        int nextBoardIndex = smallLin * 3 + smallCol; 

        // 1. Verificação de limites e se o tabuleiro é o obrigatório
        if (bigLin >= 0 && bigLin < 3 && bigCol >= 0 && bigCol < 3) 
        {
            bool IsPlayAllowed;

        if (mustPlayBoardIndex == -1 || boardIndex == mustPlayBoardIndex) 
            IsPlayAllowed = true;
        else 
            IsPlayAllowed = false; 
            
            if(IsPlayAllowed)
            {    
                if (winnerMatrix[bigLin][bigCol] == BoardMark::EMPTY && 
                    matrix[boardIndex][smallLin][smallCol] == BoardMark::EMPTY)
                {
                    // Faz a jogada
                    matrix[boardIndex][smallLin][smallCol] = currentPlayer;
                    
                    // Verifica o mini-Tabuleiro e decreta o vencedor
                    if(detectVictory(matrix,boardIndex)) { 
                        winnerMatrix[bigLin][bigCol] = currentPlayer; 
                    }
                    
                    // Atualiza o próximo Tabuleiro
                    mustPlayBoardIndex = nextBoardIndex; 
                    
                    // Regra da Exeção: O próximo tabuleiro obrigatório já foi vencido ou está cheio
                    int nextBigLin = nextBoardIndex / 3;
                    int nextBigCol = nextBoardIndex % 3;
                    
                    if (winnerMatrix[nextBigLin][nextBigCol] != BoardMark::EMPTY || 
                        isBoardFull(matrix, nextBoardIndex))
                    
                        mustPlayBoardIndex = -1; // Libera a jogada para qualquer tabuleiro

                    // Alterna o jogador
                    if (currentPlayer == BoardMark::XMARK)
                        currentPlayer = BoardMark::OMARK;
                    else
                        currentPlayer = BoardMark::XMARK;
                }
            }
        }
    }
}


int main(void) 
{
    int windowSize = 600;
    InitWindow(windowSize, windowSize, "Ultimate Tic Tac Toe");
    SetTargetFPS(60);

    initWelcomeButtons();

    printf("Tentando carregar jogo salvo...\n");
    int loadingSucess = loadGame(&estado_io, NOME_ARQUIVO);

    if (loadingSucess) {
        printf("Jogo carregado com sucesso! Convertendo para 3D/2D.\n");
        structToMatrix(&estado_io, matrix, winnerMatrix, &mustPlayBoardIndex, &currentPlayer); 
        
        // Verifica o estado do jogo carregado
        globalWinner = detectGlobalVictory(winnerMatrix);
        
        if(globalWinner != BoardMark :: EMPTY)
            currentScreen = GameScreen :: END;
        else 
            currentScreen = GameScreen :: GAME;     
    } 
    else 
    {
        printf("Nenhum jogo salvo encontrado. Iniciando Welcome Screen.\n");
        currentScreen = GameScreen :: WELCOME;
    }
    
    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Lógica de Estado (Welcome)
        if (currentScreen == GameScreen :: WELCOME) 
        {
            int selected = drawWelcomeScreen();
            
            if (selected != -1) 
            {
                init(matrix, winnerMatrix); 
                currentScreen = GameScreen :: GAME; // Sai da tela de inicio
                isRobotGame = false; // Reset da flag do robô

                switch(selected) 
                {
                    case 0: /* jogador X */ break;
                    case 1: /* jogador O */ break;
                    case 2: /* jogar contra robô */ 
                    isRobotGame = true;
                    break;
                }
            }
        } 
        
        // Lógica de Estado (Game)
        else if (currentScreen == GameScreen :: GAME) 
        {
            drawUltimateBoard();
            drawMarks();
            cleanmatrix(matrix, mustPlayBoardIndex);

            // Implementação do modo jogar contra Robô
            if(isRobotGame && currentPlayer == BoardMark :: OMARK)
            {
                WaitTime(0.5);
                robotPlay(matrix,winnerMatrix,mustPlayBoardIndex,currentPlayer);
            }
            else if (!isRobotGame || (isRobotGame && currentPlayer == BoardMark :: XMARK))
                handleMouseInput();

            // Verificação da vitória global 
            globalWinner = detectGlobalVictory(winnerMatrix);
            if(globalWinner != BoardMark :: EMPTY)
                currentScreen = GameScreen :: END; // Transição para a tela final
            
        }
        
        // Lógica de Estado (End)
        else if (currentScreen == GameScreen :: END)
            drawEndScreen();

        EndDrawing();
    }

   if (currentScreen == GameScreen::GAME) 
    // Se o jogo ESTAVA ativo (true), a função recebe false (o jogo parou/terminou).
    askForSave(false); 
  else 
    // Se o jogo NÃO ESTAVA ativo (false), a função recebe true (o jogo parou/terminou).
    askForSave(true);

    CloseWindow();
}