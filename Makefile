build:
	gcc -O2 -Wall -o MyGame ./src/*.c -lSDL2 -lSDL2_ttf -lSDL2_image

run:
	./MyGame

clear:
	rm MyGame

fresh:
	make clear
	make build
	make run