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
    SDL_Window *win = SDL_CreateWindow("Boating around", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GLContext ctx = SDL_GL_CreateContext(win);

    glEnable(GL_DEPTH_TEST);

    // Initialize systems
    initIslands();
    initWeather();
    Config_Init(win);

    // Fog
    glEnable(GL_FOG);
    glFogf(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 30.0f);
    glFogf(GL_FOG_END, 300.0f);
    glFogfv(GL_FOG_COLOR, (float[]){0.6f, 0.75f, 0.9f, 1.0f});

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1920.0f / 1080.0f, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    // --- TEXTURE AND MODEL LOADING ---
    loadModel("assets/models/player/CruiseLiner.obj");
    GLuint shipTexture = loadModelTexture("assets/models/player/CruiseLiner_BaseColor.png");

    // Compas
    GLuint compassTexture = loadModelTexture("assets/hud/compass.png");

    HUD_Init(800, 700);
    HUD_LoadFont("assets/fonts/roboto_font.ttf", 64);
    Guide_Init();

    InputState input = {0};
    float playerX = 0, playerY = 5.0f, playerZ = 0, playerAngle = 0;
    float camAngleX = 20, camAngleY = 0, camDist = 25;
    int running = 1;

    float playerHP = 1.0f;

    while (running)
    {
        SDL_Event ev;

        input.mouseXRel = 0;
        input.mouseYRel = 0;
        input.scroll = 0;

        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                running = 0;

            // Catch window resize events to update OpenGL's aspect ratio
            if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                int newWidth = ev.window.data1;
                int newHeight = ev.window.data2;

                glViewport(0, 0, newWidth, newHeight);

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45.0, (float)newWidth / (float)newHeight, 0.1, 1000.0);
                glMatrixMode(GL_MODELVIEW);
            }

            Config_HandleInput(&ev);

            if (ev.type == SDL_KEYDOWN)
            {
                if (ev.key.keysym.sym == SDLK_F1)
                    Guide_Toggle();

                // --- NEW ESCAPE LOGIC ---
                if (ev.key.keysym.sym == SDLK_ESCAPE)
                {
                    // If ANY menu is open, pressing ESC will close them
                    if (Guide_IsVisible() || Config_IsOpen())
                    {
                        if (Guide_IsVisible())
                            Guide_Toggle();
                        if (Config_IsOpen())
                            Config_Toggle();
                    }
                    else
                    {
                        // If no menus are open, pressing ESC opens the Settings!
                        Config_Toggle();
                    }
                }
                // ------------------------

                if (ev.key.keysym.sym == SDLK_KP_PLUS)
                    increaseDayNightSpeed();
                if (ev.key.keysym.sym == SDLK_KP_MINUS)
                    decreaseDayNightSpeed();
            }
            handleInput(&ev, &input);
        }

        int hasCrashed = UpdateMovement(&input, &playerX, &playerZ, &playerAngle);

        // 2. Handle HP and Restart
        if (hasCrashed)
        {
            playerHP -= 0.15f; // Lose 15% HP per crash

            // If HP hits 0, reset the game!
            if (playerHP <= 0.0f)
            {
                printf("Ship Destroyed! Restarting...\n");
                playerHP = 1.0f;
                playerX = 0.0f;
                playerZ = 0.0f;
                playerAngle = 0.0f;
                ResetMovement();

                // Optional: Randomize the islands again so it's a fresh game!
                initIslands();
            }
        }

        float currentTime = SDL_GetTicks() / 1000.0f;
        playerY = getWaterHeight(playerX, playerZ, currentTime) + 0.0f;

        camAngleY += input.mouseXRel * 0.5f;
        camAngleX += input.mouseYRel * 0.5f;
        camDist -= input.scroll * 2;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        updateCamera(playerX, playerY, playerZ, camAngleX, camAngleY, camDist);

        updateWeather(currentTime);
        applyLighting();

        drawTerrain(currentTime, playerX, playerZ);

        // --- RENDER THE PLAYER MODEL ---
        glPushMatrix();
        glTranslatef(playerX, playerY, playerZ);
        glRotatef(playerAngle, 0, 1, 0);

        // changeing the scale of the ship
        renderModel(shipTexture, 0.5f);

        glPopMatrix();

        float speedToDraw = GetCurrentSpeed();
        float playerAngleRadians = playerAngle * (pi / 180.0f);

        HUD_DrawLayout(0, 0, compassTexture, playerHP, playerAngleRadians, speedToDraw);
        
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