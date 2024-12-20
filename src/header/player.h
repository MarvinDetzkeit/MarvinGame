#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "constants.h"
#include "level.h"

typedef struct
{
    int x;
    int y;
    int movUp;
    int movDown;
    int movLeft;
    int movRight;
    SDL_Texture *playerSprite;
    SDL_Texture **sprites;
    int items[NUMOFITEMS];
} Player;

void printPlayerPosition(Player* p);

int getPlayerTile(Player *p, Level *l);

void movePlayer(Player *p, Level *l);

void movePlayerEditor(Player *p);

void initPlayer(SDL_Renderer *r, Player *p, Level *l, SDL_Rect *rect);

void cleanPlayer(Player *p);

#endif
