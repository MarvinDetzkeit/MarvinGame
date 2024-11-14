#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "header/constants.h"
#include "header/uiwidgets.h"
#include "header/game.h"
#include "header/items.h"
#include "header/sound.h"

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

#define WAITFRAMEVALUE (FPS / 3)
uint8_t waitFrames = WAITFRAMEVALUE;

// Functions waitDialogue and ignoreThisPress implement a timer for the game to wait WAITFRAMEVALUE many Frames until the player
// can go to the next line when talking to an NPC
void waitDialogue() {
    if (waitFrames > 0) {
        waitFrames--;
    }
}

uint8_t ignoreThisPress() {
    if (waitFrames <= 0) {
        waitFrames = WAITFRAMEVALUE;
        return 0;
    }
    else return 1;
}

int updateQuest(void *ptr);
int updateQuestB(void* ptr);
int updateQuestC(void * ptr);

int updateTalk(void *ptr) {
    static int playSound = 1;
    if (playSound) { //play the sound only once when the player starts talking to the npc
        playClickSound();
        playSound = 0;
    }
    waitDialogue();
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
                        if (ignoreThisPress()) break;
                        npc->countInner++;
                        if (npc->lines[npc->countOuter][npc->countInner] == NULL) {
                            npc->countOuter = (npc->countOuter + 1) % npc->numLines;
                            npc->countInner = 0;
                            update = updateGame;
                            render = renderGame;
                            npcText = "";
                            playSound = 1;
                        }
                        else playClickSound();
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
    createNameTexture(renderer);
    renderNameBox(renderer);
}

// For the Labyrinth and Toilet Quest
void renderTalkNoName() {
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
        npcArr[i].updateInteraction = updateTalk;
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


    //Kemal
    npcArr[7].updateInteraction = updateQuestC;

    //Bushaltestelle
    npcArr[10].renderInteraction = renderTalkNoName;

    //Schatz Quest
    npcArr[8].hasThis = items[7];
    npcArr[8].updateInteraction = updateQuestB;
    npcArr[8].renderInteraction = renderTalkNoName;

    //Dixi-Klo Quest
    npcArr[9].hasThis = items[0];
    npcArr[9].updateInteraction = updateQuestB;
    npcArr[9].renderInteraction = renderTalkNoName;

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
    waitDialogue();
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
        playClickSound();
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
                        if (ignoreThisPress()) break;
                        current++;
                        if (current == numsLines[beforeOrAfter]) {
                        switch (beforeOrAfter)
                        {
                        case 0:
                            if (player->items[npc->needsThis->id]) { //If player has item the npc needs
                            current = 0;
                            beforeOrAfter = 1;
                            npcText = questLines[beforeOrAfter][current];
                            playClickSound();
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
                            playGetItemSound();
                            goto RESETQUEST;
                            break;
                        default:
                            break;
                        }
                    } else {
                        npcText = questLines[beforeOrAfter][current];
                        playClickSound();
                    }
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

//Swicth Tiles by number
void switchTiles(int numA, int numB) {
    SDL_Texture* temp = tiles->textures[numA];
    tiles->textures[numA] = tiles->textures[numB];
    tiles->textures[numB] = temp;
}

//Labyrinth / Klo Quest
int updateQuestB(void* ptr) {
    NPC *npc = (NPC*) ptr;
    static int loaded = 0;
    static char ***questLines;
    static int numsLines[2]; //store nums of lines
    static int current = 0; // current line
    static int beforeOrAfter = 0; //0 for before; 1 for after
    waitDialogue();
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
        playClickSound();
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
                        if (ignoreThisPress()) break;
                        current++;
                        if (current == numsLines[beforeOrAfter]) {
                        switch (beforeOrAfter)
                        {
                        case 0:
                            current = 0;
                            beforeOrAfter = 1;
                            npcText = questLines[beforeOrAfter][current];
                            playClickSound();
                            break;
                        case 1:
                            player->items[npc->hasThis->id] = 1; //get Treasure
                            itemSprite = npc->hasThis->sprite;
                            update = updateItemScreen;
                            render = renderItemScreen;
                            npc->updateInteraction = updateGame;
                            npc->renderInteraction = renderGame;
                            if (strcmp(npc->name, "LabyrinthSchatz") == 0) switchTiles(106, 107);
                            playGetItemSound();
                            goto RESETQUEST;
                            break;
                        default:
                            break;
                        }
                    } else {
                        npcText = questLines[beforeOrAfter][current];
                        playClickSound();
                    }
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
    static uint8_t firstTimeInteracting = 1;
    waitDialogue();
    //Activate quests for NPCs (only when executing this function for the first time)
    if (firstTimeInteracting) {
        firstTimeInteracting = 0;
        npcArr[1].updateInteraction = updateQuest;
        npcArr[2].updateInteraction = updateQuest;
        npcArr[3].updateInteraction = updateQuest;
        npcArr[5].updateInteraction = updateQuest;
        npcArr[6].updateInteraction = updateQuest;
    }
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
        playClickSound();
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
                        if (ignoreThisPress()) break;
                        current++;
                        if (current == numsLines[beforeOrAfter]) {
                        switch (beforeOrAfter)
                        {
                        case 0:
                            if (player->items[3] && player->items[5]) { //If player has items Schaufel and Metalldetektor
                            current = 0;
                            beforeOrAfter = 1;
                            npcText = questLines[beforeOrAfter][current];
                            playClickSound();
                            } else {
                                current = 0;
                                update = updateGame;
                                render = renderGame;
                                goto RESETQUEST;
                            }
                            break;
                        case 1:
                            player->items[npc->hasThis->id] = 1; //get item from NPC
                            levelBlendEffect();
                            switchTiles(138, 140);
                            switchTiles(139, 141);
                            switchTiles(143, 144);
                            levelBlendEffect();
                            itemSprite = npc->hasThis->sprite;
                            update = updateItemScreen;
                            render = renderItemScreen;
                            npc->updateInteraction = updateTalk;
                            playGetItemSound();
                            goto RESETQUEST;
                            break;
                        default:
                            break;
                        }
                    } else {
                        npcText = questLines[beforeOrAfter][current];
                        playClickSound();
                    }
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