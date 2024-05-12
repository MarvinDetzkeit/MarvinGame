#include <stdio.h>
#include "header/constants.h"
#include "header/level.h"

typedef struct
{
    int x;
    int y;
    int movUp;
    int movDown;
    int movLeft;
    int movRight;
} Player;


void printPlayerPosition(Player* p) {
    printf("Player Position: (%d, %d)\n", p->x, p->y);
}

//move player with collision; Tiles with negative values stored in l->tiles have collision
//for each direction, it checks 2 corners of the player rect
//Collision only works properly for positive coordinates (level coordinates are always positive - levels start at (0, 0))
void movePlayer(Player *p, Level *l) {
    if (p->movUp) {
        p->y -= PLAYERSPEED - ((p->movLeft + p->movRight) * (PLAYERSPEED / 4));
        if (getTile(l, p->x / TILESIZE, p->y / TILESIZE) < 0 || getTile(l, (p->x + TILESIZE-1) / TILESIZE, p->y / TILESIZE) < 0) {
            p->y += TILESIZE - (p->y % TILESIZE);
        }
    }
    if (p->movDown) {
        p->y += PLAYERSPEED - (p->movLeft + p->movRight) * (PLAYERSPEED / 4);
        if (getTile(l, p->x / TILESIZE, (p->y + TILESIZE-1) / TILESIZE) < 0 || getTile(l, (p->x + TILESIZE-1) / TILESIZE, (p->y + TILESIZE-1) / TILESIZE) < 0) {
            p->y -= (p->y % TILESIZE);
        }
    }
    if (p->movLeft) {
        p->x -= PLAYERSPEED - (p->movUp + p->movDown) * (PLAYERSPEED / 4);
        if (getTile(l, p->x / TILESIZE, p->y / TILESIZE) < 0 || getTile(l, p->x / TILESIZE, (p->y + TILESIZE-1) / TILESIZE) < 0) {
            p->x += TILESIZE - (p->x % TILESIZE);
        }
    }
    if (p->movRight) {
        p->x += PLAYERSPEED - (p->movUp + p->movDown) * (PLAYERSPEED / 4);
        if (getTile(l, (p->x + TILESIZE-1) / TILESIZE, p->y / TILESIZE) < 0 || getTile(l, (p->x + TILESIZE-1) / TILESIZE, (p->y + TILESIZE-1) / TILESIZE) < 0) {
            p->x -= (p->x % TILESIZE);
        }
    }
}

//Moving the player without collsion for the editor
void movePlayerEditor(Player *p) {
    if (p->movUp) {
        p->y -= (2 * PLAYERSPEED) - ((p->movLeft + p->movRight) * ((2 * PLAYERSPEED) / 4));
    }
    if (p->movDown) {
        p->y += (2 * PLAYERSPEED) - (p->movLeft + p->movRight) * ((2 * PLAYERSPEED) / 4);
    }
    if (p->movLeft) {
        p->x -= (2 * PLAYERSPEED) - (p->movUp + p->movDown) * ((2 * PLAYERSPEED) / 4);
    }
    if (p->movRight) {
        p->x += (2 * PLAYERSPEED) - (p->movUp + p->movDown) * ((2 * PLAYERSPEED) / 4);
    }
}
