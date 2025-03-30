
/*!
 * @file FuncoesAntenas.h
 * @brief Cabeçalho das funções de gestão de antenas com listas ligadas.
 * @author Filipe
 * @date 2025-03-23
 */


#ifndef FUNCOES_ANTENAS_H
#define FUNCOES_ANTENAS_H

#include <stdbool.h>
#include "estruturas.h"

// Prototipagem de funções
Antena* CriarAntena(char freq, int x, int y);
Antena* InserirAntenaOrdenada(Antena* h, Antena* nova);
Antena* ProcurarAntena(Antena* h, char freq, int x, int y);
Antena* RemoverAntena(Antena* h, char freq, int x, int y);
void ListarAntenas(Antena* h);
void CalcularInterferencias(Antena* lista, int linhas, int colunas);
Antena* CarregarAntenasDeFicheiro(const char* nomeFicheiro, int* linhas, int* colunas);
bool GravarAntenasBinario(const char* nomeFicheiro, Antena* h);



#endif
