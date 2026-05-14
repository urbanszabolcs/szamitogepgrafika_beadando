#include "movement.h"
#include "../../terrain/terrain.h"   
#include <math.h> 

static float targetSpeed = 0.0f;  
static float currentSpeed = 0.0f; 

// --- NEW: Helper functions for the HUD and Restart logic ---
float GetCurrentSpeed(void) { return currentSpeed; }

void ResetMovement(void) {
    targetSpeed = 0.0f;
    currentSpeed = 0.0f;
}
// -----------------------------------------------------------

int UpdateMovement(const InputState* input, float* objX, float* objZ, float* objAngle) 
{
    float maxSpeedForward = 0.5f;
    float maxSpeedReverse = -0.125f;
    float acceleration = 0.002f; 
    float throttleChange = 0.008f; 
    int crashed = 0; // Track if we hit something this frame
    
    if (input->w) targetSpeed += throttleChange; 
    if (input->s) targetSpeed -= throttleChange; 
    
    if (targetSpeed > maxSpeedForward) targetSpeed = maxSpeedForward;
    if (targetSpeed < maxSpeedReverse) targetSpeed = maxSpeedReverse;

    if (currentSpeed < targetSpeed) {
        currentSpeed += acceleration;
        if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
    } else if (currentSpeed > targetSpeed) {
        currentSpeed -= acceleration;
        if (currentSpeed < targetSpeed) currentSpeed = targetSpeed; 
    }

    *objX += sinf(*objAngle * 0.0174f) * currentSpeed;
    *objZ += cosf(*objAngle * 0.0174f) * currentSpeed;

    float baseTurnSpeed = 1.2f; 
    float speedMultiplier = fabsf(currentSpeed) / maxSpeedForward; 
    float actualTurnSpeed = baseTurnSpeed * speedMultiplier;

    if (input->a) *objAngle += actualTurnSpeed;
    if (input->d) *objAngle -= actualTurnSpeed;

    // === ISLAND COLLISION ===
    if (checkIslandCollision(*objX, *objZ))
    {
        *objX -= sinf(*objAngle * 0.0174f) * 1.5f;
        *objZ -= cosf(*objAngle * 0.0174f) * 1.5f;
        currentSpeed = 0.0f;
        targetSpeed = 0.0f;
        crashed = 1; // We hit something!
    }
    
    return crashed;
}