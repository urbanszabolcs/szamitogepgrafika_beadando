#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "environment/environment.h"
#include "model/model.h"
#include "camera/camera.h"
#include "input/input.h"



int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Hajo Szimulacio 2026", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 800.0/600.0, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);

    loadModel("assets/boat-row-large.obj");

    InputState input = {0};
    float objX = 0, objZ = 0, objAngle = 0;
    float camAngleX = 20, camAngleY = 0, camDist = 20;
    int running = 1;

    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = 0;
            handleInput(&ev, &input);
        }

        // Mozgás logika
        if (input.w) { objX += sinf(objAngle*0.0174) * 0.2; objZ += cosf(objAngle*0.0174) * 0.2; }
        if (input.a) objAngle += 2.0;
        if (input.d) objAngle -= 2.0;
        
        camAngleY += input.mouseXRel * 0.5;
        camAngleX += input.mouseYRel * 0.5;
        camDist -= input.scroll * 2;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        updateCamera(objX, 0, objZ, camAngleX, camAngleY, camDist);

        drawWater(SDL_GetTicks() / 1000.0f);
        

        glPushMatrix();
            glTranslatef(objX, 0, objZ);
            glRotatef(objAngle, 0, 1, 0);
            glColor3f(0.5, 0.3, 0.1);
            renderModel();
        glPopMatrix();

        SDL_GL_SwapWindow(win);
        SDL_Delay(16);
    }
    return 0;
}