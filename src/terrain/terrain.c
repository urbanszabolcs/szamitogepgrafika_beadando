#include "terrain.h"

void drawTerrain(float time, float playerX, float playerZ)
{
    drawWaves(time, playerX, playerZ);
    drawIsland(playerX, playerZ);
}