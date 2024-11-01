CC=gcc
CFLAGS= -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
DEBUGFLAGS=-Wall -g
SRC_DIR=./src
GAME_FILES=$(filter-out $(SRC_DIR)/editor.c, $(wildcard $(SRC_DIR)/*.c))
EDITOR_FILES=$(filter-out $(SRC_DIR)/main.c $(SRC_DIR)/game.c, $(wildcard $(SRC_DIR)/*.c))

.PHONY: all build editor run clear fresh debug

all: build editor

build:
	$(CC) -o2 $(CFLAGS) -o MarvinGame $(GAME_FILES)

editor:
	$(CC) -o2 $(CFLAGS) -o MarvinEditor $(EDITOR_FILES)

run:
	./MarvinGame

clean:
	@if [ -f "MarvinGame" ]; then \
		rm "MarvinGame"; \
	fi
	@if [ -f "MarvinEditor" ]; then \
		rm "MarvinEditor"; \
	fi

fresh: build run

debug: clean
	$(CC) $(DEBUGFLAGS) -o MarvinGame $(GAME_FILES) $(CFLAGS)
	lldb ./MarvinGame

feditor: editor
	./MarvinEditor

deditor: clean
	$(CC) $(DEBUGFLAGS) -o MarvinEditor $(EDITOR_FILES) $(CFLAGS)
	lldb ./MarvinEditor
