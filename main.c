#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/input/controls/controls.h"
#include "src/input/movement/movement.h"
#include "src/input/camera/camera.h"
#include "src/models/model.h"
#include "src/terrain/terrain.h"
#include "src/hud/hud.h"
#include "src/hud/guide.h"
#include "src/weather/weather.h"
#include "src/config/config.h"

#define pi 3.1415926535

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Teszt", 100, 100, 1920, 1080, SDL_WINDOW_OPENGL);
    SDL_GLContext ctx = SDL_GL_CreateContext(win);

    glEnable(GL_DEPTH_TEST);

    // Initialize systems
    initIslands();
    initWeather();
    Config_Init(win);

    // Fog
    glEnable(GL_FOG);
    glFogf(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 80.0f);
    glFogf(GL_FOG_END, 300.0f);
    glFogfv(GL_FOG_COLOR, (float[]){0.6f, 0.75f, 0.9f, 1.0f});

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1920.0f / 1080.0f, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    loadModel("assets/models/player/player_model.obj");

    HUD_Init(800, 700);
    HUD_LoadFont("assets/fonts/roboto_font.ttf", 64); // Make sure the path points to a real .ttf file!
    Guide_Init();

    InputState input = {0};
    float playerX = 0, playerY = 5.0f, playerZ = 0, playerAngle = 0;
    float camAngleX = 20, camAngleY = 0, camDist = 25;
    int running = 1;

    float playerHP = 1.0f;

    while (running)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                running = 0;

            // Catch window resize events to update OpenGL's aspect ratio
            if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                int newWidth = ev.window.data1;
                int newHeight = ev.window.data2;

                // Update the viewport to the new size
                glViewport(0, 0, newWidth, newHeight);

                // Update the 3D perspective matrix so nothing looks stretched
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45.0, (float)newWidth / (float)newHeight, 0.1, 1000.0);
                glMatrixMode(GL_MODELVIEW);
            }
            // --------------------------

            Config_HandleInput(&ev);

            if (ev.type == SDL_KEYDOWN)
            {
                if (ev.key.keysym.sym == SDLK_F1)
                    Guide_Toggle();
                if (ev.key.keysym.sym == SDLK_ESCAPE)
                {
                    if (Guide_IsVisible())
                        Guide_Toggle();
                }
                
                if (ev.key.keysym.sym == SDLK_c) // Press C to open config menu
                    Config_Toggle();

                if (ev.key.keysym.sym == SDLK_KP_PLUS)
                    increaseDayNightSpeed();
                if (ev.key.keysym.sym == SDLK_KP_MINUS)
                    decreaseDayNightSpeed();

                // Handle config navigation
            }

            handleInput(&ev, &input);
        }

        UpdateMovement(&input, &playerX, &playerZ, &playerAngle);

        float currentTime = SDL_GetTicks() / 1000.0f;
        playerY = getWaterHeight(playerX, playerZ, currentTime) + 0.8f;

        camAngleY += input.mouseXRel * 0.5f;
        camAngleX += input.mouseYRel * 0.5f;
        camDist -= input.scroll * 2;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        updateCamera(playerX, playerY, playerZ, camAngleX, camAngleY, camDist);

        updateWeather(currentTime);
        applyLighting();

        drawTerrain(currentTime);

        glPushMatrix();
        glTranslatef(playerX, playerY, playerZ);
        glRotatef(playerAngle, 0, 1, 0);
        glColor3f(0.5f, 0.3f, 0.1f);
        renderModel();
        glPopMatrix();

        float playerAngleRadians = playerAngle * (pi / 180.0f);
        HUD_DrawLayout(0, 0, 0, playerHP, playerAngleRadians);

        if (Guide_IsVisible())
            Guide_Render();

        if (Config_IsOpen())
            Config_Render();

        SDL_GL_SwapWindow(win);
        SDL_Delay(16);
    }

    Guide_Cleanup();
    return 0;
}