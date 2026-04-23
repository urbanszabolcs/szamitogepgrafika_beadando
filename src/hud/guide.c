#include "guide.h"
#include "hud.h" // The rendering is needed from this (HUD_RenderTexture)
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <stdio.h>

static bool isVisible = false;
static GLuint guideTextureID = 0;
static int texWidth = 0;
static int texHeight = 0;

void Guide_Init(void)
{
    // Loading the guide picture into memory , Might use something else
    SDL_Surface *loadedSurface = SDL_LoadBMP("guide.bmp");
    if (!loadedSurface)
    {
        printf("Hiba a guide.bmp betoltesekor: %s\n", SDL_GetError());
        return;
    }

    // Convert to standard 32-bit RGBA format
    //  This way it doesn't matter how Paint saved it, OpenGL won't crash.
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(loadedSurface); // Remove the guide from memory

    if (!surface)
    {
        printf("Hiba a kep konvertalasakor: %s\n", SDL_GetError());
        return;
    }

    texWidth = surface->w;
    texHeight = surface->h;

    // Converting SDL_Surface to OpenGL texture
    glGenTextures(1, &guideTextureID);
    glBindTexture(GL_TEXTURE_2D, guideTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    // Texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);
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
    // If it's not enabled, or the image failed to load, we do nothing.
    if (!isVisible || guideTextureID == 0)
        return;

    // Drawing the guide in the center of the screen (or at a fixed point).
    HUD_RenderTexture(guideTextureID, 0.0f, 0.0f, (float)texWidth, (float)texHeight);
}

void Guide_Cleanup(void)
{
    if (guideTextureID != 0)
    {
        glDeleteTextures(1, &guideTextureID);
        guideTextureID = 0;
    }
}