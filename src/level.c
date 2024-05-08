#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/constants.h"

char path[50];

typedef struct {
    char name[20];
    int x;
    int y;
    int playerX;
    int playerY;
    int *tiles;
} Level;

void printLevelInformation(Level *l);

void loadLevel(Level *l) {
    strcpy(path, "src/level/");
    strcat(path, l->name);

    //load level from file 
    FILE *file = fopen(path, "r");
    char buffer[6];
    //Load Level size and player position
    if (fgets(buffer, 6, file) == NULL) {
        fprintf(stderr, "Error while loading in level.\n");
        fclose(file);
        return;
    }
    l->x = atoi(buffer);
    if (fgets(buffer, 6, file) == NULL) {
        fprintf(stderr, "Error while loading in level.\n");
        fclose(file);
        return;
    }
    l->y = atoi(buffer);
    if (fgets(buffer, 6, file) == NULL) {
        fprintf(stderr, "Error while loading in level.\n");
        fclose(file);
        return;
    }
    l->playerX = atoi(buffer);
    if (fgets(buffer, 6, file) == NULL) {
        fprintf(stderr, "Error while loading in level.\n");
        fclose(file);
        return;
    }
    l->playerY = atoi(buffer);
    //Load level tiles
    l->tiles = malloc(l->x * l->y * sizeof(int));
    for (int i = 0; i < l->x * l->y; i++) {
        if (fgets(buffer, 6, file) == NULL) {
            fprintf(stderr, "Error while loading in level.\n");
            fclose(file);
            return;
        }
        l->tiles[i] = atoi(buffer);
    }
    printf("Level \'%s\' loaded successfully.\n", l->name);
    fclose(file);
    
}

void unloadLevel(Level *l) {
    free(l->tiles);
    free(l->name);
    //free(l); commented out because space might be used again to load in different level
}

int getTile(Level *l, int x, int y) {
    if (x < 0 || y < 0 || x >= l->x || y >= l->y) {
        //return -1 if position is not on grid
        return -1;
    }
    return l->tiles[x + (l->x * y)];
}

//Get coordinates of TIle on which the player stands on
int getPlayerTileX(Level *l) {
    return l->playerX / TILESIZE;
}

int getPlayerTileY(Level *l) {
    return l->playerY / TILESIZE;
}

void changePlayerPosition(Level *l, int x, int y) {
    l->playerX = l->playerX + x;
    l->playerY = l->playerY + y;
}

void printLevelInformation(Level *l) {
    printf("Name: %s\nDimensionX: %d\nDimensionY: %d\nPlayer Position X: %d\nPlayer Position Y: %d\n",
    l->name,
    l->x,
    l->y,
    l->playerX,
    l->playerY);
}
