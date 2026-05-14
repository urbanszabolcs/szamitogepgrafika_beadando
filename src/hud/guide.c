#include "guide.h"
#include "hud.h" 
#include <GL/gl.h>
#include <stdbool.h>

static bool isVisible = false;

void Guide_Init(void)
{
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

    float panelX = 100.0f;
    float panelY = 100.0f;
    float panelW = 600.0f;
    float panelH = 500.0f;

    HUD_RenderRect(panelX, panelY, panelW, panelH, 0.05f, 0.05f, 0.08f, 0.92f);
    HUD_RenderRect(panelX + 30, panelY + 75, panelW - 60, 3, 0.0f, 0.45f, 0.85f, 1.0f);

    HUD_RenderText("Controls Guide", panelX + 160, panelY + 20, 0.55f, 1.0f, 1.0f, 1.0f); 

    float startY = panelY + 110.0f;
    float gap = 55.0f; // Slightly increased gap since we removed a line
    float textScale = 0.35f;

    // Left Column (The Keys)
    HUD_RenderText("W / S", panelX + 50, startY, textScale, 1.0f, 0.85f, 0.1f);
    HUD_RenderText("A / D", panelX + 50, startY + gap * 1, textScale, 1.0f, 0.85f, 0.1f);
    HUD_RenderText("Mouse / Scroll", panelX + 50, startY + gap * 2, textScale, 1.0f, 0.85f, 0.1f);
    HUD_RenderText("F1", panelX + 50, startY + gap * 3, textScale, 1.0f, 0.85f, 0.1f);
    HUD_RenderText("ESC", panelX + 50, startY + gap * 4, textScale, 1.0f, 0.85f, 0.1f);
    HUD_RenderText("Numpad + / -", panelX + 50, startY + gap * 5, textScale, 1.0f, 0.85f, 0.1f);

    // Right Column (The Actions)
    float rightColX = panelX + 250.0f;
    HUD_RenderText("- Engine Throttle (Ahead/Reverse)", rightColX, startY, textScale, 1.0f, 1.0f, 1.0f);
    HUD_RenderText("- Steer Ship", rightColX, startY + gap * 1, textScale, 1.0f, 1.0f, 1.0f);
    HUD_RenderText("- Look Around & Zoom", rightColX, startY + gap * 2, textScale, 1.0f, 1.0f, 1.0f);
    HUD_RenderText("- Toggle This Guide", rightColX, startY + gap * 3, textScale, 1.0f, 1.0f, 1.0f);
    HUD_RenderText("- Settings / Close Menus", rightColX, startY + gap * 4, textScale, 1.0f, 1.0f, 1.0f);
    HUD_RenderText("- Change Day / Night Speed", rightColX, startY + gap * 5, textScale, 1.0f, 1.0f, 1.0f);
}

void Guide_Cleanup(void)
{
}