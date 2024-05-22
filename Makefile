CC=gcc
CFLAGS=-O2 -lSDL2 -lSDL2_ttf -lSDL2_image
DEBUGFLAGS=-Wall -g
SRC_DIR=./src
GAME_FILES=$(filter-out $(SRC_DIR)/editor.c, $(wildcard $(SRC_DIR)/*.c))
EDITOR_FILES=$(filter-out $(SRC_DIR)/main.c $(SRC_DIR)/game.c $(SRC_DIR)/objects.c $(SRC_DIR)/uiwidgets.c, $(wildcard $(SRC_DIR)/*.c))

.PHONY: all build editor run clear fresh debug

all: build editor

build:
	$(CC) $(CFLAGS) -o MarvinGame $(GAME_FILES)

editor:
	$(CC) $(CFLAGS) -o MarvinEditor $(EDITOR_FILES)

run:
	./MarvinGame

clean:
	@if [ -f "MarvinGame" ]; then \
		rm "MarvinGame"; \
	fi
	@if [ -f "MarvinEditor" ]; then \
		rm "MarvinEditor"; \
	fi

fresh: clean build run

debug: clean
	$(CC) $(DEBUGFLAGS) -o MarvinGame $(GAME_FILES) -lSDL2 -lSDL2_ttf -lSDL2_image
	lldb ./MarvinGame

feditor: clean editor
	./MarvinEditor

detitor: clean
	$(CC) $(DEBUGFLAGS) -o MarvinEditor $(EDITOR_FILES) -lSDL2 -lSDL2_ttf -lSDL2_image
	lldb ./MarvinEditor
