#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/gl.h>

void drawWaves(float time);
void drawIsland(void);
void drawTerrain(float time);
void initIslands(void);

float getWaterHeight(float x, float z, float time);
int checkIslandCollision(float x, float z);
#endif