/** @file main.c
 * @brief Antenna interference analysis project.
 * @author Filipe Fernandes
 */
#include <stdio.h>
#include <stdbool.h>
#include "antennas.h"

int main() {
    Antenna* list = NULL;
    int rows = 0, columns = 0;

    const char* input_filename = "antennas.txt";

    list = load_antennas_from_file(input_filename, &rows, &columns);
    if (!list) {
        printf("Could not load antennas from the input file.\n");
        return 1;
    }

    printf("Antennas loaded from grid (%d rows x %d columns):\n", rows, columns);
    list_antennas(list);

    print_interference_grid(list, rows, columns);

    if (save_antennas_binary("antennas.bin", list))
        printf("\nAntennas successfully saved to 'antennas.bin'.\n");
    else
        printf("\nCould not save antennas to the binary file.\n");

    return 0;
}
