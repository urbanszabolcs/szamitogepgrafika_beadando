#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL2/SDL.h>

typedef struct{
    int w,a,s,d;
    int mouseLeftClick;
    int mouseXRel, mouseYRel;
    int scroll;
}InputState;

void handleInput(SDL_Event* ev, InputState* state);

#endif