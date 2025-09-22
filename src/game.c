#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "header/constants.h"
#include "header/player.h"
#include "header/camera.h"
#include "header/level.h"
#include "header/tiles.h"
#include "header/uiwidgets.h"
#include "header/objects.h"
#include "header/items.h"
#include "header/animation.h"
#include "header/sound.h"

//Player variables
Player *player = NULL;
SDL_Rect playerObj;
SDL_Texture *playerSprite;

Item **items;

//camera variables
Camera *camera;

//global variables to store screen positions
int screenX;
int screenY;

//Level variables
Level *level = NULL;
Level *overworld = NULL;
Level *sublevel = NULL;
SDL_Rect levelObj;
Tiles *tiles = NULL;

//Render variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int running = 0;
Uint32 time_last_frame = 0;

//Game loop function pointers
int (*update)();
void (*render)();

void *updatePTR = NULL;

//Delay to cap game at 60FPS
void delay() {
    Uint32 wait_time = (1000 / FPS) - (SDL_GetTicks() - time_last_frame);
    if ((int)wait_time > 0) {
        SDL_Delay(wait_time);
    }
    time_last_frame = SDL_GetTicks();
}

//Render blend effect for level transition
void levelBlendEffect() {
    static int fade0 = 0;
    static int fade1 = 1;
    static SDL_Rect rec = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    for (int i = 0; i <= 25; i++) {
        delay();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 5 + (fade0 * 250) + (i * 10 * fade1));
        SDL_RenderClear(renderer);
        renderGame();
        SDL_RenderFillRect(renderer, &rec);
        SDL_RenderPresent(renderer);
    }
    fade0 = 1 - fade0;
    fade1 = -fade1;
}

void handleTeleportation() {
    //getTile(level, (player->x + (TILESIZE / 2)) / TILESIZE, (player->y + TILESIZE - 1) / TILESIZE)
    int teleporter = getPlayerTile(player, level);
    if (!teleporter) return;
    playSwitchLevelSound();
    levelBlendEffect();
    switch (teleporter)
    {
    //Return back to overworld
    case 1:
        unloadLevel(sublevel);
        level = overworld;
        player->x = (level->playerX * TILESIZE);
        player->y = (level->playerY * TILESIZE) + 1;
        break;
    //goto Level wohnung.lvl
    case 2:
        level->playerX = (player->x + (TILESIZE / 2)) / TILESIZE;
        level->playerY = (player->y + TILESIZE - 1) / TILESIZE;
        strcpy(sublevel->name, "wohnung.lvl");
        loadLevel(sublevel);
        level = sublevel;
        player->x = (level->playerX * TILESIZE);
        player->y = (level->playerY * TILESIZE);
        break;
    //goto Level omaundopa.lvl
    case 3:
        level->playerX = (player->x + (TILESIZE / 2)) / TILESIZE;
        level->playerY = (player->y + TILESIZE - 1) / TILESIZE;
        strcpy(sublevel->name, "omaundopa.lvl");
        loadLevel(sublevel);
        level = sublevel;
        player->x = (level->playerX * TILESIZE);
        player->y = (level->playerY * TILESIZE);
        break;
    //goto Level dixiklo.lvl
    case 4:
        level->playerX = (player->x + (TILESIZE / 2)) / TILESIZE;
        level->playerY = (player->y + TILESIZE - 1) / TILESIZE;
        strcpy(sublevel->name, "dixiklo.lvl");
        loadLevel(sublevel);
        level = sublevel;
        player->x = (level->playerX * TILESIZE);
        player->y = (level->playerY * TILESIZE);
        break;
    //goto Level gisbertwiese.lvl
    case 5:
        level->playerX = (player->x + (TILESIZE / 2)) / TILESIZE;
        level->playerY = (player->y + TILESIZE - 1) / TILESIZE;
        strcpy(sublevel->name, "gisbertwiese.lvl");
        loadLevel(sublevel);
        level = sublevel;
        player->x = (level->playerX * TILESIZE);
        player->y = (level->playerY * TILESIZE);
        break;
    }

    calculateCameraPositionEditor(camera, player);
    levelBlendEffect();
}

