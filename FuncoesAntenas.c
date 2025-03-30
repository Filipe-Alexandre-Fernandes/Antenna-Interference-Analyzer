
/*!
 * @file FuncoesAntenas.c
 * @brief Implementação de funções para gestão de antenas com listas ligadas.
 * @author Filipe
 * @date 2025-03-23
 */

#include "FuncoesAntenas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "FuncoesAntenas.h"
/*!
 * @brief Cria uma nova antena.
 * @param freq Frequência da antena (ex: 'A', 'O')
 * @param x Coordenada X da antena
 * @param y Coordenada Y da antena
 * @return Apontador para a nova antena criada
 */
Antena* CriarAntena(char freq, int x, int y) {
    Antena* nova = (Antena*)malloc(sizeof(Antena));
    if (nova != NULL) {
        nova->frequencia = freq;
        nova->x = x;
        nova->y = y;
        nova->prox = NULL;
    }
    return nova;
}

/*!
 * @brief Insere uma antena na lista de forma ordenada por coordenadas y, depois x.
 * @param h Início da lista
 * @param nova Nova antena a inserir
 * @return Início da lista atualizada
 */
Antena* InserirAntenaOrdenada(Antena* h, Antena* nova) {
    if (!nova || ProcurarAntena(h, nova->frequencia, nova->x, nova->y))
        return h;

    if (!h || (nova->y < h->y || (nova->y == h->y && nova->x < h->x))) {
        nova->prox = h;
        return nova;
    }

    Antena *atual = h, *anterior = NULL;
    while (atual && (atual->y < nova->y || (atual->y == nova->y && atual->x < nova->x))) {
        anterior = atual;
        atual = atual->prox;
    }
    anterior->prox = nova;
    nova->prox = atual;
    return h;
}

/*!
 * @brief Procura uma antena específica pela frequência e coordenadas.
 * @param h Início da lista
 * @param freq Frequência da antena
 * @param x Coordenada x
 * @param y Coordenada y
 * @return Apontador para a antena se encontrada, ou NULL
 */
Antena* ProcurarAntena(Antena* h, char freq, int x, int y) {
    while (h) {
        if (h->frequencia == freq && h->x == x && h->y == y)
            return h;
        h = h->prox;
    }
    return NULL;
}

/*!
 * @brief Remove uma antena com base na frequência e coordenadas.
 * @param h Início da lista
 * @param freq Frequência
 * @param x Coordenada x
 * @param y Coordenada y
 * @return Início da lista atualizado
 */
Antena* RemoverAntena(Antena* h, char freq, int x, int y) {
    Antena *aux = h, *anterior = NULL;
    while (aux && (aux->frequencia != freq || aux->x != x || aux->y != y)) {
        anterior = aux;
        aux = aux->prox;
    }
    if (!aux) return h;

    if (!anterior)
        h = aux->prox;
    else
        anterior->prox = aux->prox;

    free(aux);
    return h;
}

/**
 * @brief Lista todas as antenas presentes na lista ligada.
 *
 * Esta função percorre a lista ligada de antenas e imprime no terminal
 * a frequência e as coordenadas de cada uma.
 *
 * @param h Apontador para o início da lista de antenas.
 */
void ListarAntenas(Antena* h) {
    while (h) {
        printf("Frequência: %c | Coordenadas: (%d, %d)\n", h->frequencia, h->y, h->x);
        h = h->prox;
    }
}



/*!
 * @brief Calcula e desenha interferências com base nos vetores entre pares de antenas da mesma frequência.
 *
 * Esta versão usa coordenadas a iniciar no 1, vetores simétricos, e imprime a matriz.
 *
 * @param lista Apontador para a lista de antenas
 * @param linhas Número de linhas da matriz
 * @param colunas Número de colunas da matriz
 */
void CalcularInterferencias(Antena* lista, int linhas, int colunas) {
    char matriz[linhas + 1][colunas + 1];
    // Inicializar a matriz com '.'
    for (int y = 1; y <= linhas; y++) {
        for (int x = 1; x <= colunas; x++) {
            matriz[y][x] = '.';
        }
    }
    // Colocar as antenas na matriz
    for (Antena* a = lista; a != NULL; a = a->prox) {
        if (a->x >= 1 && a->x <= colunas && a->y >= 1 && a->y <= linhas)
            matriz[a->y][a->x] = a->frequencia;
    }
    // Calcular interferências com base em pares de antenas
    for (Antena* a1 = lista; a1 != NULL; a1 = a1->prox) {
        for (Antena* a2 = a1->prox; a2 != NULL; a2 = a2->prox) {
            if (a1->frequencia == a2->frequencia) {
                int dx = a2->x - a1->x;
                int dy = a2->y - a1->y;
                // A1 - V
                int x1 = a1->x - dx;
                int y1 = a1->y - dy;
                if (x1 >= 1 && x1 <= colunas && y1 >= 1 && y1 <= linhas && matriz[y1][x1] == '.')
                    matriz[y1][x1] = '#';
                // A2 + V
                int x2 = a2->x + dx;
                int y2 = a2->y + dy;
                if (x2 >= 1 && x2 <= colunas && y2 >= 1 && y2 <= linhas && matriz[y2][x2] == '.')
                    matriz[y2][x2] = '#';
            }
        }
    }
    // Mostrar matriz;
printf("\nMatriz com interferências (formato original do ficheiro):\n");
for (int y = 1; y <= linhas; y++) {
    for (int x = 1; x <= colunas; x++) {
        printf("%c", matriz[y][x]);
    }
    printf("\n");
}
}



