#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include "header/constants.h"
#include "header/game.h"
#include "header/uiwidgets.h"
#include "header/objects.h"

/*
typedef struct {
    int id;
    char name[50];
    char text[500];
    SDL_Texture *sprite;
} Item;
*/
SDL_Texture *itemSprite = NULL;

void initItems(SDL_Renderer *r, Item **items) {
    char path[100];
    char buffer[54];
    strcpy(path, "src/data/ItemNames.txt");
    FILE *f = fopen(path, "r");
    int i = -1;
    while (i < NUMOFITEMS) {
        i++;
        fgets(buffer, 50, f);
        if (strlen(buffer) <= 2) continue;
        items[i] = malloc(sizeof(Item));
        items[i]->id = i;
        strcpy(items[i]->name, buffer);
        items[i]->name[strcspn(items[i]->name, "\n")] = 0;
        strcpy(path, "src/data/sprites/");
        strcat(path, items[i]->name);
        strcat(path, ".png");
        SDL_Surface *s = IMG_Load(path);
        printf("%s\n", path);
        items[i]->sprite = SDL_CreateTextureFromSurface(r, s);
        SDL_FreeSurface(s);
    }
}

void renderItemScreen(void) {
    renderGame();
    renderItemBox(renderer, itemSprite);
}

int updateItemScreen(void *ptr) {
    static uint8_t waitFrames = FPS / 2;
    if (waitFrames) waitFrames--;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 0;
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
                    case SDLK_t:
                        if (waitFrames > 0) break;
                        render = renderGame;
                        update = updateGame;
                        waitFrames = FPS / 2;
                        break;
                    default:
                        break;
                }

        }
    }
    return 1;
}