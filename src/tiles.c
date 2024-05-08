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
    numTiles = 2;
    nameLen = 20;
    SDL_Surface* surface = NULL;
    char path[15 + nameLen];

    t->names = malloc(numTiles * sizeof(char*));
    t->textures = malloc(numTiles * sizeof(SDL_Texture*));

    for (int i = 0; i < numTiles; i++) {
        t->names[i] = malloc(nameLen * sizeof(char));
    }
    strcpy(t->names[0], "grass.png");
    strcpy(t->names[1], "stone.png");

    //Load Textures
    for (int i = 0; i < numTiles; i++) {
        strcpy(path, "src/sprites/");
        strcat(path, t->names[i]);
        surface = IMG_Load(path);
        t->textures[i] = SDL_CreateTextureFromSurface(r, surface);
        SDL_FreeSurface(surface);
    }

}

void cleanTiles(Tiles *t) {
    for (int i = 0; i < numTiles; i++) {
        free(t->textures[i]);
        free(t->names[i]);
    }
    free(t->names);
    free(t->textures);   
}

void renderTile(SDL_Renderer *r, SDL_Texture *t, SDL_Rect *rc) {
    SDL_RenderCopy(r, t, NULL, rc);
}