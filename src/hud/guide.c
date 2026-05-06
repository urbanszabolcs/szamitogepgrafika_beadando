#include "guide.h"
#include "hud.h" 
#include <GL/gl.h>
#include <stdbool.h>

static bool isVisible = false;

void Guide_Init(void)
{
    // No more image loading! We just ensure it starts hidden.
    isVisible = false;
}

void Guide_Toggle(void)
{
    isVisible = !isVisible;
}

bool Guide_IsVisible(void)
{
    return isVisible;
}

void Guide_Render(void)
{
    if (!isVisible)
        return;

    // Use your 800x700 virtual HUD space to center the panel
    float panelX = 100.0f;
    float panelY = 100.0f;
    float panelW = 600.0f;
    float panelH = 500.0f;

    // 1. Draw the semi-transparent dark background panel
    HUD_RenderRect(panelX, panelY, panelW, panelH, 0.05f, 0.05f, 0.08f, 0.92f);
    
    // 2. Draw a decorative blue line under the title
    HUD_RenderRect(panelX + 30, panelY + 75, panelW - 60, 3, 0.0f, 0.45f, 0.85f, 1.0f);

    // 3. Draw the Title
    // (Adjust the X coordinate if it isn't perfectly centered based on your font size)
    HUD_RenderText("Controls Guide", panelX + 160, panelY + 20, 0.55f, 1.0f, 1.0f, 1.0f); 

    // 4. Draw the Control Keys
    float startY = panelY + 110.0f;
    float gap = 50.0f;
    float textScale = 0.35f;

    // Left Column (The Keys)
    HUD_RenderText("W / A / S / D", panelX + 50, startY, textScale, 1.0f, 0.85f, 0.1f); // Yellow text
    HUD_RenderText("Mouse", panelX + 50, startY + gap * 1, textScale, 1.0f, 0.85f, 0.1f);
    HUD_RenderText("Scroll Wheel", panelX + 50, startY + gap * 2, textScale, 1.0f, 0.85f, 0.1f);
    HUD_RenderText("C", panelX + 50, startY + gap * 3, textScale, 1.0f, 0.85f, 0.1f);
    HUD_RenderText("O", panelX + 50, startY + gap * 4, textScale, 1.0f, 0.85f, 0.1f);
    HUD_RenderText("Numpad + / -", panelX + 50, startY + gap * 5, textScale, 1.0f, 0.85f, 0.1f);
    HUD_RenderText("ESC", panelX + 50, startY + gap * 6, textScale, 1.0f, 0.85f, 0.1f);

    // Right Column (The Actions)
    float rightColX = panelX + 250.0f;
    HUD_RenderText("- Move Ship", rightColX, startY, textScale, 1.0f, 1.0f, 1.0f); // White text
    HUD_RenderText("- Look Around", rightColX, startY + gap * 1, textScale, 1.0f, 1.0f, 1.0f);
    HUD_RenderText("- Zoom Camera In / Out", rightColX, startY + gap * 2, textScale, 1.0f, 1.0f, 1.0f);
    HUD_RenderText("- Open Settings Menu", rightColX, startY + gap * 3, textScale, 1.0f, 1.0f, 1.0f);
    HUD_RenderText("- Toggle This Guide", rightColX, startY + gap * 4, textScale, 1.0f, 1.0f, 1.0f);
    HUD_RenderText("- Change Day / Night Speed", rightColX, startY + gap * 5, textScale, 1.0f, 1.0f, 1.0f);
    HUD_RenderText("- Close Menus", rightColX, startY + gap * 6, textScale, 1.0f, 1.0f, 1.0f);
}

void Guide_Cleanup(void)
{
    // Since we aren't generating an OpenGL texture from a BMP anymore, 
    // there is absolutely nothing we need to clean up here!
}