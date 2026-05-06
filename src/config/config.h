#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL.h>

void Config_Init(SDL_Window* window);
void Config_Toggle(void);
int Config_IsOpen(void);
void Config_HandleInput(SDL_Event* ev);
void Config_Render(void);

#endif