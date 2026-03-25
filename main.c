#define SDL_MAIN_HANDLED
//
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//

#include "src/input/controls/controls.h"
#include "src/input/camera/camera.h"
//#include "src/models/model.h"
#include "src/terrain/terrain.h"
//
#define pi 3.142857

//

int main(int argc, char **argv)
{
   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window* win = SDL_CreateWindow("Teszt", 100, 100, 800, 700, SDL_WINDOW_OPENGL);
   SDL_GLContext ctx = SDL_GL_CreateContext(win);
   
   glEnable(GL_DEPTH_TEST);
   glMatrixMode(GL_PROJECTION);
   gluPerspective(45.0, 800.0/700.0, 0.1, 1000.0);
   glMatrixMode(GL_MODELVIEW);

   //loadModel("assets/objects/player_model.obj");

   
}
