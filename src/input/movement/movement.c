#include "movement.h"
#include "../../terrain/terrain.h"   
#include <math.h> 

void UpdateMovement(const InputState* input, float* objX, float* objZ, float* objAngle) 
{
    float moveSpeed = 0.5f; //Make moevement speed changeable with SHIFT key, Show the current speed lvl on the hud

    if (input->w)
    {
        *objX += sinf(*objAngle * 0.0174f) * moveSpeed;
        *objZ += cosf(*objAngle * 0.0174f) * moveSpeed;
    }
    if (input->s)
    {
        *objX -= sinf(*objAngle * 0.0174f) * moveSpeed;
        *objZ -= cosf(*objAngle * 0.0174f) * moveSpeed;
    }

    if (input->a)
        *objAngle += 2.0f;
    if (input->d)
        *objAngle -= 2.0f;

    // === Island Collision ===
    if (checkIslandCollision(*objX, *objZ))
    {
        *objX -= sinf(*objAngle * 0.0174f) * 1.2f;
        *objZ -= cosf(*objAngle * 0.0174f) * 1.2f;
    }
}