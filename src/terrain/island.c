#include "terrain.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_ISLANDS 8

typedef struct {
    float x, z;
    float radius;
    float height;
    float baseHeight;
} Island;

static Island islands[MAX_ISLANDS];
static int islandCount = 0;

void initIslands(void)
{
    srand(42);                    // Fixed seed for consistent islands
    islandCount = 6 + rand() % 3; // 6 to 8 islands

    for (int i = 0; i < islandCount; i++)
    {
        islands[i].x = (rand() % 320 - 160) * 1.8f;
        islands[i].z = (rand() % 320 - 160) * 1.8f;
        islands[i].radius = 18.0f + rand() % 18;
        islands[i].height = 8.0f + rand() % 13;
        islands[i].baseHeight = 0.0f;        // Base at water level
    }
}

void drawIsland(void)
{
    for (int i = 0; i < islandCount; i++)
    {
        Island is = islands[i];

        glPushMatrix();
        glTranslatef(is.x, is.baseHeight, is.z);

        // === Underwater dark base ===
        glColor3f(0.12f, 0.38f, 0.48f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, -2.0f, 0);
        for (int a = 0; a <= 32; a++) {
            float ang = a * 0.19635f;
            glVertex3f(cosf(ang) * is.radius * 1.2f, -2.0f, sinf(ang) * is.radius * 1.2f);
        }
        glEnd();

        // === Sandy beach ===
        glColor3f(0.88f, 0.78f, 0.58f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0.2f, 0);
        for (int a = 0; a <= 32; a++) {
            float ang = a * 0.19635f;
            glVertex3f(cosf(ang) * is.radius, 0.2f, sinf(ang) * is.radius);
        }
        glEnd();

        // === Main green land body (3D) ===
        glColor3f(0.30f, 0.58f, 0.20f);
        glBegin(GL_QUAD_STRIP);
        for (int a = 0; a <= 32; a++) {
            float ang = a * 0.19635f;
            float x1 = cosf(ang) * is.radius;
            float z1 = sinf(ang) * is.radius;

            glVertex3f(x1, 0.2f, z1);
            glVertex3f(x1 * 0.58f, is.height * 0.55f, z1 * 0.58f);
        }
        glEnd();

        // === Top plateau ===
        glColor3f(0.25f, 0.50f, 0.16f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, is.height * 0.55f, 0);
        for (int a = 0; a <= 32; a++) {
            float ang = a * 0.19635f;
            glVertex3f(cosf(ang) * is.radius * 0.58f, is.height * 0.55f, sinf(ang) * is.radius * 0.58f);
        }
        glEnd();

        glPopMatrix();
    }
}

// ====================== COLLISION ======================
int checkIslandCollision(float x, float z)
{
    for (int i = 0; i < islandCount; i++)
    {
        float dx = x - islands[i].x;
        float dz = z - islands[i].z;
        float distance = sqrtf(dx * dx + dz * dz);

        if (distance < islands[i].radius + 4.0f)   // 4.0 = boat safety margin
            return 1; // Collision detected
    }
    return 0;
}