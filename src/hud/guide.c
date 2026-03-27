#include "guide.h"
#include "hud.h" // Szükség van rá a HUD_RenderTexture függvény miatt
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <stdio.h>

static bool isVisible = false;
static GLuint guideTextureID = 0;
static int texWidth = 0;
static int texHeight = 0;

void Guide_Init(void) {
    //Kép betöltése a memóriába
    SDL_Surface* loadedSurface = SDL_LoadBMP("guide.bmp");
    if (!loadedSurface) {
        printf("Hiba a guide.bmp betoltesekor: %s\n", SDL_GetError());
        return;
    }

    // Konvertáljuk át szabványos 32-bites RGBA formátumra!
    // Így teljesen mindegy, hogyan mentette el a Paint, az OpenGL nem fog kifagyni.
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(loadedSurface); // A régit töröljük a RAM-ból

    if (!surface) {
        printf("Hiba a kep konvertalasakor: %s\n", SDL_GetError());
        return;
    }

    texWidth = surface->w;
    texHeight = surface->h;

    //SDL_Surface konvertálása OpenGL textúrává
    glGenTextures(1, &guideTextureID);
    glBindTexture(GL_TEXTURE_2D, guideTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    
    // Szűrési beállítások
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
    // Ha nincs bekapcsolva, vagy nem sikerült betölteni a képet, nem csinálunk semmit
    if (!isVisible || guideTextureID == 0)
        return;

    // A Guide kirajzolása a képernyő közepére (vagy egy fix pontra).
    HUD_RenderTexture(guideTextureID, 100.0f, 100.0f, (float)texWidth, (float)texHeight);
}

void Guide_Cleanup(void)
{
    if (guideTextureID != 0)
    {
        glDeleteTextures(1, &guideTextureID);
        guideTextureID = 0;
    }
}