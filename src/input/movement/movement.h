#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../controls/controls.h"

// Returns 1 if we crashed into an island, 0 if safe
int UpdateMovement(const InputState* input, float* objX, float* objZ, float* objAngle);

// New functions to read and reset the engine
float GetCurrentSpeed(void);
void ResetMovement(void);

#endif