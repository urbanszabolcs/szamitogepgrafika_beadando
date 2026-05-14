#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/gl.h>

void initIslands(void);

// --- UPDATE: Add playerX and playerZ to these functions ---
void drawTerrain(float time, float playerX, float playerZ);
void drawWaves(float time, float playerX, float playerZ);
void drawIsland(float playerX, float playerZ);
// ----------------------------------------------------------

int checkIslandCollision(float x, float z);
float getWaterHeight(float worldX, float worldZ, float time);

#endif