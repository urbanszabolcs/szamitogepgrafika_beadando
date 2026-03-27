#ifndef HUD_H
#define HUD_H

#include <GL/gl.h>

// HUD inicializálása a képernyő méreteivel
void HUD_Init(int screenWidth, int screenHeight);

// Alapvető rajzoló függvények
void HUD_RenderRect(float x, float y, float width, float height, float r, float g, float b, float a);
void HUD_RenderTexture(GLuint texture, float x, float y, float width, float height);
void HUD_RenderTextureRotated(GLuint texture, float x, float y, float width, float height, float rotationRadians);

// A teljes HUD felület kirajzolása a megadott vázlat alapján
void HUD_DrawLayout(GLuint ammoTexture, GLuint speedTexture, GLuint compassTexture, float healthPercentage, float shipYawRadians);

#endif // HUD_H