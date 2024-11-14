# MarvinGame
A 2D-Topdown game with Pixel graphics, written in C using [SDL2](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.libsdl.org/&ved=2ahUKEwjytPvIxYqGAxU9X_EDHRvfBC4QFnoECBAQAQ&usg=AOvVaw0UKX-Hd5cnZaTK_nk7m-ZI). In the game, you play as me, the main mission is to find the Döner recipe.
This was my first ever C project, and I learned a lot. I made a lot of mistakes, the biggest one is how header files are used, which I won't change as it is already too late for that.

## Game
### Features
![Current State of the game](src/pics/game.png)

- Player can move
- Walking animation
- The camera scrolls with the player
- Collision
- Dialogue system
- Sound
- All pixel art (except one tile) was created by me

### Level Editor
![Level Editor](src/pics/levelEditor.png)

- Level editor for building levels up to 1000x1000 Tiles in size
- You can place Tiles and give them collision
- You can set the player spawn point
- You can place NPCs

## How to run
You need to install SDL2, SDL2\_ttf, SDL2\_image and SDL2\_mixer. You might have to add them to your Path as well.

### Install Dependencies on Mac
```
brew install sdl2 sdl2_ttf sdl2_image sdl2_mixer
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

## Credits
The game theme is [this song](https://www.youtube.com/watch?v=kCd83Zfcmto&list=PLjVTojiDW0-5PCkO2LO-L0RBHtEFu-HmD&index=3) by [Loeder](https://x.com/LoederMusic).

All except one sound effect are from the game [The Legend of Zelda: A Link to the Past](https://en.wikipedia.org/wiki/The_Legend_of_Zelda:_A_Link_to_the_Past).