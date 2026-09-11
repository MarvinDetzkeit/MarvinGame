#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "header/constants.h"
#include "header/game.h"

//Transparency value for Text box
# define ALPHA 180
# define INVENTORY_WIDTH (SCREEN_WIDTH - (2 * TILESIZE))
# define INVENTORY_HEIGHT (SCREEN_HEIGHT - (2 * TILESIZE))
# define INVENTORY_LEFT_WIDTH (INVENTORY_WIDTH * 2 / 5)
# define INVENTORY_PADDING 16

typedef struct {
    char name[64];
    char description[1024];
    SDL_Texture *nameTexture;
    SDL_Texture *descriptionTexture;
} InventoryEntry;

static InventoryEntry inventoryEntries[NUMOFITEMS];
static SDL_Texture *unknownTexture = NULL;
static int inventorySelection = 0;
static const int inventoryOrder[NUMOFITEMS] = {7, 0, 6, 1, 3, 4, 5, 2};

SDL_Rect backgroundRect;
SDL_Rect backgroundRectName;
SDL_Rect textRect;
SDL_Rect nameRect;
TTF_Font *textFont;
TTF_Font *nameFont;
SDL_Surface *textSurface;
SDL_Surface *nameSurface;
SDL_Texture *textTure = NULL;
SDL_Texture *nameTexture = NULL;
SDL_Color textColor = {255, 255, 255};

char* name = "Test";

void initUIWidgets(SDL_Renderer *r) {
    textFont = TTF_OpenFont("src/data/ArialBlack.ttf", 24);
    nameFont = TTF_OpenFont("src/data/ArialBlack.ttf", 18);
    backgroundRect.h = 50;
    backgroundRect.w = 8 * SCREEN_WIDTH / 10;
    backgroundRect.x = (SCREEN_WIDTH - backgroundRect.w) / 2;
    backgroundRect.y = SCREEN_HEIGHT - backgroundRect.h - 10;

    FILE *file = fopen("src/data/itemDescriptions.txt", "r");
    char line[1024];
    for (int i = 0; i < NUMOFITEMS; i++) {
        strcpy(inventoryEntries[i].name, "???");
        strcpy(inventoryEntries[i].description, "???");
        inventoryEntries[i].nameTexture = NULL;
        inventoryEntries[i].descriptionTexture = NULL;

        if (file && fgets(line, sizeof(line), file)) {
            inventoryEntries[i].description[0] = 0;

            if (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\r\n")] = 0;
                strncpy(inventoryEntries[i].name, line, sizeof(inventoryEntries[i].name) - 1);
                inventoryEntries[i].name[sizeof(inventoryEntries[i].name) - 1] = 0;
            }
            while (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\r\n")] = 0;
                if (!line[0]) break;
                if (inventoryEntries[i].description[0]) {
                    strncat(inventoryEntries[i].description, "\n",
                        sizeof(inventoryEntries[i].description) - strlen(inventoryEntries[i].description) - 1);
                }
                strncat(inventoryEntries[i].description, line,
                    sizeof(inventoryEntries[i].description) - strlen(inventoryEntries[i].description) - 1);
            }
        }

        SDL_Surface *surface = TTF_RenderUTF8_Blended(nameFont, inventoryEntries[i].name, textColor);
        inventoryEntries[i].nameTexture = SDL_CreateTextureFromSurface(r, surface);
        SDL_FreeSurface(surface);

        surface = TTF_RenderUTF8_Blended_Wrapped(nameFont, inventoryEntries[i].description,
            textColor, INVENTORY_WIDTH - INVENTORY_LEFT_WIDTH - (2 * INVENTORY_PADDING));
        inventoryEntries[i].descriptionTexture = SDL_CreateTextureFromSurface(r, surface);
        SDL_FreeSurface(surface);
    }
    if (file) fclose(file);

    SDL_Surface *surface = TTF_RenderUTF8_Blended(nameFont, "???", textColor);
    unknownTexture = SDL_CreateTextureFromSurface(r, surface);
    SDL_FreeSurface(surface);
}

