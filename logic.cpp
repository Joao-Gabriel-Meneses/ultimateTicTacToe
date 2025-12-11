#include <stdio.h>
#define LINHA 3
#define COLUNA 3
#define PROFUNDIDADE 9

enum class BoardMark : char
{
    EMPTY = '-',
    XMARK = 'X',
    OMARK = 'O'
};

bool detect_victory(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int index_played)
{
    int possibilities[8][3][2] = {
        // Horizontais
        {{0, 0},
         {0, 1},
         {0, 2}},

        {{1, 0},
         {1, 1},
         {1, 2}},

        {{2, 0},
         {2, 1},
         {2, 2}},
        // Verticais
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
    {
        for (int i = 0; i < LINHA; i++)
        {
            for (int j = 0; j < COLUNA; j++)
            {
                matrix[z][i][j] = BoardMark::EMPTY;
            }
        }
    }
}

void play(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA], int max_plays)
{
    for (int i = 0; i < max_plays; i++)
    {
        if (i % 2 == 0)
        {
            // par é jogador 1
        }
        else
        {
            // impar é jogador 2
        }
    }
}
void print(BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA])
{
    for (int z = 0; z < PROFUNDIDADE; z++)
    {
        printf("Mini Tabuleiro %d: \n", z);
        for (int j = 0; j < LINHA; j++)
        {
            for (int i = 0; i < COLUNA; i++)
            {
                printf("%c ", (char)matrix[z][j][i]);
            }
            printf("\n");
        }
    }
}

int main(void)
{
    int tam = 3;
    int max_plays = tam * tam * tam * tam;
    BoardMark matrix[PROFUNDIDADE][LINHA][COLUNA];
    init(matrix);
    print(matrix);

    return 0;
}