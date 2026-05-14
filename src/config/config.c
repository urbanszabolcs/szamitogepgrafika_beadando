#include "config.h"
#include "../hud/hud.h"
#include <GL/gl.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// --- Global Variables ---
static int configOpen = 0;             
static int currentResolutionIndex = 5; 
static SDL_Window *mainWindow = NULL;  

typedef struct
{
    int width;
    int height;
} Resolution;

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
    mainWindow = window;        
    currentResolutionIndex = 5; 
}

void Config_Toggle(void)
{
    configOpen = !configOpen; 
}

int Config_IsOpen(void)
{
    return configOpen; 
}

void Config_HandleInput(SDL_Event *ev)
{
    if (!configOpen) 
        return;

    // REMOVED the SDLK_ESCAPE block here so main.c can handle it!

    if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT)
    {
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

        float scaledMouseX = ((float)ev->button.x / winW) * 800.0f;
        float scaledMouseY = ((float)ev->button.y / winH) * 700.0f;

        for (int i = 0; i < numResolutions; i++)
        {
            int x = 200 + (i % 3) * 170;
            int y = 280 + (i / 3) * 70;

            if (scaledMouseX >= x && scaledMouseX <= x + 140 &&
                scaledMouseY >= y && scaledMouseY <= y + 55)
            {
                if (currentResolutionIndex != i)
                {
                    currentResolutionIndex = i;
                    int newW = resolutions[i].width;
                    int newH = resolutions[i].height;

                    printf("Changing resolution to: %dx%d\n", newW, newH);

                    if (mainWindow)
                    {
                        // We also disable SDL_WINDOW_FULLSCREEN_DESKTOP when changing resolutions
                        // so it snaps to the windowed resolution correctly!
                        SDL_SetWindowFullscreen(mainWindow, 0); 
                        SDL_SetWindowSize(mainWindow, newW, newH);
                        SDL_SetWindowPosition(mainWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                    }
                }
                break;
            }
        }

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

    int menuWidth = 540;
    int menuHeight = 460;
    int startX = 180;
    int startY = 180;

    HUD_RenderRect(startX, startY, menuWidth, menuHeight, 0.03f, 0.03f, 0.12f, 0.95f);
    HUD_RenderRect(startX + 40, startY + 30, menuWidth - 80, 55, 0.0f, 0.45f, 0.85f, 1.0f);
    HUD_RenderText("Settings", startX + 200, startY + 15, 0.45f, 1.0f, 1.0f, 1.0f);

    for (int i = 0; i < numResolutions; i++)
    {
        int x = 200 + (i % 3) * 170;
        int y = 280 + (i / 3) * 70;

        if (i == currentResolutionIndex)
        {
            HUD_RenderRect(x, y, 140, 55, 1.0f, 0.85f, 0.1f, 1.0f);
        }
        else
        {
            HUD_RenderRect(x, y, 140, 55, 0.25f, 0.3f, 0.75f, 0.9f);
        }

        char resName[32];
        snprintf(resName, sizeof(resName), "%dx%d", resolutions[i].width, resolutions[i].height);
        HUD_RenderText(resName, x + 15, y + 15, 0.35f, 1.0f, 1.0f, 1.0f);
    }

    int quitX = startX + menuWidth - 120;
    int quitY = startY + menuHeight - 60;

    HUD_RenderRect(quitX, quitY, 100, 40, 0.85f, 0.15f, 0.15f, 1.0f);
    HUD_RenderText("Quit", quitX + 25, quitY + 8, 0.4f, 1.0f, 1.0f, 1.0f);
}