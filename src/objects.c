#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "header/constants.h"
#include "header/uiwidgets.h"
#include "header/game.h"
#include "header/items.h"

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

int updateQuest(void *ptr);
int updateQuestB(void* ptr);
int updateQuestC(void * ptr);

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
        npcArr[i].updateInteraction = updateQuest;
        npcArr[i].renderInteraction = renderTalk;
    }
    free(nameBuffer);
    //Angelinas Items
    npcArr[6].hasThis = items[1];
    npcArr[6].needsThis = items[0];
    //Baris Items
    npcArr[2].hasThis = items[5];
    npcArr[2].needsThis = items[4];
    //Hendriks Items
    npcArr[3].hasThis = items[4];
    npcArr[3].needsThis = items[1];
    //Julias Items
    npcArr[5].hasThis = items[6];
    npcArr[5].needsThis = items[7];
    //Kemal Item
    npcArr[7].hasThis = items[2];
    //Toms Items
    npcArr[1].hasThis = items[3];
    npcArr[1].needsThis = items[6];

    //Hund und Kemal andere Quest
    npcArr[4].updateInteraction = updateTalk;
    npcArr[7].updateInteraction = updateQuestC;

    //Schatz Quest
    npcArr[8].hasThis = items[7];
    npcArr[8].updateInteraction = updateQuestB;

    //Dixi-Klo Quest
    npcArr[9].hasThis = items[0];
    npcArr[9].updateInteraction = updateQuestB;


    printf("Loaded %d NPCs in.\n", numNPCs);

}

void initNPCsEditor(SDL_Renderer *r) {
    numNPCs = 0;
    int nameLen = 20;
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
        npcArr[i].updateInteraction = updateQuest;
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

int updateQuest(void *ptr) {
    NPC *npc = (NPC*) ptr;
    static int loaded = 0;
    static char ***questLines;
    static int numsLines[2]; //store nums of lines
    static int current = 0; // current line
    static int beforeOrAfter = 0; //0 for before; 1 for after
    //Load Quest Lines on first run
    if (!loaded) {
        questLines = malloc(sizeof(char**) * 2); //One text before giving item, One afterwards
        char buf[100];
        strcpy(buf, "src/data/npcTexts/");
        strcat(buf, npc->name);
        strcat(buf, "Quest.txt");
        FILE *f = fopen(buf, "r");
        for (int i = 0; i < 2; i++) {
            int c = 0;
            questLines[i] = malloc(sizeof(char*) * 10); //Up to 10 Lines
            do {
                fgets(buf, 100, f);
                if (strlen(buf) <= 2) break;
                numsLines[i]++;
                questLines[i][c] = malloc(sizeof(char) * 100);
                strcpy(questLines[i][c], buf);
                questLines[i][c][strcspn(questLines[i][c], "\n")] = 0;
                c++;
            } while (1);
        }
        loaded = 1;
        npcText = questLines[beforeOrAfter][current];
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 0;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
                    case SDLK_t:
                        current++;
                        if (current == numsLines[beforeOrAfter]) {
                        switch (beforeOrAfter)
                        {
                        case 0:
                            if (player->items[npc->needsThis->id]) { //If player has item the npc needs
                            current = 0;
                            beforeOrAfter = 1;
                            npcText = questLines[beforeOrAfter][current];
                            } else {
                                current = 0;
                                update = updateGame;
                                render = renderGame;
                                goto RESETQUEST;
                            }
                            break;
                        case 1:
                            player->items[npc->hasThis->id] = 1; //get item from NPC
                            itemSprite = npc->hasThis->sprite;
                            update = updateItemScreen;
                            render = renderItemScreen;
                            npc->updateInteraction = updateTalk;
                            goto RESETQUEST;
                            break;
                        default:
                            break;
                        }
                    } else npcText = questLines[beforeOrAfter][current];
                }
        }
    }

    return 1;
    RESETQUEST:
    current = 0;
    beforeOrAfter = 0;
    for (int a = 0; a < 2; a++) {
        for (int b = 0; b < numsLines[a]; b++) {
            free(questLines[a][b]);
        }
        free(questLines[a]);
    }
    free(questLines);
    loaded = 0;
    numsLines[0] = numsLines[1] = 0;
    npcText = "";
    return 1;
}

void changePodestTile() {
    SDL_Texture* temp = tiles->textures[106];
    tiles->textures[106] = tiles->textures[107];
    tiles->textures[107] = temp;
}

