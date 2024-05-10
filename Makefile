build:
	gcc -O2 -o MyGame ./src/*.c -lSDL2 -lSDL2_ttf -lSDL2_image

run:
	./MyGame

clear:
	@if [ -f "$(MyGame)" ]; then \
		rm "$(MyGame)"; \
	fi

fresh:
	make clear
	make build
	make run

debug:
	make clear
	gcc -Wall -o MyGame ./src/*.c -lSDL2 -lSDL2_ttf -lSDL2_image -g
	lldb ./MyGame