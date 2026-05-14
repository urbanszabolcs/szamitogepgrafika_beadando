#include "terrain.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_ISLANDS 30

typedef struct
{
    float x, z;
    float radius;
    float height;
    float baseHeight;
} Island;

static Island islands[MAX_ISLANDS];
static int islandCount = 0;

// --- NEW: Procedural Shape Generator ---
// This distorts the radius based on the angle and the island's unique position
static float getDeformedRadius(float baseRadius, float angle, float seedX, float seedZ)
{
    // Combine 3 different waves to create an organic "blob" shape.
    // The multipliers (2.0, 3.0, 5.0) MUST be whole numbers so the island connects seamlessly at the end of the loop!
    float offset = 0.25f * sinf(angle * 2.0f + seedX) + 
                   0.15f * cosf(angle * 3.0f + seedZ) + 
                   0.10f * sinf(angle * 5.0f - seedX);
                   
    return baseRadius * (1.0f + offset);
}
// ---------------------------------------

void initIslands(void)
{
    srand(42); // Fixed seed for consistent islands

    // Spawn between 15 and 25 islands
    islandCount = 15 + rand() % 10;

    for (int i = 0; i < islandCount; i++)
    {
        // Spread them out across a huge -400 to +400 grid
        islands[i].x = (float)(rand() % 800 - 400);
        islands[i].z = (float)(rand() % 800 - 400);

        islands[i].radius = 18.0f + rand() % 18;
        islands[i].height = 8.0f + rand() % 13;
        islands[i].baseHeight = 0.0f; // Base at water level
    }
}

void drawIsland(float playerX, float playerZ)
{
    float maxDistSq = 350.0f * 350.0f;

    for (int i = 0; i < islandCount; i++)
    {
        Island is = islands[i];

        float dx = is.x - playerX;
        float dz = is.z - playerZ;

        if ((dx * dx + dz * dz) > maxDistSq)
        {
            continue; 
        }
        
        glPushMatrix();
        glTranslatef(is.x, is.baseHeight, is.z);

        // === Underwater dark base ===
        glColor3f(0.12f, 0.38f, 0.48f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, -2.0f, 0);
        for (int a = 0; a <= 32; a++)
        {
            float ang = a * 0.19635f; // (2 * PI) / 32
            float defRad = getDeformedRadius(is.radius * 1.2f, ang, is.x, is.z);
            glVertex3f(cosf(ang) * defRad, -2.0f, sinf(ang) * defRad);
        }
        glEnd();

        // === Sandy beach ===
        glColor3f(0.88f, 0.78f, 0.58f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0.2f, 0);
        for (int a = 0; a <= 32; a++)
        {
            float ang = a * 0.19635f;
            float defRad = getDeformedRadius(is.radius, ang, is.x, is.z);
            glVertex3f(cosf(ang) * defRad, 0.2f, sinf(ang) * defRad);
        }
        glEnd();

        // === Main green land body (3D) ===
        glColor3f(0.30f, 0.58f, 0.20f);
        glBegin(GL_QUAD_STRIP);
        for (int a = 0; a <= 32; a++)
        {
            float ang = a * 0.19635f;
            // Base of the slope matches the beach
            float rBase = getDeformedRadius(is.radius, ang, is.x, is.z);
            // Top of the slope matches the plateau
            float rTop = getDeformedRadius(is.radius * 0.58f, ang, is.x, is.z);

            glVertex3f(cosf(ang) * rBase, 0.2f, sinf(ang) * rBase);
            glVertex3f(cosf(ang) * rTop, is.height * 0.55f, sinf(ang) * rTop);
        }
        glEnd();

        // === Top plateau ===
        glColor3f(0.25f, 0.50f, 0.16f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, is.height * 0.55f, 0);
        for (int a = 0; a <= 32; a++)
        {
            float ang = a * 0.19635f;
            float defRad = getDeformedRadius(is.radius * 0.58f, ang, is.x, is.z);
            glVertex3f(cosf(ang) * defRad, is.height * 0.55f, sinf(ang) * defRad);
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

        // Calculate the exact angle from the island to the player's ship
        float angleToPlayer = atan2f(dz, dx);
        
        // Find out exactly how far the beach stretches out at this specific angle
        float actualIslandRadius = getDeformedRadius(islands[i].radius, angleToPlayer, islands[i].x, islands[i].z);

        if (distance < actualIslandRadius + 4.0f) // 4.0 = boat safety margin
            return 1; // Collision detected
    }
    return 0;
}