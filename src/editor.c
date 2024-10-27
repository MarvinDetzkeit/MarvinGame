#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <unistd.h>
#include "header/constants.h"
#include "header/player.h"
#include "header/camera.h"
#include "header/level.h"
#include "header/tiles.h"
#include "header/objects.h"
#include "header/items.h"

//Inits to make it compile
Item** items;
int (*update)();
void (*render)();
void renderGame() {
    return;
}
int updateGame(void *ptr) {
    return 0;
}


//values for editor
int tilesize = 64;
char pathE[50];
int inventory[9];
int npcNumber = 1;
int current = 0;
int draw = 0;
int mouseX = 0;
int mouseY = 0;
int shiftCollision = 0;
//0 for setting Tiles, 1 for deleting Tiles löschen, 2 for setting collision, 3 for setting Spawnpoint, 4 for placing NPCs
int mode = 0;
int playerSpawnX = 0;
int playerSpawnY = 0;

//UI variables
SDL_Rect box;
SDL_Rect tileItem;
int grid = 1;
SDL_Surface *textSurface;
TTF_Font *font;
SDL_Texture **inventoryNumbers;
SDL_Texture **modeName;
SDL_Rect numberRect;
SDL_Rect modeRect;
char **modes;
SDL_Rect npcBox;


//Player variables
Player *player = NULL;

//camera variables
Camera *camera;

//global variables to store screen positions
int screenX;
int screenY;

//Level variables
Level *level = NULL;
SDL_Rect levelObj;
Tiles *tiles = NULL;
int sizeOfLevel = 1000;

//Render variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int running = 1;
Uint32 time_last_frame = 0;

