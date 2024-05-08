#ifndef TILES_H
#define TILES_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

// Global variables for tile handling
extern int numTiles;
extern int nameLen;

// Structure to hold tile names and their corresponding textures
typedef struct
{
    char **names;
    SDL_Texture **textures;
    
} Tiles;

// Function to initialize the Tiles structure
void initTiles(Tiles *t, SDL_Renderer *r);

// Function to clean up resources used by Tiles
void cleanTiles(Tiles *t);

// Function to render a tile
void renderTile(SDL_Renderer *r, SDL_Texture *t, SDL_Rect *rc);

#endif // TILES_H
