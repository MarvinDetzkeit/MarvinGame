#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL2/SDL.h>

// Structure definition for NPC
typedef struct {
    SDL_Texture *sprite;
    int x;
    int y;
    char name[20];
    char ***lines;
    int numLines;
    int countOuter;
    int countInner;
} NPC;

// Global variables
extern char *npcText;
extern SDL_Rect objRect;
extern NPC *npcArr;
extern int numNPCs;

// Function prototypes
int updateTalk(void *ptr);
void renderTalk();
void loadNPCText(NPC *npc);
void initNPCs();
void cleanNPC(NPC *n);
void cleanNPCs();
int isClose(NPC *n, Player *p);

#endif // OBJECTS_H
