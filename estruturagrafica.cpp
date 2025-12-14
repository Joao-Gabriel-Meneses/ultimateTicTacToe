#include "raylib.h"
#include <stdio.h>
#include "estrutura.hpp"

BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA];
BoardMark currentPlayer;
int mustPlayBoardIndex = -1;
bool isRobotGame;

const int screenSize = 600;
const int bigCellSize = screenSize / 3;
const int smallCellSize = bigCellSize / 3;

// Implementação de Menu Interativo
enum class GameScreen {
    WELCOME,
    GAME
};

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
            if(i == 0 || i == 2)
                currentPlayer = BoardMark :: XMARK;
            if(i == 1)
                currentPlayer = BoardMark :: OMARK;

            clickedButton = i;
        }
    }

    return clickedButton; // retorna 0, 1 ou 2 se clicado; -1 se nenhum
}

void drawUltimateBoard() {

    if(mustPlayBoardIndex != -1){
        int targetBigLin = mustPlayBoardIndex / 3;
        int targetBigCol = mustPlayBoardIndex % 3;

        int startCol = targetBigCol * bigCellSize;
        int startLin = targetBigLin * bigCellSize;

        DrawRectangle(startCol, startLin, bigCellSize, bigCellSize, YELLOW);
    }

    // Tabuleiro grande
    for (int i = 1; i < 3; i++) {
        DrawLine(i * bigCellSize, 0, i * bigCellSize, screenSize, BLACK);
        DrawLine(0, i * bigCellSize, screenSize, i * bigCellSize, BLACK);
    }

    // Mini-tabuleiros
    for (int bigLin = 0; bigLin < LINHA; bigLin++) {
        for (int bigCol = 0; bigCol < COLUNA; bigCol++) {
            for (int y = 1; y < 3; y++) {
                DrawLine(bigCol * bigCellSize, bigLin * bigCellSize + y * smallCellSize,
                         (bigCol + 1) * bigCellSize, bigLin * bigCellSize + y * smallCellSize, GRAY);
                DrawLine(bigCol * bigCellSize + y * smallCellSize, bigLin * bigCellSize,
                         bigCol * bigCellSize + y * smallCellSize, (bigLin + 1) * bigCellSize, GRAY);
            }
        }
    }
}

void drawMarks() {
    // Linhas do tabuleiro grande
    for (int bigLin = 0; bigLin < LINHA; bigLin++) {
        // Colunas do tabuleiro grande
        for (int bigCol = 0; bigCol < COLUNA; bigCol++) {
            int boardIndex = bigLin * 3 + bigCol; 
            
            // linhas dentro do mini-tabuleiro
            for (int smallLin = 0; smallLin < LINHA; smallLin++) {
                // coluna dentro do mini-tabuleiro
                for (int smallCol = 0; smallCol < COLUNA; smallCol++) {
                    int centerHorizontal = bigCol * bigCellSize + smallCol * smallCellSize + smallCellSize / 2;
                    int centerVertical = bigLin * bigCellSize + smallLin * smallCellSize + smallCellSize / 2;
                    
                    if (matrix[boardIndex][smallLin][smallCol] == BoardMark::XMARK) {
                        DrawLine(centerHorizontal - 20, centerVertical - 20, centerHorizontal + 20, centerVertical + 20, RED);
                        DrawLine(centerHorizontal - 20, centerVertical + 20, centerHorizontal + 20, centerVertical - 20, RED);
                    } else if (matrix[boardIndex][smallLin][smallCol] == BoardMark::OMARK) {
                        DrawCircleLines(centerHorizontal, centerVertical, 20, BLUE);
                    }
                }
            }
        }
    }
}

// Funções de input
void handleMouseInput(){
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        int bigCol = mouse.x / bigCellSize;
        int bigLin = mouse.y / bigCellSize;
        int smallCol = (int(mouse.x) % bigCellSize) / smallCellSize;
        int smallLin = (int(mouse.y) % bigCellSize) / smallCellSize;

        //converter coordenadas (linha e coluna) em um índice único de uma lista (array)
        int boardIndex = bigLin * 3 + bigCol; 
        int nextPosPlay = smallLin * 3 + smallCol;

        // verificação de limites
        if (bigLin >= 0 && bigLin < 3 && bigCol >= 0 && bigCol < 3) 
        {

            bool IsPlayAllowed = (mustPlayBoardIndex == -1) || (boardIndex == mustPlayBoardIndex); 
            
            if(IsPlayAllowed)
            {    
                // Verifica se a célula está vazia E se o mini-tabuleiro ainda não foi vencido
                if (matrix[boardIndex][smallLin][smallCol] == BoardMark::EMPTY && !detect_victory(matrix, boardIndex) && matrix[nextPosPlay])
                {
                    //Atualiza o mini-tabuleiro obrigatório
                    mustPlayBoardIndex = nextPosPlay; 

                    //Atualiza o jogador
                    matrix[boardIndex][smallLin][smallCol] = currentPlayer;
                    
                    //Regra da Exceção:
                    if(detect_victory(matrix, mustPlayBoardIndex) || isBoardFull(matrix, mustPlayBoardIndex))
                        mustPlayBoardIndex = -1;

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

int main(void) {
    int windowSize = 600;
    InitWindow(windowSize, windowSize, "Ultimate Tic Tac Toe");
    SetTargetFPS(60);

    initWelcomeButtons();

    bool inWelcomeScreen = true;
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (inWelcomeScreen) {
            int selected = drawWelcomeScreen();
            
            if (selected != -1) {
                init(matrix); 
                inWelcomeScreen = false;

                switch(selected) {
                    case 0: /* jogador X */ break;
                    case 1: /* jogador O */ break;
                    case 2: /* jogar contra robô */ 
                    isRobotGame = true;
                    break;
                }
            }
        } else {
            drawUltimateBoard();
            drawMarks();

            // Implementação do modo jogar contra Robô
            if(isRobotGame && currentPlayer == BoardMark :: OMARK)
            {
                WaitTime(0.5);
                robotPlay(matrix,mustPlayBoardIndex,currentPlayer);
            }
            else if (!isRobotGame || (isRobotGame && currentPlayer == BoardMark :: XMARK))
            {
                handleMouseInput();
            }
            // Futuramente: Checar vitória global e desenhar tela de fim de jogo
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
