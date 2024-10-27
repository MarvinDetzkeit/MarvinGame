#ifndef ITEMS_H
#define ITEMS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string.h>

// Definition der Struktur Item
typedef struct {
    int id;
    char name[50];
    char text[500];
    SDL_Texture *sprite;
} Item;

// Deklaration der externen Variablen
extern SDL_Texture *itemSprite;

// Funktionsdeklarationen
void initItems(SDL_Renderer *r, Item **items);
void renderItemScreen(void);
int updateItemScreen(void *ptr);

#endif // ITEMS_H