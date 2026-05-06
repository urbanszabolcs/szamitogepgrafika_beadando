#include "weather.h"
#include <GL/gl.h>
#include <math.h>

static float dayNightFactor = 0.5f;
static float cycleSpeed = 0.012f;   // Slower = longer cycle

void initWeather(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void updateWeather(float time)
{
    // Slow day/night cycle
    float cycle = fmodf(time * cycleSpeed, 1.0f);
    dayNightFactor = (sinf(cycle * 6.28318f) + 1.0f) / 2.0f;

    // Light blue sky during day, dark at night
    float skyR = 0.4f  + dayNightFactor * 0.45f;
    float skyG = 0.65f + dayNightFactor * 0.30f;
    float skyB = 0.95f;

    glClearColor(skyR, skyG, skyB, 1.0f);
    glFogfv(GL_FOG_COLOR, (float[]){skyR, skyG, skyB, 1.0f});
}

void applyLighting(void)
{
    // Sun position
    float sunAngle = dayNightFactor * 6.28318f - 1.57f;
    GLfloat lightPos[4] = {
        cosf(sunAngle) * 100.0f,
        sinf(sunAngle) * 80.0f + 30.0f,
        sinf(sunAngle) * 40.0f,
        0.0f
    };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Ambient light
    GLfloat ambient[4] = {
        0.25f + dayNightFactor * 0.35f,
        0.25f + dayNightFactor * 0.35f,
        0.35f + dayNightFactor * 0.35f,
        1.0f
    };

    // Diffuse light (sun)
    GLfloat diffuse[4] = {
        0.7f + dayNightFactor * 0.8f,
        0.65f + dayNightFactor * 0.7f,
        0.5f + dayNightFactor * 0.9f,
        1.0f
    };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
}

// Speed controls (called from main.c with numpad + / -)
void increaseDayNightSpeed(void)
{
    cycleSpeed += 0.003f;
    if (cycleSpeed > 0.1f) cycleSpeed = 0.1f;
}

void decreaseDayNightSpeed(void)
{
    cycleSpeed -= 0.003f;
    if (cycleSpeed < 0.003f) cycleSpeed = 0.003f;
}