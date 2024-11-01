//Functions for water animation
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "header/constants.h"
#include "header/game.h"

#define WATERTILE 148
#define SANDWATERTILE 147

SDL_Texture** waterAnimation;
SDL_Texture** sandWaterAnimation;

//Store the water and sandwater Tiles
SDL_Texture* water0storage;
SDL_Texture* sandWater0storage;

//call after initTiles
void initFrames(SDL_Renderer* r) {
    waterAnimation = malloc(8 * sizeof(SDL_Texture*));
    sandWaterAnimation = malloc(8 * sizeof(SDL_Texture*));
    SDL_Surface* s = NULL;
    char path1[] = "src/data/sprites/waterX.png";
    char path2[] = "src/data/sprites/sandWaterX.png";
    
    for (int i = 0; i < 8; i++) {
        path1[22] = '0' + i;
        path2[26] = '0' + i;
        s = IMG_Load(path1);
        waterAnimation[i] = SDL_CreateTextureFromSurface(r, s);
        SDL_FreeSurface(s);
        s = IMG_Load(path2);
        sandWaterAnimation[i] = SDL_CreateTextureFromSurface(r, s);
        SDL_FreeSurface(s);
    }

    water0storage = tiles->textures[WATERTILE];
    sandWater0storage = tiles->textures[SANDWATERTILE];

    printf("Frames for water animation initialised.\n");
}

void animateWater() {
    static int animationFrame = 0;
    static int whichFrame = 0;
    animationFrame++;
    if ((animationFrame % (FPS / 2))) return; //Only change frame each 500ms
    whichFrame++;
    whichFrame = whichFrame % 8;

    tiles->textures[WATERTILE] = waterAnimation[whichFrame];
    tiles->textures[SANDWATERTILE] =sandWaterAnimation[whichFrame];
}

//call before cleanTiles
void cleanFrames() {
    for (int i = 0; i < 8; i++) {
        SDL_DestroyTexture(waterAnimation[i]);
        SDL_DestroyTexture(sandWaterAnimation[i]);
    }
    free(waterAnimation);
    free(sandWaterAnimation);

    tiles->textures[WATERTILE] = water0storage;
    tiles->textures[SANDWATERTILE] = sandWater0storage;

    printf("Cleaned Frames.\n");
}