int initializeEditor(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Failed initializing SDL.\n");
        return 0;
    }
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    //init window
    window = SDL_CreateWindow(NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_BORDERLESS);
    if (!window) {
        fprintf(stderr, "Failed to create window.\n");
        return 0;
    }
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    printf("Window Created Size: %d x %d\n", width, height);

    //init renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Failed to create renderer.\n");
        return 0;
    }

    //initialize level
    level = malloc(sizeof(Level));
    char levelName[20];
    printf("Input a level name. Unused name to create a new level, used name to edit that level: ");
    fgets(levelName, sizeof(levelName), stdin);
    levelName[strcspn(levelName, "\n")] = 0;
    strcpy(level->name, levelName);
    strcpy(pathE, "src/level/");
    strcat(pathE, levelName);
    int *editLevel = calloc(sizeOfLevel*sizeOfLevel, sizeof(int));
    if (access(pathE, F_OK) == 0) {
        printf("Loading level \'%s\' into the editor.\n", levelName);
        loadLevel(level);
        int startX = ((sizeOfLevel - level->x) / 2);
        int startY = ((sizeOfLevel - level->y) / 2);
        playerSpawnX = startX + level->playerX;
        playerSpawnY = startY + level->playerY;
        printf("0: Playerspawn x=%d, y=%d\n", playerSpawnX, playerSpawnY);
        for (int i = startX; i < startX + level->x; i++) {
            for (int j = startY; j < startY + level->y; j++) {
                editLevel[(j * sizeOfLevel) + i] = getTile(level, i - startX, j - startY);
                
            }
        }
        free(level->tiles);
    }
    else {
        playerSpawnX = ((sizeOfLevel - level->x) / 2);
        playerSpawnY = ((sizeOfLevel - level->y) / 2);
        printf("Creating new level \'%s\'.\n", levelName);
    }
    level->tiles = editLevel;
    printf("1: Playerspawn x=%d, y=%d\n", playerSpawnX, playerSpawnY);
    level->x = sizeOfLevel;
    level->y = sizeOfLevel;
    editLevel = NULL;

    tiles = malloc(sizeof(Tiles));
    initTiles(tiles, renderer);

    //initialize player
    player = malloc(sizeof(Player));
    player->x = (tilesize * sizeOfLevel) / 2;
    player->y = (tilesize * sizeOfLevel) / 2;
    player->movUp = 0;
    player->movDown = 0;
    player->movLeft = 0;
    player->movRight = 0;

    //initialize camera
    camera = malloc(sizeof(Camera));
    initCamera(camera, player->x, player->y);

    //Init NPCs
    initNPCsEditor(renderer);

    //Init Items
    items = malloc(sizeof(Item*) * NUMOFITEMS); //needed for editor to compile

    //initialize inventory
    for (int i = 0; i < 9; i++) {
        inventory[i] = 2;
    }

    //initialize UI
    box.h = 100;
    box.w = 630;
    box.x = (SCREEN_WIDTH - box.w) / 2;
    box.y = SCREEN_HEIGHT - box.h;
    tileItem.h = 64;
    tileItem.w = 64;
    tileItem.y = box.y + 3;
    npcBox.w = 70;
    npcBox.h = box.h;
    npcBox.x = box.w + box.x + 50;
    npcBox.y = box.y;
    font = TTF_OpenFont("src/data/ArialBlack.ttf", 24);
    if (!font) printf("Font is NULL\n");
    inventoryNumbers = malloc(10 * sizeof(SDL_Texture*));
    SDL_Color textColor = {255, 255, 255, 255};
    char numberChar[2];
    numberChar[1] = 0;
    int texture_width, texture_height;
    for (int i = 0; i < 9; i++) {
        numberChar[0] = '0' + i+1;
        textSurface = TTF_RenderText_Blended(font, numberChar, textColor);
        inventoryNumbers[i] = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(inventoryNumbers[i], NULL, NULL, &texture_width, &texture_height);
        SDL_SetTextureBlendMode(inventoryNumbers[i], SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(textSurface);
    }
    numberRect.y = box.y + 70;
    numberRect.h = texture_height;
    numberRect.w = texture_width;

    modes = malloc(5 * sizeof(char*));
    for (int i = 0; i < 5; i++) modes[i] = malloc(sizeof(char) * 12);
    strcpy(modes[0], "Build");
    strcpy(modes[1], "Erase");
    strcpy(modes[2], "Collision");
    strcpy(modes[3], "Set Spawn");
    strcpy(modes[4], "Place NPC");
    modeName = malloc(5 * sizeof(SDL_Texture*));
    for (int i = 0; i < 5; i++) {
        textSurface = TTF_RenderText_Blended(font, modes[i], textColor);
        modeName[i] = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(modeName[i], NULL, NULL, &texture_width, &texture_height);
        SDL_SetTextureBlendMode(modeName[i], SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(textSurface);
    }
    modeRect.h = 70;
    modeRect.w = 630;
    modeRect.x = (SCREEN_WIDTH - modeRect.w) / 2;
    modeRect.y = 0;
    TTF_CloseFont(font);
    return 1;
}

//Returns the array position in the level->tiles array for screen coordinates (x, y)
int tileAtScreenPosition(int x, int y) {
    x += camera->x - (SCREEN_WIDTH / 2);
    y += camera->y - (SCREEN_HEIGHT / 2);
    x /= tilesize;
    y /= tilesize;
    return x + (sizeOfLevel * y);
}

void saveLevel(Level *l);

void updateEditor(void) {
    //Delay to cap game at 60FPS
    Uint32 wait_time = (1000 / FPS) - (SDL_GetTicks() - time_last_frame);
    if ((int)wait_time > 0) {
        SDL_Delay(wait_time);
    }
    time_last_frame = SDL_GetTicks();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                draw = 1;
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                break;
            case SDL_MOUSEBUTTONUP:
                draw = 0;
                if (mode == 2) level->tiles[tileAtScreenPosition(mouseX, mouseY)] = level->tiles[tileAtScreenPosition(mouseX, mouseY)] ^ 0xf0000000;
                if (mode == 4) {
                    level->tiles[tileAtScreenPosition(mouseX, mouseY)] = level->tiles[tileAtScreenPosition(mouseX, mouseY)] & 0xf00fffff;
                    level->tiles[tileAtScreenPosition(mouseX, mouseY)] = level->tiles[tileAtScreenPosition(mouseX, mouseY)] ^ (npcNumber << 20);
                    printf("Placing NPC with number %d, hex: 0x%08x, hex shifted 0x%08x\n", npcNumber, npcNumber, (npcNumber << 20));
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = 0;
                        break;
                    case SDLK_w:
                        player->movUp = 1 - player->movDown;
                        break;
                    case SDLK_s:
                        player->movDown = 1 - player->movUp;
                        break;
                    case SDLK_a:
                        player->movLeft = 1 - player->movRight;
                        break;
                    case SDLK_d:
                        player->movRight = 1 - player->movLeft;
                        break;
                    case SDLK_UP:
                        if (mode == 0) {
                        inventory[current]++;
                        if (inventory[current] >= numTiles) inventory[current] = 2;
                        }
                        if (mode == 4) {
                            npcNumber++;
                            if (npcNumber > numNPCs) npcNumber = 1;
                        }
                        break;
                    case SDLK_DOWN:
                        if (mode == 0) {
                            inventory[current]--;
                            if (inventory[current] < 2) inventory[current] = numTiles - 1;
                        }
                        if (mode == 4) {
                            npcNumber--;
                            if (npcNumber < 1) npcNumber = numNPCs;
                        }
                        break;
                    case SDLK_LSHIFT:
                        shiftCollision = 1;
                        break;
                    case SDLK_1:
                        current = 0;
                        break;
                    case SDLK_2:
                        current = 1;
                        break;
                    case SDLK_3:
                        current = 2;
                        break;
                    case SDLK_4:
                        current = 3;
                        break;
                    case SDLK_5:
                        current = 4;
                        break;
                    case SDLK_6:
                        current = 5;
                        break;
                    case SDLK_7:
                        current = 6;
                        break;
                    case SDLK_8:
                        current = 7;
                        break;
                    case SDLK_9:
                        current = 8;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        player->movUp = 0;
                        break;
                    case SDLK_s:
                        player->movDown = 0;
                        break;
                    case SDLK_a:
                        player->movLeft = 0;
                        break;
                    case SDLK_d:
                        player->movRight = 0;
                        break;
                    case SDLK_0:
                        saveLevel(level);
                        break;
                    case SDLK_g:
                        grid = 1 - grid;
                        break;
                    case SDLK_m:
                        mode = (mode + 1) % 5;
                        printf("Switched to mode %d.\n", mode);
                        break;
                    case SDLK_LSHIFT:
                        shiftCollision = 0;
                        break;
                    case SDLK_i:
                        player->x /= tilesize;
                        player->y /= tilesize;
                        tilesize += 8;
                        player->x *= tilesize;
                        player->y *= tilesize;
                        break;
                    case SDLK_o:
                        player->x /= tilesize;
                        player->y /= tilesize;
                        tilesize -= 8;
                        player->x *= tilesize;
                        player->y *= tilesize;
                        break;
                    case SDLK_r:
                        player->x = sizeOfLevel * tilesize / 2;
                        player->y = sizeOfLevel * tilesize / 2;
                        break;
                }
            
        }
        

    }
    if (draw) {
        int store = tileAtScreenPosition(mouseX, mouseY);
        if (-1 < store && store < sizeOfLevel*sizeOfLevel) {
        switch (mode) {
        case 0:
            level->tiles[store] = inventory[current] ^ (0xf0000000 * shiftCollision);
            break;
        case 1:
            level->tiles[store] = 0;
            break;
        case 3:
            printf("3: Playerspawn x=%d, y=%d\n", playerSpawnX, playerSpawnY);
            playerSpawnX = store % sizeOfLevel;
            playerSpawnY = store / sizeOfLevel;
            printf("4: Playerspawn x=%d, y=%d\n", playerSpawnX, playerSpawnY);
            break;
        }
        }
    }


    levelObj.w = tilesize;
    levelObj.h = tilesize;
    movePlayerEditor(player);
    setCameraPosition(camera, player->x, player->y);

}