int updateGame(void *ptr) {
    int npc = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
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
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
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
                    case SDLK_t:
                        for (int i = 0; i < 3; i++) {
                            for (int j = 0; j < 3; j++) {
                                if (!npc) npc = (getTile(level, (player->x / TILESIZE) + i - 1, (player->y / TILESIZE) + j - 1) & 0x0ff00000) >> 20;
                            }
                        }
                        if (npc) {
                            name = npcArr[npc].name;
                            update = npcArr[npc].updateInteraction;
                            render = npcArr[npc].renderInteraction;
                            updatePTR = npcArr + npc;
                            player->movDown = 0;
                            player->movUp = 0;
                            player->movLeft = 0;
                            player->movRight = 0;
                        }
                        break;
                }
        }
    }
    movePlayer(player, level);
    handleTeleportation();
    calculateCameraPositionEditor(camera, player);

    return 1;
}


void positionOnScreen(int x, int y) {
    screenX = x - camera->x + ((SCREEN_WIDTH - TILESIZE) / 2);
    screenY = y - camera->y + ((SCREEN_HEIGHT - TILESIZE) / 2);
}

void renderGame(void) {
    //render level
    int npc;
    int tile;
    for (int i = (camera->x / TILESIZE) - TILENUMX - 2; i < (camera->x / TILESIZE) + TILENUMX + 2; i++) {
        for (int j = (camera->y / TILESIZE) - TILENUMY - 2; j < (camera->y / TILESIZE) + TILENUMY + 2; j++) {
            positionOnScreen(i * TILESIZE, j * TILESIZE);
            tile = getTile(level, i, j);
            if (tile != 0) {
                levelObj.x = screenX;
                levelObj.y = screenY;
                renderTile(renderer, tiles->textures[tile & 0x0000ffff], &levelObj);
                npc = (tile & 0x0ff00000) >> 20;
                if (npc) renderTile(renderer, npcArr[npc].sprite, &levelObj);
           }
        }
    }

    //render player
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    positionOnScreen(player->x, player->y);
    playerObj.x = screenX;
    playerObj.y = screenY;
    //SDL_RenderFillRect(renderer, &playerObj);
    renderTile(renderer, player->playerSprite, &playerObj);

}

void cleanUp(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    cleanPlayer(player);
    free(player);
    unloadLevel(level);
    free(level);
    cleanFrames();
    cleanTiles(tiles);
    cleanUIWidgets();
    cleanSound();
    printf("Cleaned up!\n");
}

int initialize(void) {
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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Failed to create renderer.\n");
        return 0;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //initialize level
    overworld = malloc(sizeof(Level));
    sublevel = malloc(sizeof(Level));
    strcpy(overworld->name, "Luebeck.lvl");
    loadLevel(overworld);
    overworld->playerX = 28;
    overworld->playerY = 24;
    //Player spawns in sublevel wohnung.lvl
    strcpy(sublevel->name, "wohnung.lvl");
    loadLevel(sublevel);
    sublevel->playerX = 4;
    sublevel->playerY = 5;
    levelObj.w = TILESIZE;
    levelObj.h = TILESIZE;
    level = sublevel;
    tiles = malloc(sizeof(Tiles));
    initTiles(tiles, renderer);

    //initialize player
    player = malloc(sizeof(Player));
    initPlayer(renderer, player, level, &playerObj);


    //initialize items
    items = malloc(sizeof(Item*) * NUMOFITEMS);
    initItems(renderer, items);

    //initialize camera
    camera = malloc(sizeof(Camera));
    initCamera(camera, player->x, player->y);

    //Init UIWidgets
    initUIWidgets();

    //Init NPCs
    initNPCs(renderer);

    //Init Animation
    initFrames(renderer);

    //Init sound
    initSound();
    playGameTheme();

    //Set funtion pointers for game loop
    //update = updateGame;
    //render = renderGame;
    //Start game with the turorial
    update = updateTalk;
    render = renderTalkNoName;
    updatePTR = npcArr + 12;

    return 1;
}