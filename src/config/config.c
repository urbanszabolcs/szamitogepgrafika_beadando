#include "config.h"
#include "../hud/hud.h"
#include <GL/gl.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// --- Global Variables ---
static int configOpen = 0;             // Tracks if the menu is currently visible
static int currentResolutionIndex = 5; // Tracks which resolution is active (default 1920x1080)
static SDL_Window *mainWindow = NULL;  // Pointer to the window so we can resize it

// Structure to store width and height pairs
typedef struct
{
    int width;
    int height;
} Resolution;

// List of available resolutions for the user to choose from
static Resolution resolutions[] = {
    {800, 600},
    {1024, 768},
    {1280, 720},
    {1366, 768},
    {1600, 900},
    {1920, 1080},
    {2560, 1440}};

static int numResolutions = 7;
// ------------------------

void Config_Init(SDL_Window *window)
{
    mainWindow = window;        // Store the window pointer for later use in resizing
    currentResolutionIndex = 5; // Start with the default selection
}

void Config_Toggle(void)
{
    configOpen = !configOpen; // Flip between 0 (closed) and 1 (open)
}

int Config_IsOpen(void)
{
    return configOpen; // Used by main.c to decide whether to call Render/HandleInput
}

void Config_HandleInput(SDL_Event *ev)
{
    if (!configOpen) // Don't process input if the menu is closed
        return;

    // Allow the user to close the menu using the Escape key
    if (ev->type == SDL_KEYDOWN && ev->key.keysym.sym == SDLK_ESCAPE)
    {
        configOpen = 0;
        return;
    }

    // Process mouse clicks
    if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT)
    {
        // 1. Get the actual current size of the window (which might have changed)
        int winW, winH;
        SDL_Window *currentWindow = SDL_GetWindowFromID(ev->button.windowID);
        if (currentWindow)
        {
            SDL_GetWindowSize(currentWindow, &winW, &winH);
        }
        else
        {
            winW = 1920;
            winH = 1080;
        }

        // 2. COORDINATE SCALING:
        // SDL mouse coordinates are in real pixels (e.g., 0-1920).
        // Our HUD is drawn in a virtual space (0-800).
        // We multiply the mouse position by (VirtualSize / RealSize) to find where
        // the click "lands" in our HUD menu.
        float scaledMouseX = ((float)ev->button.x / winW) * 800.0f;
        float scaledMouseY = ((float)ev->button.y / winH) * 700.0f;

        // 3. Check Resolution Boxes using Grid Math
        for (int i = 0; i < numResolutions; i++)
        {
            // (i % 3) keeps columns within 0, 1, 2
            // (i / 3) increments the row every 3 items
            int x = 200 + (i % 3) * 170;
            int y = 280 + (i / 3) * 70;

            // Check if the scaled mouse coordinates are inside this button's box
            if (scaledMouseX >= x && scaledMouseX <= x + 140 &&
                scaledMouseY >= y && scaledMouseY <= y + 55)
            {
                // If it's a new resolution, apply the change
                if (currentResolutionIndex != i)
                {
                    currentResolutionIndex = i;
                    int newW = resolutions[i].width;
                    int newH = resolutions[i].height;

                    printf("Changing resolution to: %dx%d\n", newW, newH);

                    if (mainWindow)
                    {
                        // Change the physical window size
                        SDL_SetWindowSize(mainWindow, newW, newH);
                        // Center it so it doesn't drift off screen
                        SDL_SetWindowPosition(mainWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                    }
                }
                break;
            }
        }

        // 4. Check the Quit Button

        int startX = 180;
        int startY = 180;
        int menuWidth = 540;
        int menuHeight = 460;
        int quitX = startX + menuWidth - 120;
        int quitY = startY + menuHeight - 60;

        if (scaledMouseX >= quitX && scaledMouseX <= quitX + 100 &&
            scaledMouseY >= quitY && scaledMouseY <= quitY + 40)
        {
            printf("Quit button clicked!\n");

            // GRACEFUL EXIT:
            // Instead of force-closing, we push an SDL_QUIT event.
            // main.c will catch this, stop its loop, and clean up memory properly.
            SDL_Event quitEvent;
            quitEvent.type = SDL_QUIT;
            SDL_PushEvent(&quitEvent);
            return;
        }
    }
}

void Config_Render(void)
{
    if (!configOpen)
        return;

    // Menu layout constants (in our 800x700 virtual space)
    int menuWidth = 540;
    int menuHeight = 460;
    int startX = 180;
    int startY = 180;

    // Background panel (Dark Blue/Gray)
    HUD_RenderRect(startX, startY, menuWidth, menuHeight, 0.03f, 0.03f, 0.12f, 0.95f);

    // Header title box
    HUD_RenderRect(startX + 40, startY + 30, menuWidth - 80, 55, 0.0f, 0.45f, 0.85f, 1.0f);

    // Title Text: 0.45f is the scale (shrunk from our large 64pt font)
    HUD_RenderText("Settings", startX + 200, startY + 15, 0.45f, 1.0f, 1.0f, 1.0f);

    // Draw the grid of resolution buttons
    for (int i = 0; i < numResolutions; i++)
    {
        // Must match the X/Y math used in HandleInput
        int x = 200 + (i % 3) * 170;
        int y = 280 + (i / 3) * 70;

        // Highlight the currently selected resolution in Yellow
        if (i == currentResolutionIndex)
        {
            HUD_RenderRect(x, y, 140, 55, 1.0f, 0.85f, 0.1f, 1.0f);
        }
        else
        {
            // Others are a standard Blue
            HUD_RenderRect(x, y, 140, 55, 0.25f, 0.3f, 0.75f, 0.9f);
        }

        // Convert resolution numbers to a string for display
        char resName[32];
        snprintf(resName, sizeof(resName), "%dx%d", resolutions[i].width, resolutions[i].height);

        // Render the text centered in each box
        HUD_RenderText(resName, x + 15, y + 15, 0.35f, 1.0f, 1.0f, 1.0f);
    }

    // Draw the Quit Button (Red)
    int quitX = startX + menuWidth - 120;
    int quitY = startY + menuHeight - 60;

    HUD_RenderRect(quitX, quitY, 100, 40, 0.85f, 0.15f, 0.15f, 1.0f);

    // Quit Text
    HUD_RenderText("Quit", quitX + 25, quitY + 8, 0.4f, 1.0f, 1.0f, 1.0f);
}