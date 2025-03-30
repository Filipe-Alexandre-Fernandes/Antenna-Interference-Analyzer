#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

/*!
 * @file estruturas.h
 * @brief Definição das estruturas necessárias para o projeto;
 * @author Filipe
 * @date 2025-03-23
 */



/**
 * @brief Estrutura que representa uma antena com frequência e coordenadas.
 */
typedef struct Antena {
    char frequencia;     /** Frequência da antena */
    int x, y;            /** Coordenadas da antena (x, y) */
    struct Antena *prox; /** Ponteiro para a próxima antena na lista ligada */
} Antena;

/*!
 * @brief Estrutura auxiliar para ficheiro binário.
 */
 typedef struct {
    char tipo;  // pode ser frequência da antena ou '#' para interferência
    int x;
    int y;
} RegistoBin;
#endif