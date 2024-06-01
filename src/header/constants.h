#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT  750

#define TILESIZE 96
#define TILENUMX (SCREEN_WIDTH / (2 * TILESIZE)) + 1
#define TILENUMY (SCREEN_HEIGHT / (2 * TILESIZE)) + 1

#define FPS 60

#define PLAYERSPEED (60 * TILESIZE) / (16 * FPS)


#endif