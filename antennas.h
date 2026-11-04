/** @file antennas.h
 * @brief Antenna interference analysis project.
 * @author Filipe Fernandes
 */
#ifndef ANTENNAS_H
#define ANTENNAS_H

#include <stdbool.h>
#include "antenna_types.h"

Antenna* create_antenna(char freq, int x, int y);
Antenna* insert_antenna_sorted(Antenna* h, Antenna* new_antenna);
Antenna* find_antenna(Antenna* h, char freq, int x, int y);
Antenna* remove_antenna(Antenna* h, char freq, int x, int y);
void list_antennas(Antenna* h);
void print_interference_grid(Antenna* list, int rows, int columns);
Antenna* load_antennas_from_file(const char* filename, int* rows, int* columns);
bool save_antennas_binary(const char* filename, Antenna* h);

#endif
