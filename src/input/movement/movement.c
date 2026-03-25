#include "movement.h"
#include <math.h> 

void UpdateMovement(const InputState* input, float* objX, float* objZ, float* objAngle) 
{
    // Mozgás logika
    if (input->w)
    {
        *objX += sinf(*objAngle * 0.0174f) * 0.2f;
        *objZ += cosf(*objAngle * 0.0174f) * 0.2f;
    }
    if (input->s)
    {
        *objX -= sinf(*objAngle * 0.0174f) * 0.2f;
        *objZ -= cosf(*objAngle * 0.0174f) * 0.2f;
    }

    if (input->a)
        *objAngle += 2.0f;
    if (input->d)
        *objAngle -= 2.0f;
}