//Labyrinth / Klo Quest
int updateQuestB(void* ptr) {
    NPC *npc = (NPC*) ptr;
    static int loaded = 0;
    static char ***questLines;
    static int numsLines[2]; //store nums of lines
    static int current = 0; // current line
    static int beforeOrAfter = 0; //0 for before; 1 for after
    //Load Quest Lines on first run
    if (!loaded) {
        questLines = malloc(sizeof(char**) * 2); //One text before giving item, One afterwards
        char buf[100];
        strcpy(buf, "src/data/npcTexts/");
        strcat(buf, npc->name);
        strcat(buf, "Quest.txt");
        FILE *f = fopen(buf, "r");
        for (int i = 0; i < 2; i++) {
            int c = 0;
            questLines[i] = malloc(sizeof(char*) * 10); //Up to 10 Lines
            do {
                fgets(buf, 100, f);
                if (strlen(buf) <= 2) break;
                numsLines[i]++;
                questLines[i][c] = malloc(sizeof(char) * 100);
                strcpy(questLines[i][c], buf);
                questLines[i][c][strcspn(questLines[i][c], "\n")] = 0;
                c++;
            } while (1);
        }
        loaded = 1;
        npcText = questLines[beforeOrAfter][current];
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 0;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
                    case SDLK_t:
                        current++;
                        if (current == numsLines[beforeOrAfter]) {
                        switch (beforeOrAfter)
                        {
                        case 0:
                            current = 0;
                            beforeOrAfter = 1;
                            npcText = questLines[beforeOrAfter][current];
                            break;
                        case 1:
                            player->items[npc->hasThis->id] = 1; //get Treasure
                            itemSprite = npc->hasThis->sprite;
                            update = updateItemScreen;
                            render = renderItemScreen;
                            npc->updateInteraction = updateGame;
                            npc->renderInteraction = renderGame;
                            if (strcmp(npc->name, "LabyrinthSchatz") == 0) changePodestTile();
                            goto RESETQUEST;
                            break;
                        default:
                            break;
                        }
                    } else npcText = questLines[beforeOrAfter][current];
                }
        }
    }

    return 1;
    RESETQUEST:
    current = 0;
    beforeOrAfter = 0;
    for (int a = 0; a < 2; a++) {
        for (int b = 0; b < numsLines[a]; b++) {
            free(questLines[a][b]);
        }
        free(questLines[a]);
    }
    free(questLines);
    loaded = 0;
    numsLines[0] = numsLines[1] = 0;
    npcText = "";
    return 1;
}

//Kemals Quest
int updateQuestC(void *ptr) {
    NPC *npc = (NPC*) ptr;
    static int loaded = 0;
    static char ***questLines;
    static int numsLines[2]; //store nums of lines
    static int current = 0; // current line
    static int beforeOrAfter = 0; //0 for before; 1 for after
    //Load Quest Lines on first run
    if (!loaded) {
        questLines = malloc(sizeof(char**) * 2); //One text before giving item, One afterwards
        char buf[100];
        strcpy(buf, "src/data/npcTexts/");
        strcat(buf, npc->name);
        strcat(buf, "Quest.txt");
        FILE *f = fopen(buf, "r");
        for (int i = 0; i < 2; i++) {
            int c = 0;
            questLines[i] = malloc(sizeof(char*) * 10); //Up to 10 Lines
            do {
                fgets(buf, 100, f);
                if (strlen(buf) <= 2) break;
                numsLines[i]++;
                questLines[i][c] = malloc(sizeof(char) * 100);
                strcpy(questLines[i][c], buf);
                questLines[i][c][strcspn(questLines[i][c], "\n")] = 0;
                c++;
            } while (1);
        }
        loaded = 1;
        npcText = questLines[beforeOrAfter][current];
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 0;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
                    case SDLK_t:
                        current++;
                        if (current == numsLines[beforeOrAfter]) {
                        switch (beforeOrAfter)
                        {
                        case 0:
                            if (player->items[3] && player->items[5]) { //If player has items Schaufel and Metalldetektor
                            current = 0;
                            beforeOrAfter = 1;
                            npcText = questLines[beforeOrAfter][current];
                            } else {
                                current = 0;
                                update = updateGame;
                                render = renderGame;
                                goto RESETQUEST;
                            }
                            break;
                        case 1:
                            player->items[npc->hasThis->id] = 1; //get item from NPC
                            itemSprite = npc->hasThis->sprite;
                            update = updateItemScreen;
                            render = renderItemScreen;
                            npc->updateInteraction = updateTalk;
                            goto RESETQUEST;
                            break;
                        default:
                            break;
                        }
                    } else npcText = questLines[beforeOrAfter][current];
                }
        }
    }

    return 1;
    RESETQUEST:
    current = 0;
    beforeOrAfter = 0;
    for (int a = 0; a < 2; a++) {
        for (int b = 0; b < numsLines[a]; b++) {
            free(questLines[a][b]);
        }
        free(questLines[a]);
    }
    free(questLines);
    loaded = 0;
    numsLines[0] = numsLines[1] = 0;
    npcText = "";
    return 1;
}

void cleanNPCs() {
    for (int i = 1; i <= numNPCs; i++) {
        cleanNPC(npcArr + i);
        SDL_DestroyTexture(npcArr[i].sprite);
    }
    printf("Cleaned NPCs!\n");
}