void positionOnScreen(int x, int y) {
    screenX = x - camera->x + (SCREEN_WIDTH / 2);
    screenY = y - camera->y + (SCREEN_HEIGHT / 2);
}

void renderEditor(void) {
    //render window
    SDL_SetRenderDrawColor(renderer, 0, 100, 150, 255);
    SDL_RenderClear(renderer);

    //render level
    int tilenumx = (SCREEN_WIDTH / (2 * tilesize)) + 3;
    int tilenumy = (SCREEN_HEIGHT / (2 * tilesize)) + 3;
    int tile;
    int npc;
    for (int i = (camera->x / tilesize) - tilenumx; i < (camera->x / tilesize) + tilenumx; i++) {
        for (int j = (camera->y / tilesize) - tilenumy ; j < (camera->y / tilesize) + tilenumy; j++) {
            positionOnScreen(i * tilesize, j * tilesize);
            tile = getTile(level, i, j);
            levelObj.x = screenX;
            levelObj.y = screenY;
            if (tile != 0) {
                renderTile(renderer, tiles->textures[tile & 0x0000ffff], &levelObj);
                npc = (tile & 0x0ff00000) >> 20;
                if (npc) renderTile(renderer, npcArr[npc].sprite, &levelObj);
            }
            if (grid) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &levelObj);
           }
            if (tileHasCollision(tile)) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &levelObj);
            }
            if (playerSpawnX == i && playerSpawnY == j) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderDrawRect(renderer, &levelObj);
            }

        }
    }

    //render UI
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderFillRect(renderer, &box);
    SDL_RenderFillRect(renderer, &npcBox);
    for (int i = 0; i < 9; i++) {
        tileItem.x = box.x + (70 * i) + 3;
        numberRect.x = tileItem.x - 3;
        SDL_RenderCopy(renderer, inventoryNumbers[i], NULL, &numberRect);
        if (i == current && mode == 0) {
            tileItem.x -= 3;
            tileItem.y -= 3;
            tileItem.h = 70;
            tileItem.w = 70;
            renderTile(renderer, tiles->textures[inventory[i]], &tileItem);
            tileItem.x += 3;
            tileItem.y += 3;
            tileItem.h = 64;
            tileItem.w = 64;
        }
        else {
            renderTile(renderer, tiles->textures[inventory[i]], &tileItem);
        }
    }
    if (mode == 4) {
        tileItem.x = npcBox.x;
        tileItem.y = npcBox.y;
        tileItem.h = 70;
        tileItem.w = 70;
        renderTile(renderer, npcArr[npcNumber].sprite, &tileItem);
        tileItem.h = 64;
        tileItem.w = 64;
    }
    else {
        tileItem.x = npcBox.x + 3;
        tileItem.y = npcBox.y + 3;
        renderTile(renderer, npcArr[npcNumber].sprite, &tileItem);
    }
    SDL_RenderFillRect(renderer, &modeRect);
    SDL_RenderCopy(renderer, modeName[mode], NULL, &modeRect);

    SDL_RenderPresent(renderer);
}

