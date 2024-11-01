#include <SDL2/SDL_mixer.h>

Mix_Chunk* clickSound;
Mix_Chunk* switchLevelSound;
Mix_Chunk* getItemSound;
Mix_Music* gameTheme;

void initSound() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer Error: %s\n", Mix_GetError());
    }
    clickSound = Mix_LoadWAV("src/data/sound/click.wav");
    switchLevelSound = Mix_LoadWAV("src/data/sound/levelChange.wav");
    getItemSound = Mix_LoadWAV("src/data/sound/receiveItem.wav");
    gameTheme = Mix_LoadMUS("src/data/sound/gameTheme.mp3");
}

void playClickSound() {
    Mix_PlayChannel(-1, clickSound, 0);
}

void playSwitchLevelSound() {
    Mix_PlayChannel(-1, switchLevelSound, 0);
}

void playGetItemSound() {
    Mix_PlayChannel(-1, getItemSound, 0);
}

void playGameTheme() {
    Mix_PlayMusic(gameTheme, -1);
}

void cleanSound() {
    Mix_FreeChunk(clickSound);
    Mix_FreeChunk(switchLevelSound);
    Mix_FreeChunk(getItemSound);
    Mix_FreeMusic(gameTheme);
    Mix_CloseAudio();
}