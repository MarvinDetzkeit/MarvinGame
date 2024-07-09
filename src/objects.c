#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "header/constants.h"
#include "header/uiwidgets.h"
#include "header/game.h"

typedef struct {
    char name[20];
    SDL_Texture *sprite;
    char ***lines;
    int numLines;
    int countOuter;
    int countInner;
    int (* updateInteraction)();
    void (* renderInteraction)();

} NPC;

char *npcText;
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
                            npcText = "";
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

//Ändere, sodass alles bei 1 anfängt
void initNPCs(SDL_Renderer *r) {
    numNPCs = 0;
    int nameLen = 20;
    char buffer[nameLen];
    char path[20 + nameLen];
    strcpy(path, "src/data/NPCNames.txt");
    char **nameBuffer = malloc(500 * sizeof(char*));

    FILE *f = fopen(path, "r");
    size_t cmp = 2;
    while (1) {
        nameBuffer[numNPCs] = malloc(sizeof(char) * nameLen);
        fgets(nameBuffer[numNPCs], nameLen, f);
        nameBuffer[numNPCs][strcspn(nameBuffer[numNPCs], "\n")] = 0;
        if (strlen(nameBuffer[numNPCs]) <= cmp) {
            free(nameBuffer[numNPCs]);
            break;
        }
        numNPCs++;
    }
    fclose(f);
    f = NULL;

    SDL_Surface *s = NULL;
    npcArr = malloc((numNPCs + 1) * sizeof(NPC));
    for (int i = 1; i <= numNPCs; i++) {
        strcpy(npcArr[i].name, nameBuffer[i-1]);
        npcArr[i].countInner = 0;
        npcArr[i].countOuter = 0;
        stpcpy(path, "src/data/sprites/");
        strcat(path, npcArr[i].name);
        strcat(path, ".png");
        s = IMG_Load(path);
        npcArr[i].sprite = SDL_CreateTextureFromSurface(r, s);
        SDL_FreeSurface(s);
        loadNPCText(npcArr + i);
        free(nameBuffer[i-1]);
        npcArr[i].updateInteraction = updateTalk;
        npcArr[i].renderInteraction = renderTalk;
    }
    free(nameBuffer);
    printf("Loaded %d NPCs in.\n", numNPCs);

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
}

void cleanNPCs() {
    for (int i = 1; i <= numNPCs; i++) {
        cleanNPC(npcArr + i);
        SDL_DestroyTexture(npcArr[i].sprite);
    }
    printf("Cleaned NPCs!\n");
}