/*!
 * @file main.c
 * @brief Utilização das funções implementadas em FuncoesAntenas.c
 * @author Filipe
 * @date 2025-03-23
 */


#include <stdio.h>
#include <stdbool.h>
#include "FuncoesAntenas.h"

int main() {
    Antena* lista = NULL;
    int linhas = 0, colunas = 0;

    // Nome do txt
    const char* nomeFicheiroTxt = "antenas.txt";

    // Carregar antenas
    lista = CarregarAntenasDeFicheiro(nomeFicheiroTxt, &linhas, &colunas);
    if (!lista) {
        printf("Erro ao carregar antenas do ficheiro.\n");
        return 1;
    }

    // Mostrar antenas carregadas
    printf("Antenas carregadas da matriz (%d linhas x %d colunas):\n", linhas, colunas);
    ListarAntenas(lista);

    // Calcular e mostrar interferências
    CalcularInterferencias(lista, linhas, colunas);

    // Gravar em ficheiro binário
    if (GravarAntenasBinario("antenas.bin", lista))
        printf("\nAntenas gravadas com sucesso em 'antenas.bin'.\n");
    else
        printf("\nErro ao gravar as antenas no ficheiro binário.\n");

    
    return 0;
}
