#include "hud.h"
#include <GL/gl.h>
#include <math.h>

static int sWidth, sHeight;

void HUD_Init(int screenWidth, int screenHeight)
{
    sWidth = screenWidth;
    sHeight = screenHeight;
}

// Helper function: switching from 3D to 2D view
static void Begin2D(void)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Setting up orthographic (flat) projections (Left, Right, Borrom, Top, Near, Far)
    glOrtho(0.0, sWidth, sHeight, 0.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Helper function: returning to 3d
static void End2D(void)
{
    glEnable(GL_DEPTH_TEST);

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
    // glRotatef excepts degrees, so we convert frm radians to degrees
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

// --- MAIN LAYOUT BASED ON THE DRAWING ---
void HUD_DrawLayout(GLuint ammoTexture, GLuint speedTexture, GLuint compassTexture, float healthPercentage, float shipYawRadians)
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
        // Placeholder dark blue box
        HUD_RenderRect(20.0f, sHeight - 120.0f, 250.0f, 100.0f, 0.1f, 0.2f, 0.4f, 0.8f);
    }

    // 4. Compass (Top-right corner)
    float compassSize = 120.0f;
    if (compassTexture != 0)
    {
        // The rotation is negative, so that the texture rotates in the opposite direction
        HUD_RenderTextureRotated(compassTexture, sWidth - compassSize - 20.0f, 20.0f, compassSize, compassSize, -shipYawRadians);
    }
    else
    {
        // Placeholder yellow box
        HUD_RenderRect(sWidth - compassSize - 20.0f, 20.0f, compassSize, compassSize, 0.6f, 0.6f, 0.0f, 0.8f);
    }
}