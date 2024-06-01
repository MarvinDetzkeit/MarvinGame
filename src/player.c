#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "header/constants.h"
#include "header/level.h"

typedef struct
{
    int x;
    int y;
    int movUp;
    int movDown;
    int movLeft;
    int movRight;
    SDL_Texture *playerSprite;
    //Down: 0-2; Left: 3-5; Right: 6-8; Up: 9-11
    SDL_Texture **sprites;
} Player;


void printPlayerPosition(Player* p) {
    printf("Player Position: (%d, %d)\n", p->x, p->y);
}

//move player with collision; Tiles are marked as collidable by setting the 4 most significant bits to 1: 0xf...
//for each direction, it checks 2 corners of the player rect
//Collision only works properly for positive coordinates (level coordinates are always positive - levels start at (0, 0))
void movePlayer(Player *p, Level *l) {
    //counts frames to chose right animation sprite
    static int frameCounter = 0;
    static int innerAnimation = 0;
    static int outerAnimation = 0;
    if (p->movLeft) {
        p->x -= PLAYERSPEED - (p->movUp + p->movDown) * (PLAYERSPEED / 4);
        if (tileHasCollision(getTile(l, p->x / TILESIZE, p->y / TILESIZE)) || tileHasCollision(getTile(l, p->x / TILESIZE, (p->y + TILESIZE-1) / TILESIZE))) {
            p->x += TILESIZE - (p->x % TILESIZE);
        }
        outerAnimation = 3;
    }
    if (p->movRight) {
        p->x += PLAYERSPEED - (p->movUp + p->movDown) * (PLAYERSPEED / 4);
        if (tileHasCollision(getTile(l, (p->x + TILESIZE-1) / TILESIZE, p->y / TILESIZE)) || tileHasCollision(getTile(l, (p->x + TILESIZE-1) / TILESIZE, (p->y + TILESIZE-1) / TILESIZE))) {
            p->x -= (p->x % TILESIZE);
        }
        outerAnimation = 6;
    }
    if (p->movUp) {
        p->y -= PLAYERSPEED - ((p->movLeft + p->movRight) * (PLAYERSPEED / 4));
        if (tileHasCollision(getTile(l, p->x / TILESIZE, p->y / TILESIZE)) || tileHasCollision(getTile(l, (p->x + TILESIZE-1) / TILESIZE, p->y / TILESIZE))) {
            p->y += TILESIZE - (p->y % TILESIZE);
        }
        outerAnimation = 9;
    }
    if (p->movDown) {
        p->y += PLAYERSPEED - (p->movLeft + p->movRight) * (PLAYERSPEED / 4);
        if (tileHasCollision(getTile(l, p->x / TILESIZE, (p->y + TILESIZE-1) / TILESIZE)) || tileHasCollision(getTile(l, (p->x + TILESIZE-1) / TILESIZE, (p->y + TILESIZE-1) / TILESIZE))) {
            p->y -= (p->y % TILESIZE);
        }
        outerAnimation = 0;
    }
    if (!(p->movDown || p->movUp || p->movRight || p->movLeft)) {
        innerAnimation = 0;
    }
    else {
        if (frameCounter % (FPS / 6) == 0) innerAnimation = (innerAnimation % 2) + 1;
    }
    p->playerSprite = p->sprites[outerAnimation + innerAnimation];
    frameCounter++;
}

//Moving the player without collsion for the editor
void movePlayerEditor(Player *p) {
    if (p->movUp) {
        p->y -= (2 * PLAYERSPEED) - ((p->movLeft + p->movRight) * ((2 * PLAYERSPEED) / 4));
    }
    if (p->movDown) {
        p->y += (2 * PLAYERSPEED) - (p->movLeft + p->movRight) * ((2 * PLAYERSPEED) / 4);
    }
    if (p->movLeft) {
        p->x -= (2 * PLAYERSPEED) - (p->movUp + p->movDown) * ((2 * PLAYERSPEED) / 4);
    }
    if (p->movRight) {
        p->x += (2 * PLAYERSPEED) - (p->movUp + p->movDown) * ((2 * PLAYERSPEED) / 4);
    }
}

void initPlayer(SDL_Renderer *r, Player *p) {
    p->sprites = malloc(12 * sizeof(SDL_Texture*));

    SDL_Surface *s = IMG_Load("src/data/sprites/MarvinD0.png");
    p->sprites[0] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinD1.png");
    p->sprites[1] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinD2.png");
    p->sprites[2] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinL0.png");
    p->sprites[3] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinL1.png");
    p->sprites[4] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinL2.png");
    p->sprites[5] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinR0.png");
    p->sprites[6] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinR1.png");
    p->sprites[7] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinR2.png");
    p->sprites[8] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinU0.png");
    p->sprites[9] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinU1.png");
    p->sprites[10] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    s = IMG_Load("src/data/sprites/MarvinU2.png");
    p->sprites[11] = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);

    p->playerSprite = p->sprites[0];
    
}

void cleanPlayer(Player *p) {
    for (int i = 0; i < 12; i++) {
        SDL_DestroyTexture(p->sprites[i]);
    }
    free(p->sprites);
}