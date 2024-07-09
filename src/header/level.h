#ifndef LEVEL_H
#define LEVEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration of the Level struct
typedef struct {
    char name[20];
    int x;
    int y;
    int playerX;
    int playerY;
    int *tiles;
} Level;

// Function prototypes
void loadLevel(Level *l);
void unloadLevel(Level *l);
int getTile(Level *l, int x, int y);
int tileHasCollision(int tile);
int tileGetObject(int tile);
int tileGetTeleporter(int tile);
int getPlayerTileX(Level *l);
int getPlayerTileY(Level *l);
void changePlayerPosition(Level *l, int x, int y);
void printLevelInformation(Level *l);

#endif // LEVEL_H
