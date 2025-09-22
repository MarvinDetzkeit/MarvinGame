#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "header/game.h"
#include "header/constants.h"
#include "header/player.h"
#include "header/camera.h"
#include "header/level.h"
#include "header/tiles.h"
#include "header/uiwidgets.h"
#include "header/animation.h"

#define VSYNC_WAIT_FRAMES 2

int main() {
    running = initialize();
    time_last_frame = SDL_GetTicks();
    int wait_frames = 0;
    while(running) {
        if (wait_frames == 0) {
            running = update(updatePTR);
            animateWater();
            wait_frames = VSYNC_WAIT_FRAMES;
        }
        wait_frames--;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render();
        SDL_RenderPresent(renderer);
    }
    cleanUp();
    return 0;
}
