#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
if (argc < 3) {
printf("Dieses Programm berechnet den Wert, den ein Tile annimmt, um ein Teleporter zu werden.\n");
printf("Nutze es wie folgt: ./calc <Tile Number> <Teleport Location>\n");
return 0;
}
int tile = atoi(argv[1]);
int location = atoi(argv[2]);
int newLabel = tile | (location << 16);
printf("%d\n", newLabel);
return 0;
}