void saveLevel(Level *l) {
    FILE *f = fopen(pathE, "w");
    int left = sizeOfLevel;
    int right = 0;
    int up = sizeOfLevel;
    int down = 0;
    for (int i = 0; i < sizeOfLevel*sizeOfLevel; i++) {
        if (i % sizeOfLevel < left && level->tiles[i] != 0) left = i % sizeOfLevel;
        if (i % sizeOfLevel > right && level->tiles[i] != 0) right = i % sizeOfLevel;
        if (i / sizeOfLevel < up && level->tiles[i] != 0) up = i / sizeOfLevel;
        if (i / sizeOfLevel > down && level->tiles[i] != 0) down = i / sizeOfLevel;
    }
    fprintf(f, "%d\n%d\n%d\n%d\n", right - left+1, down - up+1, playerSpawnX - left, playerSpawnY - up);
    printf("Playerspawn x=%d, y=%d\n", playerSpawnX, playerSpawnY);
    for (int y = up; y <= down; y++) {
        for (int x = left; x <= right; x++) {
            fprintf(f, "%d\n", level->tiles[x + (y * sizeOfLevel)]);
        }
    }
    fclose(f);
    printf("Saved level at %s.\n", pathE);

}

void cleanUpEditor(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    free(player);
    unloadLevel(level);
    free(level);
    cleanNPCs();
    cleanTiles(tiles);
    for (int i = 0; i < 9; i++) {
        SDL_DestroyTexture(inventoryNumbers[i]);
    }
    free(inventoryNumbers);
    for (int i = 0; i < 5; i++) {
        SDL_DestroyTexture(modeName[i]);
        free(modes[i]);
    }
    free(modeName);
    free(modes);
    printf("Cleaned up!\n");
}

int main() {
    initializeEditor();
    while (running)
    {
        updateEditor();
        renderEditor();
    }
    cleanUpEditor();
    return 0;
}