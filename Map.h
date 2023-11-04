//
// Created by user on 01.11.2023.
//

#ifndef EX2_MAP_H
#define EX2_MAP_H
#define MAX_SIZE                                           \
    100 // Maximum number of elements in the map
typedef struct Map{
    int size; // Current number of elements in the map
    char keys[MAX_SIZE][100]; // Array to store the keys
    double values[MAX_SIZE];
    void (*print)(struct Map *);
    void (*insert)(char[],double ,struct Map *);
    int (*getIndex)(char [],struct Map *);
} Map;
Map *new();
void insertMap(char[],double ,Map *);
int getIndexMap(char [],Map *);
void printMap(Map *);

#endif //EX2_MAP_H
