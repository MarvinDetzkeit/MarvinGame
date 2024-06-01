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

int main() {
    running = initialize();
    time_last_frame = SDL_GetTicks();
    while(running) {
        delay();
        SDL_SetRenderDrawColor(renderer, 0, 100, 150, 255);
        running = update(updatePTR);
        SDL_RenderClear(renderer);
        render();
        SDL_RenderPresent(renderer);
    }
    cleanUp();
    return 0;
}
