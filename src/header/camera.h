#ifndef CAMERA_H
#define CAMERA_H

#include "player.h" // Ensure Player struct is defined for use in function prototypes

typedef struct {
    int x;
    int y;
} Camera;

// Function to initialize the camera's position
void initCamera(Camera *c, int x, int y);

// Function to set the camera's position
void setCameraPosition(Camera *c, int x, int y);

// Function to calculate and adjust the camera's position based on the player's position
void calculateCameraPosition(Camera *c, Player *p);

#endif // CAMERA_H
