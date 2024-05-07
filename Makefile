build:
	gcc -o2 -Wall -o MyGame ./src/*.c -lSDL2 -lSDL2_ttf

run:
	./MyGame

clear:
	rm MyGame

fresh:
	make clear
	make build
	make run