#include <stdio.h>

typedef struct
{
    int x;
    int y;
    int movUp;
    int movDown;
    int movLeft;
    int movRight;
} Player;


void printPosition(Player* p) {
    printf("Player Position: (%d, %d)\n", p->x, p->y);
}
