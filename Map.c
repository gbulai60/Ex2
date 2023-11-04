//
// Created by user on 01.11.2023.
//
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <conio.h>
#include "Map.h"
Map *new(){
    Map *map = malloc(sizeof(Map));
    if (map == NULL) {
        fputs("Error: Out of memory\n", stderr);
        exit(1);
    }
    else {
        map->size = 0;
        map->insert = &insertMap;
        map->getIndex = &getIndexMap;
        map->print = &printMap;
        return map;
    }
}
void insertMap(char key[],double value,Map *map){
    int index =map->getIndex(key,map);
    if (index == -1) { // Key not found
        strcpy(map->keys[map->size], key);
        map->values[map->size] = value;
        map->size++;
    }
    else { // Key found
        map->values[index]+= value;
    }

}
int getIndexMap(char key[],Map *map){
    if(map!=NULL) {
        for (int i = 0; i < map->size; i++) {
            if (strcmp(map->keys[i], key) == 0) {
                return i;
            }
        }
    }
    return -1; // Key not found
}

void printMap(Map * map){
    for (int i = 0; i < map->size; i++) {
        printf("BANCA %s: total venit din comisioane - %lf\n", map->keys[i], map->values[i]);
    }
}
