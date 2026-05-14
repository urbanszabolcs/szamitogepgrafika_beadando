#include "hud.h"
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

static int sWidth, sHeight;
static TTF_Font *hudFont = NULL; // Stores our loaded font

void HUD_Init(int screenWidth, int screenHeight)
{
    sWidth = screenWidth;
    sHeight = screenHeight;
}

// --- Font Initialization ---
void HUD_LoadFont(const char *fontPath, int fontSize)
{
    // Initialize the TTF engine if it hasn't been already
    if (TTF_Init() == -1)
    {
        printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
        return;
    }

    // Load the font file
    hudFont = TTF_OpenFont(fontPath, fontSize);
    if (!hudFont)
    {
        printf("Failed to load font %s: %s\n", fontPath, TTF_GetError());
    }
}

// Helper function: switching from 3D to 2D view
static void Begin2D(void)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Setting up orthographic (flat) projections (Left, Right, Bottom, Top, Near, Far)
    glOrtho(0.0, sWidth, sHeight, 0.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Helper function: returning to 3D
static void End2D(void)
{
    glEnable(GL_DEPTH_TEST);

    // --- CRITICAL FIXES FOR 3D RENDERING ---
    glDisable(GL_BLEND);               // Turn off UI transparency
    glDisable(GL_TEXTURE_2D);          // Turn off 2D texturing
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Reset global color to pure white
    // ---------------------------------------

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void HUD_RenderRect(float x, float y, float width, float height, float r, float g, float b, float a)
{
    Begin2D();
    glDisable(GL_TEXTURE_2D);

    glColor4f(r, g, b, a);

    glBegin(GL_QUADS);
    glVertex2f(x, y);                  // Top-Left
    glVertex2f(x + width, y);          // Top-Right
    glVertex2f(x + width, y + height); // Bottom-Right
    glVertex2f(x, y + height);         // Bottom-Left
    glEnd();

    End2D();
}

void HUD_RenderTextureRotated(GLuint texture, float x, float y, float width, float height, float rotationRadians)
{
    Begin2D();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // White color so we don't tint the texture

    // Calculating the center of rotation
    float cx = x + (width / 2.0f);
    float cy = y + (height / 2.0f);

    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    // glRotatef expects degrees, so we convert from radians to degrees
    glRotatef(rotationRadians * (180.0f / 3.14159265f), 0.0f, 0.0f, 1.0f);
    glTranslatef(-cx, -cy, 0.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + width, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + width, y + height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + height);
    glEnd();

    glPopMatrix();
    End2D();
}

void HUD_RenderTexture(GLuint texture, float x, float y, float width, float height)
{
    HUD_RenderTextureRotated(texture, x, y, width, height, 0.0f); // 0 degree rotation
}

// --- Text Rendering Function (with pitch fix) ---
// --- Text Rendering Function (with scaling support) ---
void HUD_RenderText(const char *text, float x, float y, float scale, float r, float g, float b)
{
    if (!hudFont || !text)
        return;

    SDL_Color color = {(Uint8)(r * 255), (Uint8)(g * 255), (Uint8)(b * 255), 255};
    SDL_Surface *originalSurface = TTF_RenderText_Blended(hudFont, text, color);
    if (!originalSurface)
        return;

    SDL_Surface *surface = SDL_ConvertSurfaceFormat(originalSurface, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(originalSurface);

    if (!surface)
        return;

    // Calculate the scaled drawing dimensions
    float drawW = surface->w * scale;
    float drawH = surface->h * scale;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    // Using GL_LINEAR makes scaled-down text look perfectly smooth
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Begin2D();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Draw the quad using the scaled width and height
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + drawW, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + drawW, y + drawH);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + drawH);
    glEnd();

    End2D();

    glDeleteTextures(1, &texture);
    SDL_FreeSurface(surface);
}

// --- MAIN LAYOUT BASED ON THE DRAWING ---
void HUD_DrawLayout(GLuint ammoTexture, GLuint speedTexture, GLuint compassTexture, float healthPercentage, float shipYawRadians, float currentSpeed)
{
    // 1. Ammo / Current weapon (Top-Left corner)
    if (ammoTexture != 0)
    {
        HUD_RenderTexture(ammoTexture, 20.0f, 20.0f, 180.0f, 80.0f);
    }
    else
    {
        // Placeholder gray box if no image is loaded
        HUD_RenderRect(20.0f, 20.0f, 180.0f, 80.0f, 0.3f, 0.3f, 0.3f, 0.8f);
    }

    // 2. HP Bar (Middle of the bottom-left section)
    float hpWidth = 250.0f;
    float hpHeight = 25.0f;
    float hpX = 20.0f;
    float hpY = sHeight - 160.0f; // Slightly above the bottom of the screen

    if (healthPercentage < 0.0f)
        healthPercentage = 0.0f;
    if (healthPercentage > 1.0f)
        healthPercentage = 1.0f;

    HUD_RenderRect(hpX, hpY, hpWidth, hpHeight, 0.2f, 0.0f, 0.0f, 1.0f);                    // Red background
    HUD_RenderRect(hpX, hpY, hpWidth * healthPercentage, hpHeight, 0.0f, 0.8f, 0.2f, 1.0f); // Green line

    // 3. Ship speed, Direction panel (Bottom left corner)
    if (speedTexture != 0)
    {
        HUD_RenderTexture(speedTexture, 20.0f, sHeight - 120.0f, 250.0f, 100.0f);
    }
    else
    {
        // Background box
        HUD_RenderRect(20.0f, sHeight - 120.0f, 250.0f, 100.0f, 0.1f, 0.2f, 0.4f, 0.8f);

        // --- NEW: Render the Speed Text ---
        char speedStr[64];
        // Multiply by 100 so "0.6f" shows as a cool "60 Knots"
        snprintf(speedStr, sizeof(speedStr), "Speed: %d kts", (int)(fabsf(currentSpeed) * 100));
        HUD_RenderText(speedStr, 35.0f, sHeight - 105.0f, 0.4f, 1.0f, 1.0f, 1.0f);

        // Render the Gear State (Ahead, Neutral, or Reverse)
        if (currentSpeed > 0.05f)
        {
            HUD_RenderText("Ahead", 35.0f, sHeight - 65.0f, 0.35f, 0.0f, 1.0f, 0.0f); // Green
        }
        else if (currentSpeed < -0.05f)
        {
            HUD_RenderText("Reverse", 35.0f, sHeight - 65.0f, 0.35f, 1.0f, 0.0f, 0.0f); // Red
        }
        else
        {
            HUD_RenderText("Neutral", 35.0f, sHeight - 65.0f, 0.35f, 0.7f, 0.7f, 0.7f); // Gray
        }
    }
    // 4. Compass (Top-right corner)
    float compassSize = 120.0f;
    float compassX = sWidth - compassSize - 20.0f;
    float compassY = 20.0f;

    if (compassTexture != 0)
    {
        // The rotation is negative so the compass dial spins opposite to the ship
        HUD_RenderTextureRotated(compassTexture, compassX, compassY, compassSize, compassSize, shipYawRadians);
    }
    else
    {
        // Placeholder yellow box
        HUD_RenderRect(compassX, compassY, compassSize, compassSize, 0.6f, 0.6f, 0.0f, 0.8f);
    }

    // --- NEW: Draw a static Red Triangle to mark "Forward" ---
    Begin2D();
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.85f, 0.15f, 0.15f, 1.0f); // Bright Red

    glBegin(GL_TRIANGLES);
    float centerX = compassX + (compassSize / 2.0f);
    // Draw an upward-pointing triangle right at the top middle of the compass
    glVertex2f(centerX, compassY - 8.0f);         // Top point
    glVertex2f(centerX - 8.0f, compassY + 12.0f); // Bottom left
    glVertex2f(centerX + 8.0f, compassY + 12.0f); // Bottom right
    glEnd();

    End2D();
}