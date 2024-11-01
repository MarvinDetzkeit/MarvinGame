#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "header/constants.h"

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

typedef struct
{
    char *text;
} Textbox;

void initUIWidgets() {
    textFont = TTF_OpenFont("src/data/ArialBlack.ttf", 24);
    nameFont = TTF_OpenFont("src/data/ArialBlack.ttf", 18);
    backgroundRect.h = 50;
    backgroundRect.w = 8 * SCREEN_WIDTH / 10;
    backgroundRect.x = (SCREEN_WIDTH - backgroundRect.w) / 2;
    backgroundRect.y = SCREEN_HEIGHT - backgroundRect.h - 10;
}

void createTextTure(SDL_Renderer *r, char *text) {
    if (!textTure) SDL_DestroyTexture(textTure);
    textSurface = TTF_RenderText_Solid(textFont, text, textColor);
    textTure = SDL_CreateTextureFromSurface(r, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_QueryTexture(textTure, NULL, NULL, &(textRect.w), &(textRect.h));
    textRect.x = backgroundRect.x + ((backgroundRect.w - textRect.w) / 2);
    textRect.y = backgroundRect.y + ((backgroundRect.h - textRect.h) / 2);
}

void createNameTexture(SDL_Renderer *r) {
    if (!nameTexture) SDL_DestroyTexture(nameTexture);
    nameSurface = TTF_RenderText_Solid(nameFont, name, textColor);
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
    SDL_SetRenderDrawColor(r, 0, 0, 0, 100);
    SDL_RenderFillRect(r, &backgroundRectName);
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDrawRect(r, &backgroundRectName);
    SDL_RenderCopy(r, nameTexture, NULL, &nameRect);
}

void renderTextBox(SDL_Renderer *r) {
    SDL_SetRenderDrawColor(r, 0, 0, 0, 100);
    SDL_RenderFillRect(r, &backgroundRect);
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDrawRect(r, &backgroundRect);
    SDL_RenderCopy(r, textTure, NULL, &textRect);
}

void renderItemBox(SDL_Renderer *r, SDL_Texture *itemSprite) {
    SDL_Rect box = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SDL_Rect itemRec = {(SCREEN_WIDTH - ITEMSIZE) / 2, (SCREEN_HEIGHT - ITEMSIZE) / 2, ITEMSIZE, ITEMSIZE};
    SDL_SetRenderDrawColor(r, 0, 0, 0, 100);
    SDL_RenderFillRect(r, &box);
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDrawRect(r, &box);
    SDL_RenderCopy(r, itemSprite, NULL, &itemRec);
}

void cleanUIWidgets() {
    if (!textTure) SDL_DestroyTexture(textTure);
    TTF_CloseFont(textFont);
}