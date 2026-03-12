#include "island.h"
#include <math.h>
#include <stdlib.h>

// Kijavítva: Ha a math.h nem definiálja, mi megadjuk
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void drawIsland(Island* island) {
    float halfSize = island->size / 2.0f;
    float step = island->size / (float)island->detail;

    glPushMatrix(); // Elmentjük a mátrixot
    glTranslatef(island->x, -0.5f, island->z); // A sziget saját helyére toljuk

    for (int i = 0; i < island->detail; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= island->detail; j++) {
            for (int k = 0; k < 2; k++) {
                int row = i + k;
                float px = -halfSize + j * step;
                float pz = -halfSize + row * step;

                float dist = sqrt(px * px + pz * pz);
                float h = 0;
                
                if (dist < halfSize) {
                    // Itt használtuk az M_PI-t a domborulathoz
                    h = cosf((dist / halfSize) * (M_PI / 2.0f)) * island->height;
                    h += (float)(rand() % 100) / 500.0f;
                }

                // Színezés magasság szerint (Homok -> Fű -> Szikla)
                if (h > island->height * 0.7f) glColor3f(0.5f, 0.4f, 0.3f);
                else if (h > 0.1f) glColor3f(0.2f, 0.6f, 0.2f);
                else glColor3f(0.8f, 0.7f, 0.5f);

                glVertex3f(px, h, pz);
            }
        }
        glEnd();
    }
    glPopMatrix(); // Visszaállítjuk a mátrixot, hogy ne zavarja a hajót
}