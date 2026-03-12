#include "camera.h"
#include <GL/glu.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void updateCamera(float objX, float objY, float objZ, float angleX, float angleY, float dist) {
    float radX = angleX * M_PI / 180.0f;
    float radY = angleY * M_PI / 180.0f;

    float eyeX = objX + dist * cosf(radX) * sinf(radY);
    float eyeY = objY + dist * sinf(radX);
    float eyeZ = objZ + dist * cosf(radX) * cosf(radY);

    gluLookAt(eyeX, eyeY, eyeZ, objX, objY, objZ, 0, 1, 0);
}