#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "header/constants.h"

SDL_Rect backgroundRect;
SDL_Rect textRect;
TTF_Font *textFont;
SDL_Surface *textSurface;
SDL_Texture *textTure = NULL;
SDL_Color textColor = {255, 255, 255};

typedef struct
{
    char *text;
} Textbox;

void initUIWidgets() {
    textFont = TTF_OpenFont("src/data/ArialBlack.ttf", 24);
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