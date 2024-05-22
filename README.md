# MarvinGame
A 2D-Topdown game with Pixel graphics, written in C using [SDL2](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.libsdl.org/&ved=2ahUKEwjytPvIxYqGAxU9X_EDHRvfBC4QFnoECBAQAQ&usg=AOvVaw0UKX-Hd5cnZaTK_nk7m-ZI). The name is not final, will change when I figure out what the game will become.

## Current State
### Game
![Current State of the game](src/pics/game.png)

- You can move the player (currently a red cube) through a level
- The camera scrolls with the player
- The player can collide with objects (e.g. the fence in the picture)
- You can talk to NPCs (In the picture the player talks to the blue cube)
- Most Pixel Art was created by me

### Level Editor
![Level Editor](src/pics/levelEditor.png)

- Level editor for building levels up to 1000x1000 Tiles in size
- The code for the editor is a mess

## What is planned
- [ ] Add more Pixel Art (Tiles and Characters)
- [x] Add NPCs
- [x] Add a dialogue system
- [ ] Add an animation system
- [ ] Add sound
- [ ] Figure out gameplay
- [ ] Add Menu
- [ ] Add functionality to save game
- [ ] Clean up code (maybe)


## How to run (On Mac and Linux)
If you want to run this very unfinished game or the level editor, you need to install SDL2, SDL2\_ttf and SDL2\_image. On Mac you can use homebrew:

### Install Dependencies on Mac
```
brew install sdl2 sdl2_ttf sdl2_image
```

### Install Source files
```
git clone https://github.com/MarvinDetzkeit/MarvinGame
```
### Compile
```
cd MarvinGame
make
```
### Run the Game
```
./MarvinGame
```
### Run the editor
```
./MarvinEditor
```
