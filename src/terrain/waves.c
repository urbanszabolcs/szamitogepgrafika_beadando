#include "terrain.h"
#include <math.h>

void drawWaves(float time)
{
    int size = 60;           // Reduced from 250 → much better performance
    float step = 2.0f;

    glColor4f(0.0f, 0.55f, 0.85f, 0.75f);   // Nicer water color
    glBegin(GL_QUADS);

    for (int x = -size; x < size; x++)
    {
        for (int z = -size; z < size; z++)
        {
            // === IMPROVED: Per-vertex height = smooth curved surface (no more blocky look) ===
            float h1 = sinf(x       * 0.28f + time) * 0.35f + cosf(z       * 0.28f + time) * 0.18f;
            float h2 = sinf((x + 1) * 0.28f + time) * 0.35f + cosf(z       * 0.28f + time) * 0.18f;
            float h3 = sinf((x + 1) * 0.28f + time) * 0.35f + cosf((z + 1) * 0.28f + time) * 0.18f;
            float h4 = sinf(x       * 0.28f + time) * 0.35f + cosf((z + 1) * 0.28f + time) * 0.18f;

            glVertex3f(x * step, h1, z * step);
            glVertex3f((x + 1) * step, h2, z * step);
            glVertex3f((x + 1) * step, h3, (z + 1) * step);
            glVertex3f(x * step, h4, (z + 1) * step);
        }
    }
    glEnd();
}