/** @file antenna_types.h
 * @brief Antenna interference analysis project.
 * @author Filipe Fernandes
 */
#ifndef ANTENNA_TYPES_H
#define ANTENNA_TYPES_H

typedef struct Antenna {
    char frequency;     
    int x, y;            
    struct Antenna *next; 
} Antenna;

 typedef struct {
    char type;  
    int x;
    int y;
} BinaryRecord;
#endif