#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "constants.h"
#include "game.h"

//call after initTiles
void initFrames(SDL_Renderer* r);

void animateWater(void);

//call before cleanTiles
void cleanFrames(void);

#endif // ANIMATION_H