void createTextTure(SDL_Renderer *r, char *text) {
    if (!textTure) SDL_DestroyTexture(textTure);
    textSurface = TTF_RenderUTF8_Solid(textFont, text, textColor);
    textTure = SDL_CreateTextureFromSurface(r, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_QueryTexture(textTure, NULL, NULL, &(textRect.w), &(textRect.h));
    textRect.x = backgroundRect.x + ((backgroundRect.w - textRect.w) / 2);
    textRect.y = backgroundRect.y + ((backgroundRect.h - textRect.h) / 2);
}

void createNameTexture(SDL_Renderer *r) {
    if (!nameTexture) SDL_DestroyTexture(nameTexture);
    nameSurface = TTF_RenderUTF8_Solid(nameFont, name, textColor);
    nameTexture = SDL_CreateTextureFromSurface(r, nameSurface);
    SDL_FreeSurface(nameSurface);
    SDL_QueryTexture(nameTexture, NULL, NULL, &(nameRect.w), &(nameRect.h));
    nameRect.x = backgroundRect.x + ((backgroundRect.w - nameRect.w) / 2);
    nameRect.y = backgroundRect.y - nameRect.h - 5;
    backgroundRectName.x = nameRect.x - 2;
    backgroundRectName.y = nameRect.y - 2;
    backgroundRectName.h = nameRect.h + 4;
    backgroundRectName.w = nameRect.w + 4;
}

void renderNameBox(SDL_Renderer *r) {
    SDL_SetRenderDrawColor(r, 0, 0, 0, ALPHA);
    SDL_RenderFillRect(r, &backgroundRectName);
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDrawRect(r, &backgroundRectName);
    SDL_RenderCopy(r, nameTexture, NULL, &nameRect);
}

void renderTextBox(SDL_Renderer *r) {
    SDL_SetRenderDrawColor(r, 0, 0, 0, ALPHA);
    SDL_RenderFillRect(r, &backgroundRect);
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDrawRect(r, &backgroundRect);
    SDL_RenderCopy(r, textTure, NULL, &textRect);
}

void renderItemBox(SDL_Renderer *r, SDL_Texture *itemSprite) {
    SDL_Rect box = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SDL_Rect itemRec = {(SCREEN_WIDTH - ITEMSIZE) / 2, (SCREEN_HEIGHT - ITEMSIZE) / 2, ITEMSIZE, ITEMSIZE};
    SDL_SetRenderDrawColor(r, 0, 0, 0, ALPHA);
    SDL_RenderFillRect(r, &box);
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDrawRect(r, &box);
    SDL_RenderCopy(r, itemSprite, NULL, &itemRec);
}

int updateInventory(void *ptr) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 0;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
                    case SDLK_i:
                        inventorySelection = 0;
                        update = updateGame;
                        render = renderGame;
                        break;
                    case SDLK_w:
                    case SDLK_UP:
                        if (inventorySelection > 0) inventorySelection--;
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        if (inventorySelection < NUMOFITEMS - 1) inventorySelection++;
                        break;
                }
                break;
        }
    }
    return 1;
}

void renderInventory(void) {
    renderGame();

    SDL_Rect inventoryRect = {
        (SCREEN_WIDTH - INVENTORY_WIDTH) / 2,
        (SCREEN_HEIGHT - INVENTORY_HEIGHT) / 2,
        INVENTORY_WIDTH,
        INVENTORY_HEIGHT
    };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, ALPHA);
    SDL_RenderFillRect(renderer, &inventoryRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &inventoryRect);
    SDL_RenderDrawLine(renderer, inventoryRect.x + INVENTORY_LEFT_WIDTH, inventoryRect.y,
        inventoryRect.x + INVENTORY_LEFT_WIDTH, inventoryRect.y + inventoryRect.h);

    int rowHeight = (INVENTORY_HEIGHT - (2 * INVENTORY_PADDING)) / NUMOFITEMS;
    for (int i = 0; i < NUMOFITEMS; i++) {
        SDL_Rect row = {
            inventoryRect.x + INVENTORY_PADDING,
            inventoryRect.y + INVENTORY_PADDING + (i * rowHeight),
            INVENTORY_LEFT_WIDTH - (2 * INVENTORY_PADDING),
            rowHeight
        };
        if (i == inventorySelection) SDL_RenderDrawRect(renderer, &row);

        int itemId = inventoryOrder[i];
        SDL_Texture *texture = player->items[itemId]
            ? inventoryEntries[itemId].nameTexture : unknownTexture;
        SDL_Rect text;
        SDL_QueryTexture(texture, NULL, NULL, &text.w, &text.h);
        text.x = row.x + INVENTORY_PADDING;
        text.y = row.y + ((row.h - text.h) / 2);
        SDL_RenderCopy(renderer, texture, NULL, &text);
    }

    int itemId = inventoryOrder[inventorySelection];
    int rightX = inventoryRect.x + INVENTORY_LEFT_WIDTH;
    int rightWidth = INVENTORY_WIDTH - INVENTORY_LEFT_WIDTH;
    SDL_Rect icon = {
        rightX + ((rightWidth - ITEMSIZE) / 2),
        inventoryRect.y + INVENTORY_PADDING,
        ITEMSIZE,
        ITEMSIZE
    };
    if (player->items[itemId]) {
        SDL_RenderCopy(renderer, items[itemId]->sprite, NULL, &icon);
    }

    SDL_Texture *description = player->items[itemId]
        ? inventoryEntries[itemId].descriptionTexture : unknownTexture;
    SDL_Rect descriptionRect;
    SDL_QueryTexture(description, NULL, NULL, &descriptionRect.w, &descriptionRect.h);
    descriptionRect.x = rightX + ((rightWidth - descriptionRect.w) / 2);
    descriptionRect.y = icon.y + icon.h + INVENTORY_PADDING;
    SDL_RenderCopy(renderer, description, NULL, &descriptionRect);
}

void cleanUIWidgets() {
    if (!textTure) SDL_DestroyTexture(textTure);
    for (int i = 0; i < NUMOFITEMS; i++) {
        SDL_DestroyTexture(inventoryEntries[i].nameTexture);
        SDL_DestroyTexture(inventoryEntries[i].descriptionTexture);
    }
    SDL_DestroyTexture(unknownTexture);
    TTF_CloseFont(textFont);
}
