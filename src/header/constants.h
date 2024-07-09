#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SCREEN_WIDTH  1216
#define SCREEN_HEIGHT  736

#define TILESIZE 92
#define TILENUMX (SCREEN_WIDTH / (2 * TILESIZE)) + 1
#define TILENUMY (SCREEN_HEIGHT / (2 * TILESIZE)) + 1

#define FPS 60

#define PLAYERSPEED (60 * TILESIZE) / (16 * FPS)


#endif