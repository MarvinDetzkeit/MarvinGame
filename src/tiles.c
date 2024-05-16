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
    numTiles = 0;
    nameLen = 20;
    SDL_Surface* surface = NULL;
    char path[15 + nameLen];

    char **nameBuffer = malloc(500 * sizeof(char*));
    strcpy(path, "src/data/TileNames.txt");
    FILE *f = fopen(path, "r");
    if (f) printf("File not NULL\n");

    size_t cmp = 4;
    while (1) {
        nameBuffer[numTiles] = malloc(sizeof(char) * numTiles);
        fgets(nameBuffer[numTiles], nameLen, f);
        nameBuffer[numTiles][strcspn(nameBuffer[numTiles], "\n")] = 0;
        numTiles++;
        if (strlen(nameBuffer[numTiles-1]) <= cmp) {
            free(nameBuffer[numTiles-1]);
            break;
        }
    }
    fclose(f);
    f = NULL;
    printf("Loaded Tile names in. %d\n", numTiles);

    t->names = malloc((numTiles * sizeof(char*)) + 1);
    t->textures = malloc((numTiles * sizeof(SDL_Texture*)) + 1);

    for (int i = 1; i < numTiles; i++) {
        t->names[i] = malloc(nameLen * sizeof(char));
        strcpy(t->names[i], nameBuffer[i-1]);
        free(nameBuffer[i-1]);
    }
    free(nameBuffer);

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