#include <stdio.h>
#include <stdlib.h>
#include "header/constants.h"
#include "header/player.h"
#include "header/level.h"

typedef struct {
    int x;
    int y;
} Camera;

void initCamera(Camera *c, int x, int y) {
    c->x = x;
    c->y = y;
}

void setCameraPosition(Camera *c, int x, int y) {
    c->x = x;
    c->y = y;
}

void calculateCameraPosition(Camera *c, Player *p) {
    int difX = p->x - c->x;
    int difY = p->y - c->y;
    int rangeX = (TILESIZE / 16) * 16;
    int rangeY = (TILESIZE / 16) * 10;

    if (abs(difX) > rangeX) {
        c->x = p->x - ((difX / abs(difX)) * rangeX);
    }
    if (abs(difY) > rangeY) {
        c->y = p->y - ((difY / abs(difY)) * rangeY);
    }

}

//Camera control for the Level editor
void calculateCameraPositionEditor(Camera *c, Player *p) {
    c->x = p->x;
    c->y = p->y;
}