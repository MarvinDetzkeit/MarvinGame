#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"

typedef struct
{
    int x;
    int y;
    int movUp;
    int movDown;
    int movLeft;
    int movRight;
} Player;

void printPlayerPosition(Player* p);

void movePlayer(Player *p, Level *l);

void movePlayerEditor(Player *p);

#endif
