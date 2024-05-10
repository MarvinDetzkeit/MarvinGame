#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

int numTiles;
int nameLen;

typedef struct
{
    char **names;
    SDL_Texture **textures;
    
} Tiles;


void initTiles(Tiles *t, SDL_Renderer *r) {
    numTiles = 2+1;
    nameLen = 20;
    SDL_Surface* surface = NULL;
    char path[15 + nameLen];

    t->names = malloc(numTiles * sizeof(char*));
    t->textures = malloc(numTiles * sizeof(SDL_Texture*));

    for (int i = 1; i < numTiles; i++) {
        t->names[i] = malloc(nameLen * sizeof(char));
    }
    //skip index 0 because indices have to be negatable for collision detection and -0 == 0
    strcpy(t->names[1], "grass.png");
    strcpy(t->names[2], "stone.png");

    //Load Textures
    for (int i = 1; i < numTiles; i++) {
        strcpy(path, "src/sprites/");
        strcat(path, t->names[i]);
        surface = IMG_Load(path);
        t->textures[i] = SDL_CreateTextureFromSurface(r, surface);
        SDL_FreeSurface(surface);
    }

}

void cleanTiles(Tiles *t) {
    for (int i = 1; i < numTiles; i++) {
        SDL_DestroyTexture(t->textures[i]);
        free(t->names[i]);
    }
    free(t->names);
    free(t->textures);   
}

void renderTile(SDL_Renderer *r, SDL_Texture *t, SDL_Rect *rc) {
    SDL_RenderCopy(r, t, NULL, rc);
}