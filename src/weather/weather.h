#ifndef WEATHER_H
#define WEATHER_H

void initWeather(void);
void updateWeather(float time);
void applyLighting(void);

// Day/Night cycle speed control
void increaseDayNightSpeed(void);
void decreaseDayNightSpeed(void);

void drawShipShadow(float x, float z, float angle, float time);

#endif