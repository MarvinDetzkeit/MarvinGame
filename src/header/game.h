#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "constants.h"
#include "player.h"
#include "camera.h"
#include "level.h"
#include "tiles.h"
#include "uiwidgets.h"
#include "items.h"

// Function declarations
void handleTeleportation();
int updateGame(void *ptr);
void delay(void);
void positionOnScreen(int x, int y);
void renderGame(void);
void cleanUp(void);
int initialize(void);

// Global variables
extern Player *player;
extern SDL_Rect playerObj;
extern SDL_Texture *playerSprite;

extern Item **items;

extern Camera *camera;

extern int screenX;
extern int screenY;

extern Level *level;
extern SDL_Rect levelObj;
extern Tiles *tiles;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern int running;
extern Uint32 time_last_frame;

extern int (*update)(void *ptr);
extern void (*render)();
extern void *updatePTR;

#endif // GAME_H