/*!
 * @brief Lê antenas a partir de ficheiro de texto.
 *         *
 * @param nomeFicheiro Nome do ficheiro
 * @param linhas Ponteiro para armazenar o número de linhas
 * @param colunas Ponteiro para armazenar o número de colunas
 * @return Lista ligada de antenas carregadas
 */
Antena* CarregarAntenasDeFicheiro(const char* nomeFicheiro, int* linhas, int* colunas) {
    FILE* f = fopen(nomeFicheiro, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro");
        return NULL;
    }
    printf("Ficheiro aberto com sucesso. A ler linhas...\n");
    char linha[128];
    int numLinhas = 0;
    int maxColunas = 0;
    Antena* lista = NULL;
    while (fgets(linha, sizeof(linha), f)) {
        int comprimento = 0;
        for (int i = 0; linha[i] != '\0' && linha[i] != '\n'; i++) {
            char ch = linha[i];
            if (ch != '.') {
                printf("Antena encontrada: %c em (%d, %d)\n", ch, numLinhas + 1, i + 1);
                Antena* nova = CriarAntena(ch, i + 1, numLinhas + 1);
                lista = InserirAntenaOrdenada(lista, nova);
            }          
            comprimento++;
        }
        if (comprimento > maxColunas) maxColunas = comprimento;
        numLinhas++;
    }
    fclose(f);
    *linhas = numLinhas;
    *colunas = maxColunas;
    if (!lista) {
        printf("Nenhuma antena foi carregada do ficheiro!\n");
    }
    return lista;
}

/*!
 * @brief Guarda todas as antenas da lista num ficheiro binário.
 * @param nomeFicheiro Nome do ficheiro a criar
 * @param h Apontador para o início da lista ligada de antenas
 * @return true se for bem-sucedido, false caso contrário
 */
 bool GravarAntenasBinario(const char* nomeFicheiro, Antena* lista) {
    if (!lista) return false;
    FILE* fp = fopen(nomeFicheiro, "wb");
    if (!fp) return false;
    // Determinar tamanho da matriz
    int maxX = 0, maxY = 0;
    for (Antena* a = lista; a != NULL; a = a->prox) {
        if (a->x > maxX) maxX = a->x;
        if (a->y > maxY) maxY = a->y;
    }
    // Criar e inicializar matriz
    char matriz[maxY + 1][maxX + 1];
    for (int y = 1; y <= maxY; y++)
        for (int x = 1; x <= maxX; x++)
            matriz[y][x] = '.';
    // Colocar antenas
    for (Antena* a = lista; a != NULL; a = a->prox) {
        matriz[a->y][a->x] = a->frequencia;
    }
    // Calcular interferências
    for (Antena* a1 = lista; a1 != NULL; a1 = a1->prox) {
        for (Antena* a2 = a1->prox; a2 != NULL; a2 = a2->prox) {
            if (a1->frequencia == a2->frequencia) {
                int dx = a2->x - a1->x;
                int dy = a2->y - a1->y;
                int x1 = a1->x - dx;
                int y1 = a1->y - dy;
                int x2 = a2->x + dx;
                int y2 = a2->y + dy;
                if (x1 >= 1 && x1 <= maxX && y1 >= 1 && y1 <= maxY && matriz[y1][x1] == '.')
                    matriz[y1][x1] = '#';

                if (x2 >= 1 && x2 <= maxX && y2 >= 1 && y2 <= maxY && matriz[y2][x2] == '.')
                    matriz[y2][x2] = '#';
            }
        }
    }
    // Gravar tudo o que não for '.' no binário
    RegistoBin reg;
    for (int y = 1; y <= maxY; y++) {
        for (int x = 1; x <= maxX; x++) {
            if (matriz[y][x] != '.') {
                reg.tipo = matriz[y][x];
                reg.x = x;
                reg.y = y;
                fwrite(&reg, sizeof(RegistoBin), 1, fp);
            }
        }
    }
    fclose(fp);
    return true;
}



