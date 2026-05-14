#include "controls.h"

void handleInput(SDL_Event *ev, InputState *state)
{

    if (ev->type == SDL_KEYDOWN || ev->type == SDL_KEYUP)
    {
        int val = (ev->type == SDL_KEYDOWN);
        switch (ev->key.keysym.sym)
        {
        case SDLK_w:
            state->w = val;
            break;
        case SDLK_a:
            state->a = val;
            break;
        case SDLK_s:
            state->s = val;
            break;
        case SDLK_d:
            state->d = val;
            break;
        }
    }
    if (ev->type == SDL_MOUSEBUTTONDOWN && ev->button.button == SDL_BUTTON_LEFT)
        state->mouseLeftClick = 1;
    if (ev->type == SDL_MOUSEBUTTONUP && ev->button.button == SDL_BUTTON_LEFT)
        state->mouseLeftClick = 0;

    if (ev->type == SDL_MOUSEMOTION && state->mouseLeftClick)
    {
        state->mouseXRel = ev->motion.xrel;
        state->mouseYRel = ev->motion.yrel;
    }
    if (ev->type == SDL_MOUSEWHEEL)
        state->scroll = ev->wheel.y;
}