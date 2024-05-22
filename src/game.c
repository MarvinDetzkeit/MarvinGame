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

//Player variables
Player *player = NULL;
SDL_Rect playerObj;
SDL_Texture *playerSprite;

//NPC
NPC *test;

//camera variables
Camera *camera;

//global variables to store screen positions
int screenX;
int screenY;

//Level variables
Level *level = NULL;
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

int updateGame(void *ptr) {
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
                        if (isClose(test, player)) {
                            update = updateTalk;
                            render = renderTalk; 
                            updatePTR = (void *) test;
                            player->movUp = 0;
                            player->movDown = 0;
                            player->movLeft = 0;
                            player->movRight = 0;
                        }
                        break;
                }
        }
    }
    movePlayer(player, level);
    calculateCameraPosition(camera, player);

    return 1;
}


void positionOnScreen(int x, int y) {
    screenX = x - camera->x + ((SCREEN_WIDTH - TILESIZE) / 2);
    screenY = y - camera->y + ((SCREEN_HEIGHT - TILESIZE) / 2);
}

void renderGame(void) {
    //render level
    for (int i = (camera->x / TILESIZE) - TILENUMX - 2; i < (camera->x / TILESIZE) + TILENUMX + 2; i++) {
        for (int j = (camera->y / TILESIZE) - TILENUMY - 2; j < (camera->y / TILESIZE) + TILENUMY + 2; j++) {
            positionOnScreen(i * TILESIZE, j * TILESIZE);
            int tile = getTile(level, i, j);
            if (tile != 0) {
                levelObj.x = screenX;
                levelObj.y = screenY;
                renderTile(renderer, tiles->textures[abs(tile)], &levelObj);
           }
        }
    }
    
    //render NPC
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    positionOnScreen(test->x, test->y);
    playerObj.x = screenX;
    playerObj.y = screenY;
    SDL_RenderFillRect(renderer, &playerObj);

    //render player
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    positionOnScreen(player->x, player->y);
    playerObj.x = screenX;
    playerObj.y = screenY;
    SDL_RenderFillRect(renderer, &playerObj);

}

void cleanUp(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    free(player);
    unloadLevel(level);
    free(level);
    cleanTiles(tiles);
    cleanUIWidgets();
    cleanNPC(test);
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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Failed to create renderer.\n");
        return 0;
    }

    //initialize level
    level = malloc(sizeof(Level));
    strcpy(level->name, "meadow.lvl");
    loadLevel(level);
    levelObj.w = TILESIZE;
    levelObj.h = TILESIZE;
    tiles = malloc(sizeof(Tiles));
    initTiles(tiles, renderer);

    //initialize player
    player = malloc(sizeof(Player));
    player->x = (level->playerX * TILESIZE);
    player->y = (level->playerY * TILESIZE);
    player->movUp = 0;
    player->movDown = 0;
    player->movLeft = 0;
    player->movRight = 0;
    playerObj.h = TILESIZE;
    playerObj.w = TILESIZE;
    playerObj.x = (SCREEN_WIDTH - TILESIZE) / 2;
    playerObj.y = (SCREEN_HEIGHT - TILESIZE) / 2;

    //initialize camera
    camera = malloc(sizeof(Camera));
    initCamera(camera, player->x, player->y);

    //Init UIWidgets
    initUIWidgets();

    //Init NPC
    test = malloc(sizeof(NPC));
    strcpy(test->name, "testNPC");
    loadNPCText(test);
    test->x = TILESIZE * 20;
    test->y = TILESIZE * 10;

    //Set funtion pointers for game loop
    update = updateGame;
    render = renderGame;

    return 1;
}