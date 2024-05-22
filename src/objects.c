#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "header/constants.h"
#include "header/uiwidgets.h"
#include "header/game.h"

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

char *npcText;
SDL_Rect objRect;
NPC *npcArr;
int numNPCs;

int updateTalk(void *ptr) {
    NPC *npc = (NPC *) ptr;
    SDL_Event event;
    npcText = npc->lines[npc->countOuter][npc->countInner];
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 0;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
                    case SDLK_t:
                        npc->countInner++;
                        if (npc->lines[npc->countOuter][npc->countInner] == NULL) {
                            npc->countOuter = (npc->countOuter + 1) % npc->numLines;
                            npc->countInner = 0;
                            update = updateGame;
                            render = renderGame;
                        }
                        break;

                }
        }
    }

    return 1;
}

void renderTalk() {
    renderGame();
    createTextTure(renderer, npcText);
    renderTextBox(renderer);
}


void loadNPCText(NPC *npc) {
    char buf[40];
    char line[100];
    strcpy(buf, "src/data/npcTexts/");
    strcat(buf, npc->name);
    strcat(buf, ".txt");
    FILE *txt = fopen(buf, "r");
    npc->lines = malloc(50 * sizeof(char**));
    npc->numLines = 0;
    int lineCount = 0;
    int run = 1;
    npc->lines[npc->numLines] = malloc(50 * sizeof(char*));
    do {
        fgets(line, 100, txt);
        line[strlen(line) - 1] = 0;
        switch (strlen(line)) {
            case 0:
                run = 0;
                break;
            case 1:
                npc->lines[npc->numLines][lineCount] = NULL;
                npc->numLines++;
                lineCount = 0;
                npc->lines[npc->numLines] = malloc(50 * sizeof(char*));
                break;
            default:
                npc->lines[npc->numLines][lineCount] = malloc(100 * sizeof(char));
                strcpy(npc->lines[npc->numLines][lineCount], line);
                lineCount++;
                break;
        }
    } while (run);
    npc->numLines++;
    npc->lines[npc->numLines] = NULL;
    fclose(txt);
    
}

//Assuming the array npcArr is filled
void initNPCs() {
    for (int i = 0; i < numNPCs; i++) {
        loadNPCText(npcArr+i);
    }

}

void cleanNPC(NPC *n) {
    int countInner = 0;
    int countOuter = 0;
    while (1) {
        while (1) {
            if (n->lines[countOuter][countInner] == NULL) {
                countInner = 0;
                countOuter++;
                break;
            }
            free(n->lines[countOuter][countInner]);
            countInner++;
        }
        if (n->lines[countOuter] == NULL) break;
        free(n->lines[countOuter]);
        countOuter++;
    }
    free(n->lines);
    printf("NPC cleaned!\n");
}

void cleanNPCs() {
    for (int i = 0; i < numNPCs; i++) {
        cleanNPC(npcArr + i);
    }
}

//Return 1 if NPC is close enough for player to talk to
int isClose(NPC *n, Player *p) {
    double w = sqrt(pow((double)(p->x - n->x), 2.0) + pow((double)(p->y - n->y), 2.0));
    printf("%f\n", w);
    return (w < ((double) TILESIZE) * 1.5);
}