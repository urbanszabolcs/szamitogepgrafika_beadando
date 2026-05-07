//Change the waves.c file so the ship doesn't float, it always stays on the level of the water
#include "terrain.h"
#include <math.h>

#define WATER_SIZE 120

void drawWaves(float time)
{
    int size = WATER_SIZE;
    float step = 2.0f;

    glBegin(GL_QUADS);

    for (int x = -size; x < size; x++)
    {
        for (int z = -size; z < size; z++)
        {
            // Calculate wave height
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

            glVertex3f(x * step, h1, z * step);
            glVertex3f((x + 1) * step, h2, z * step);
            glVertex3f((x + 1) * step, h3, (z + 1) * step);
            glVertex3f(x * step, h4, (z + 1) * step);
        }
    }
    glEnd();
}

// Used for ship floating
float getWaterHeight(float x, float z, float time)
{
    float h = sinf(x * 0.15f + time * 0.7f) * 1.0f +
              cosf(z * 0.17f + time * 0.5f) * 0.65f +
              sinf((x + z) * 0.06f + time * 0.3f) * 0.35f;
    return h;
}