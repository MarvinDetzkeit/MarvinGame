#ifndef PLAYER_H
#define PLAYER_H

typedef struct
{
    int x;
    int y;
    int movUp;
    int movDown;
    int movLeft;
    int movRight;
} Player;

void printPosition(Player* p);

#endif
