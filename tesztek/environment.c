#include "environment.h"
#include <math.h>

void drawWater(float time) {
    int size = 50;
    float step = 2.0f;
    glBegin(GL_QUADS);
    for (int x = -size; x < size; x++) {
        for (int z = -size; z < size; z++) {
            // Hullámzás matek
            float h = sinf(x * 0.3f + time) * 0.4f + cosf(z * 0.3f + time) * 0.2f;
            
            glColor4f(0.0f, 0.5f, 0.8f, 0.7f); // Áttetsző kék
            glVertex3f(x * step, h, z * step);
            glVertex3f((x+1) * step, h, z * step);
            glVertex3f((x+1) * step, h, (z+1) * step);
            glVertex3f(x * step, h, (z+1) * step);
        }
    }
    glEnd();
}

void drawIsland(float x, float z) {
    // Itt a korábbi procedurális sziget kódod helye
}