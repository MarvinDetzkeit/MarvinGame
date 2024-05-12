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
    numTiles = 28+1;
    nameLen = 20;
    SDL_Surface* surface = NULL;
    char path[15 + nameLen];

    t->names = malloc(numTiles * sizeof(char*));
    t->textures = malloc(numTiles * sizeof(SDL_Texture*));

    for (int i = 1; i < numTiles; i++) {
        t->names[i] = malloc(nameLen * sizeof(char));
    }
    //skip index 0 because indices have to be negatable for collision detection and -0 == 0
    strcpy(t->names[1], "lines.png");
    strcpy(t->names[2], "grass.png");
    strcpy(t->names[3], "grass1.png");
    strcpy(t->names[4], "grass2.png");
    strcpy(t->names[5], "grass3.png");
    strcpy(t->names[6], "grass4.png");
    strcpy(t->names[7], "grass5.png");
    strcpy(t->names[8], "grassRock.png");
    strcpy(t->names[9], "dirtGrass.png");
    strcpy(t->names[10], "dirtGrass1.png");
    strcpy(t->names[11], "dirtGrass2.png");
    strcpy(t->names[12], "dirtGrass3.png");
    strcpy(t->names[13], "dirtGrass4.png");
    strcpy(t->names[14], "dirtGrass5.png");
    strcpy(t->names[15], "dirtGrass6.png");
    strcpy(t->names[16], "dirtGrass7.png");
    strcpy(t->names[17], "dirtGrass8.png");
    strcpy(t->names[18], "dirtGrass9.png");
    strcpy(t->names[19], "treeStump.png");
    strcpy(t->names[20], "stoneGrass.png");
    strcpy(t->names[21], "stoneGrass1.png");
    strcpy(t->names[22], "stoneGrass2.png");
    strcpy(t->names[23], "stoneGrass3.png");
    strcpy(t->names[24], "stoneGrass4.png");
    strcpy(t->names[25], "stoneGrass5.png");
    strcpy(t->names[26], "stoneGrass6.png");
    strcpy(t->names[27], "stoneGrass7.png");
    strcpy(t->names[28], "stone.png");

    //Load Textures
    for (int i = 1; i < numTiles; i++) {
        strcpy(path, "src/sprites/");
        strcat(path, t->names[i]);
        surface = IMG_Load(path);
        t->textures[i] = SDL_CreateTextureFromSurface(r, surface);
        SDL_FreeSurface(surface);
    }
    printf("Tiles loaded successfully.\n");

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