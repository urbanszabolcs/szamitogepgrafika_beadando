//Change the waves.c file so the ship doesn't float, it always stays on the level of the water
#include "terrain.h"
#include <math.h>

#define WATER_SIZE 120

static const float STEP_SIZE = 2.0f;

void drawWaves(float time)
{
    int size = WATER_SIZE;

    glBegin(GL_QUADS);

    for (int x = -size; x < size; x++)
    {
        for (int z = -size; z < size; z++)
        {
            // The wave height is calculated based on the GRID index (x, z)
            float h1 = sinf(x * 0.15f + time * 0.7f) * 1.0f +
                       cosf(z * 0.17f + time * 0.5f) * 0.65f +
                       sinf((x + z) * 0.06f + time * 0.3f) * 0.35f;

            float h2 = sinf((x + 1) * 0.15f + time * 0.7f) * 1.0f +
                       cosf(z * 0.17f + time * 0.5f) * 0.65f +
                       sinf(((x + 1) + z) * 0.06f + time * 0.3f) * 0.35f;

            float h3 = sinf((x + 1) * 0.15f + time * 0.7f) * 1.0f +
                       cosf((z + 1) * 0.17f + time * 0.5f) * 0.65f +
                       sinf(((x + 1) + (z + 1)) * 0.06f + time * 0.3f) * 0.35f;

            float h4 = sinf(x * 0.15f + time * 0.7f) * 1.0f +
                       cosf((z + 1) * 0.17f + time * 0.5f) * 0.65f +
                       sinf((x + (z + 1)) * 0.06f + time * 0.3f) * 0.35f;

            // Bright base color
            float brightness = (h1 + h2 + h3 + h4) / 4.0f;
            float r = 0.1f + brightness * 0.15f;
            float g = 0.55f + brightness * 0.25f;
            float b = 0.85f;

            glColor3f(r, g, b);

            // The physical vertex in the 3D world is multiplied by STEP_SIZE
            glVertex3f(x * STEP_SIZE, h1, z * STEP_SIZE);
            glVertex3f((x + 1) * STEP_SIZE, h2, z * STEP_SIZE);
            glVertex3f((x + 1) * STEP_SIZE, h3, (z + 1) * STEP_SIZE);
            glVertex3f(x * STEP_SIZE, h4, (z + 1) * STEP_SIZE);
        }
    }
    glEnd();
}

// Used for ship floating
float getWaterHeight(float worldX, float worldZ, float time)
{
    // Without this, the ship calculates waves out of sync with the visual rendering.
    float gridX = worldX / STEP_SIZE;
    float gridZ = worldZ / STEP_SIZE;

    float h = sinf(gridX * 0.15f + time * 0.7f) * 1.0f +
              cosf(gridZ * 0.17f + time * 0.5f) * 0.65f +
              sinf((gridX + gridZ) * 0.06f + time * 0.3f) * 0.35f;
    
    return h;
}