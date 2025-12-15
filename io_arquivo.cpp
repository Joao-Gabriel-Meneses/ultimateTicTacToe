#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "estrutura.hpp" 

// Buffer com tamanho seguroa
#define MAX_LINHA_LEN 300 

// Função auxiliar interna para analisar uma linha CSV/texto e preencher um array de BoardMark.
static void analisar_linha_dados(char *line, BoardMark *destinyArray, int maxLenght) {
    const char delimiter[] = ",";
    char *token;
    int index = 0;

    token = strtok(line, delimiter); // Encontra o primeiro delimiter em linha e substitui por \0

    // Se houver mais números na string e o array de destino tem espaço
    while (token != NULL && index < maxLenght) 
    {
        destinyArray[index] = (BoardMark)strtol(token, NULL, 10); // Conversão para BoardMark e armazenamento principal.
        token = strtok(NULL, delimiter); 
        index++;
    }
}

// Função 1: Salvar jogo 
void saveGame(const UltimateTicTacToe *game, const char *fileName) {
    FILE *fp = fopen(fileName, "w");
    if (fp == NULL) { 
        perror("Erro ao abrir arquivo para escrita"); 
        return; 
    }
    
    const int totalsmallcells = PROFUNDIDADE * LINHA * COLUNA;
    const int totalBigBoards = PROFUNDIDADE;

    // 1: Tabuleiros Menores (81 estados) 
    for (int i = 0; i < totalsmallcells; i++) 
    {
        fprintf(fp, "%d", (int)game->tabuleirosMenores[i]);
        if (i < totalsmallcells - 1)     
            fprintf(fp, ",");
    
    }
    fprintf(fp, "\n"); 

    // 2: Tabuleiro Principal (9 estados)
    for (int i = 0; i < totalBigBoards; i++) 
    {
        fprintf(fp, "%d", (int)game->tabuleiroPrincipal[i]);
        if (i < totalBigBoards - 1) 
            fprintf(fp, ",");
    }
    fprintf(fp, "\n"); 

    // 3: Variáveis de Controle 
    // Escreve proximoJogador e proximoTabuleiro
    fprintf(fp, "%d,%d\n", (int)game->proximoJogador, game->proximoTabuleiro);
    
    fclose(fp);
}


// Função 2: Carregar o jogo 
int loadGame(UltimateTicTacToe *game, const char *fileName)
 {
    FILE *fp = fopen(fileName, "r");
    char buffer[MAX_LINHA_LEN];
    
    if (fp == NULL) 
        return 0; 
    
    const int totalsmallcells = PROFUNDIDADE * LINHA * COLUNA;
    const int totalBigBoards = PROFUNDIDADE;

    memset(game, 0, sizeof(UltimateTicTacToe)); 

    // 1: Tabuleiros Menores (81) 
    if (fgets(buffer, sizeof(buffer), fp) != NULL) 
    {
        buffer[strcspn(buffer, "\n")] = 0; 
        analisar_linha_dados(buffer, game->tabuleirosMenores, totalsmallcells);
    } 
    else 
    { 
        fclose(fp); 
        return 0; 
    }

    // 2: Tabuleiro Principal (9)
    if (fgets(buffer, sizeof(buffer), fp) != NULL) 
    {
        buffer[strcspn(buffer, "\n")] = 0;
        analisar_linha_dados(buffer, game->tabuleiroPrincipal, totalBigBoards);
    } 
    else 
    { 
        fclose(fp); 
        return 0; 
    }

    // 3: Variáveis de Controle 
    if (fgets(buffer, sizeof(buffer), fp) != NULL) 
    {
        int currentP_val, mustPlay_val;
        
        // sscanf é mais seguro que fscanf em arquivos abertos
        if (sscanf(buffer, "%d,%d", &currentP_val, &mustPlay_val) == 2) 
        {
            game->proximoJogador = (BoardMark)currentP_val;
            game->proximoTabuleiro = mustPlay_val;
        } 
        else 
        { 
            fclose(fp); 
            return 0; // Falha na leitura das variáveis
        }
    } 
    else 
    { 
        fclose(fp); 
        return 0; // Falha na leitura da linha
    }

    fclose(fp);
    return 1; // Sucesso
}

// Função 3: Deletar jogo não salvo

void DeleteGame(const char *fileName){
    if (remove(fileName) == 0) 
        printf("Arquivo de salvamento anterior deletado com sucesso.\n");
    else 
        perror("Aviso: Não foi possível deletar o arquivo salvo (pode não existir)");
}