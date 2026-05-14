#include "weather.h"
#include <GL/gl.h>
#include <math.h>

static float timeOffset = 0.0f;   // Tracks how much we have fast-forwarded/rewound time
static float currentCycle = 0.0f; // 0.0 to 1.0 (Full day)

// Global weights for blending colors
static float dayW, nightW, sunsetW;

void initWeather(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void updateWeather(float time)
{
    // 1. Apply the user's manual time manipulation (+ / - keys)
    float effectiveTime = time + timeOffset;
    // Calculate where we are in the day (0.0 to 1.0)
    float cycleSpeed = 0.003f;
    
    currentCycle = fmodf(effectiveTime * cycleSpeed, 1.0f);

    // Safety check in case the user rewinds time into negative numbers!
    if (currentCycle < 0.0f)
    {
        currentCycle += 1.0f;
    }

    // 2. Sun Math (sin(0)=Sunrise, sin(pi/2)=Noon, sin(pi)=Sunset, sin(3pi/2)=Midnight)
    float sunAngle = currentCycle * 6.283185f;
    float sunHeight = sinf(sunAngle);

    // 3. Calculate exactly how much Day, Night, and Sunset we should see
    dayW = sunHeight;
    if (dayW < 0.0f)
        dayW = 0.0f;

    nightW = -sunHeight;
    if (nightW < 0.0f)
        nightW = 0.0f;

    // Sunset peaks when the sun is crossing the horizon (sunHeight is near 0)
    sunsetW = 1.0f - fabsf(sunHeight * 3.5f);
    if (sunsetW < 0.0f)
        sunsetW = 0.0f;

    // Normalize weights so they always perfectly mix to 100%
    float totalW = dayW + nightW + sunsetW;
    dayW /= totalW;
    nightW /= totalW;
    sunsetW /= totalW;

    // 4. Mix the Sky Colors based on the weights
    float skyR = (0.3f * dayW) + (0.02f * nightW) + (0.8f * sunsetW);
    float skyG = (0.6f * dayW) + (0.02f * nightW) + (0.4f * sunsetW);
    float skyB = (0.9f * dayW) + (0.08f * nightW) + (0.2f * sunsetW);

    glClearColor(skyR, skyG, skyB, 1.0f);

    // The fog perfectly matches the sky color, making the horizon blend beautifully
    glFogfv(GL_FOG_COLOR, (float[]){skyR, skyG, skyB, 1.0f});
}

void applyLighting(void)
{
    float sunAngle = currentCycle * 6.283185f;

    // Sun/Moon Position
    // We use fabsf(sinf) on the Y axis so when the sun sets, the "Moon" rises from the same horizon!
    GLfloat lightPos[4] = {
        cosf(sunAngle) * 100.0f,
        fabsf(sinf(sunAngle)) * 100.0f + 5.0f, // +5 keeps it slightly above the water at sunset
        sinf(sunAngle) * 40.0f,
        0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Ambient light (The base illumination in the shadows)
    GLfloat ambient[4] = {
        0.15f + (0.3f * dayW) + (0.20f * sunsetW), // Warm shadows at sunset
        0.15f + (0.3f * dayW) + (0.10f * sunsetW),
        0.25f + (0.3f * dayW) + (0.05f * sunsetW), // Cool blue shadows at night
        1.0f};

    // Diffuse light (The direct rays from the Sun/Moon)
    GLfloat diffuse[4] = {
        (0.8f * dayW) + (0.9f * sunsetW) + (0.15f * nightW),
        (0.8f * dayW) + (0.4f * sunsetW) + (0.15f * nightW),
        (0.8f * dayW) + (0.1f * sunsetW) + (0.25f * nightW), // Blue-ish moonlight
        1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
}

// ========================================================
// TIME MANIPULATION CONTROLS (Called from main.c)
// ========================================================

void increaseDayNightSpeed(void)
{
    // Fast-forward time by roughly 1 in-game hour
    timeOffset += 2.0f;
}

void decreaseDayNightSpeed(void)
{
    // Rewind time by roughly 1 in-game hour
    timeOffset -= 2.0f;
}