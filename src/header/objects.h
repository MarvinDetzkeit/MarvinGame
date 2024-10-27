#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "constants.h"
#include "uiwidgets.h"
#include "game.h"
#include "items.h"

typedef struct {
    char name[20];
    SDL_Texture *sprite;
    char ***lines;
    int numLines;
    int countOuter;
    int countInner;
    int (* updateInteraction)();
    void (* renderInteraction)();
    Item *hasThis;
    Item *needsThis;

} NPC;

char *npcText;
NPC *npcArr;
int numNPCs;

int updateTalk(void *ptr);
void renderTalk();
void loadNPCText(NPC *npc);
void initNPCs(SDL_Renderer *r);
void initNPCsEditor(SDL_Renderer *r);
void cleanNPC(NPC *n);
int updateQuest(void *ptr);
int updateQuestB(void* ptr);
int updateQuestC(void *ptr);
void cleanNPCs();

#endif // OBJECTS_H

