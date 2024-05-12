CC=gcc
CFLAGS=-O2 -lSDL2 -lSDL2_ttf -lSDL2_image
DEBUGFLAGS=-Wall -g
SRC_DIR=./src
GAME_FILES=$(filter-out $(SRC_DIR)/editor.c, $(wildcard $(SRC_DIR)/*.c))
EDITOR_FILES=$(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))

.PHONY: all build editor run clear fresh debug

all: build editor

build:
	$(CC) $(CFLAGS) -o MyGame $(GAME_FILES)

editor:
	$(CC) $(CFLAGS) -o MyEditor $(EDITOR_FILES)

run:
	./MyGame

clean:
	@if [ -f "MyGame" ]; then \
		rm "MyGame"; \
	fi
	@if [ -f "MyEditor" ]; then \
		rm "MyEditor"; \
	fi

fresh: clear build run

debug: clear
	$(CC) $(DEBUGFLAGS) -o MyGame $(GAME_FILES) -lSDL2 -lSDL2_ttf -lSDL2_image
	lldb ./MyGame

feditor: clean editor
	./MyEditor
