#ifndef HUD_H
#define HUD_H

#include <GL/gl.h>

// Initialize the HUD with screen dimensions
void HUD_Init(int screenWidth, int screenHeight);

// Basic rendering functions
void HUD_RenderRect(float x, float y, float width, float height, float r, float g, float b, float a);
void HUD_RenderTexture(GLuint texture, float x, float y, float width, float height);
void HUD_RenderTextureRotated(GLuint texture, float x, float y, float width, float height, float rotationRadians);

// Draw the complete HUD layout based on the provided sketch
void HUD_DrawLayout(GLuint ammoTexture, GLuint speedTexture, GLuint compassTexture, float healthPercentage, float shipYawRadians, float currentSpeed);
void HUD_LoadFont(const char* fontPath, int fontSize);
// Add 'float scale' to the parameters
void HUD_RenderText(const char* text, float x, float y, float scale, float r, float g, float b);
#endif // HUD_H