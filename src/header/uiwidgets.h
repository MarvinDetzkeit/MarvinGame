#ifndef UI_WIDGETS_H
#define UI_WIDGETS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "constants.h"

// Global variables
extern SDL_Rect backgroundRect;
extern SDL_Rect textRect;
extern TTF_Font *textFont;
extern SDL_Surface *textSurface;
extern SDL_Texture *textTure;
extern SDL_Color textColor;
extern char* name;

// Structs
typedef struct {
    char *text;
} Textbox;

// Function declarations
void initUIWidgets();
void createTextTure(SDL_Renderer *r, char *text);
void renderTextBox(SDL_Renderer *r);
void createNameTexture(SDL_Renderer *r);
void renderNameBox(SDL_Renderer *r);
void renderItemBox(SDL_Renderer *r, SDL_Texture *itemSprite);
void cleanUIWidgets();

#endif // UI_WIDGETS_H
