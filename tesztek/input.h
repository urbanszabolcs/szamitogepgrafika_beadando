#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

typedef struct {
    int w, a, s, d;
    int mouseLeft;
    int mouseXRel, mouseYRel;
    int scroll;
} InputState;

void handleInput(SDL_Event* ev, InputState* state);

#endif