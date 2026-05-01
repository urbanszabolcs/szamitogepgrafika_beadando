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

#define pi 3.1415926535

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Teszt", 100, 100, 1920, 1080, SDL_WINDOW_OPENGL);
    SDL_GLContext ctx = SDL_GL_CreateContext(win);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 800.0 / 700.0, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    loadModel("assets/models/player/player_model.obj");

    HUD_Init(800, 700);   //Hud és F1 GUIDE Init               
    Guide_Init();         //Hud és F1 GUIDE Init      

    InputState input = {0};
    float playerX = 0, playerZ = 0, playerAngle = 0;
    float camAngleX = 20, camAngleY = 0, camDist = 20;
    int running = 1; // Condition of running

    // Test variable for playerHP
    float playerHP = 1.0f; 

    while (running)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                running = 0;
            
            //F1 BUTTOM FOR GUIDE
// F1 -> Currently its on O
            if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.sym == SDLK_o) {
                    Guide_Toggle();
                }
            }

            handleInput(&ev, &input);
        }

        UpdateMovement(&input, &playerX, &playerZ, &playerAngle);

        camAngleY += input.mouseXRel * 0.5;
        camAngleX += input.mouseYRel * 0.5;
        camDist -= input.scroll * 2;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        updateCamera(playerX, 0, playerZ, camAngleX, camAngleY, camDist);

        drawTerrain(SDL_GetTicks() / 1000.0f);

        glPushMatrix();
        glTranslatef(playerX, 0, playerZ);
        glRotatef(playerAngle, 0, 1, 0);
        glColor3f(0.5, 0.3, 0.1);
        renderModel();
        glPopMatrix();

        // ---------------------------------------------------------
        // Drawing 2D hud above 3D
        // ---------------------------------------------------------
        
        // Converting radian to degrees

        float playerAngleRadians = playerAngle * (pi / 180.0f);

        // Drawing the layout of the hud, currently its a square without texture
        HUD_DrawLayout(0, 0, 0, playerHP, playerAngleRadians);

        // If F1 is pressed render the hud
        if (Guide_IsVisible()) {
            Guide_Render();
        }
        // ---------------------------------------------------------

        SDL_GL_SwapWindow(win);
        SDL_Delay(16);
    }

    // Program végi takarítás
    Guide_Cleanup();
    // SDL_Quit...
    return 0;
}